#include <algorithm>
#include <chrono>
#include <random>

#include <QColor>
#include <QFont>
#include <QFontDatabase>
#include <QFile>
#include <QFileDialog>
#include <QHash>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
// #include <QTimer>

#include "../enums.h"
#include "../load_config/load_config.h"
#include "FrescoWindow.h"


FrescoWindow::FrescoWindow(const QString &data_file_path, const QString &rating_col_name, const Theme current_theme, QWidget *parent) : QMainWindow(parent) {
    this->config = load_config("res/default_config/FrescoWindowConfig.json");

    this->current_theme = current_theme;

    this->init_ui();
    this->create_connections();
    this->parse_csv(data_file_path, rating_col_name);
}
    // self.total_time = None
    // self.time_interval = self.config.time_interval
    // self.time_label_len = 3 - len(str(self.time_interval)) + len(str(self.time_interval).strip('0'))
    // self.remaining_time_timer = QTimer()
    // self.remaining_time_timer.timeout.connect(self.update_remaining_time_label)
    // self.remaining_time_timer.setInterval(self.time_interval)

void FrescoWindow::init_ui() {
    ui.setupUi(this);

    this->setFixedSize(this->size());
    this->switch_theme(this->current_theme);

    this->setWindowTitle(this->config["fresco_window_title"].toString());
    this->ui.cells_count_spin_box->setValue(this->config["fresco_cells_count_init_value"].toInt());
    this->cells_count_spin_box_changed();

    this->ui.win_cells_count_spin_box->setValue(this->config["fresco_win_cells_count_init_value"].toInt());
    this->ui.attempts_count_spin_box->setValue(this->config["fresco_attempts_count_init_value"].toInt());

    const QString lobster_font = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":lobster_font"))[0];

    this->ui.const_lower_label->setFont(QFont(lobster_font, this->config["fresco_const_lower_label_font_size"].toInt()));
    this->ui.const_upper_label->setFont(QFont(lobster_font, this->config["fresco_const_upper_label_font_size"].toInt()));
    this->ui.question_label->setFont(QFont(lobster_font, this->config["fresco_question_label_font_size"].toInt()));
    this->ui.remaining_time_label->setFont(QFont(lobster_font, this->config["fresco_remaining_time_label_font_size"].toInt()));
    this->ui.total_time_label->setFont(QFont(lobster_font, this->config["fresco_total_time_label_font_size"].toInt()));

    this->ui.clear_button->setText(this->config["fresco_clear_button_text"].toString());
    this->ui.generate_riddle_button->setText(this->config["fresco_generate_riddle_button_text"].toString());
    this->ui.select_questions_file_button->setText(this->config["fresco_select_questions_file_button_text"].toString());
    this->ui.start_timer_button->setText(this->config["fresco_start_timer_button_text"].toString());
    this->ui.stop_timer_button->setText(this->config["fresco_stop_timer_button_text"].toString());
    this->ui.refresh_questions_button->setText(this->config["fresco_refresh_questions_button_text"].toString());
    this->ui.roulette_button->setText(this->config["fresco_roulette_button_text"].toString());

    this->ui.question_label->setWordWrap(true);
    this->ui.const_upper_label->hide();
    this->ui.const_lower_label->hide();

    this->ui.img_label->setPixmap(QPixmap(":good_fresco_image"));

    this->ui.name_combo_box->clear();
    this->ui.name_combo_box->addItem(this->config["fresco_name_combo_box_placeholder"].toString());
}

void FrescoWindow::create_connections() {
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this, [=]{this->switch_theme(this->current_theme == Theme::dark ? Theme::light : Theme::dark);});
    connect(this->ui.clear_button, &QPushButton::clicked, this, [=]{this->clear();});
    connect(this->ui.generate_riddle_button, &QPushButton::clicked, this, [=]{this->generate_riddle();});
    connect(this->ui.refresh_questions_button, &QPushButton::clicked, this, [=]{this->import_questions_from_TeX();});
    connect(this->ui.roulette_button, &QPushButton::clicked, this, [=]{this->show_roulette_dialog();});
    connect(this->ui.select_questions_file_button, &QPushButton::clicked, this, [=]{this->select_questons_file();});
    connect(this->ui.start_timer_button, &QPushButton::clicked, this, [=]{this->start_timer();});
    // connect(this->stop_timer_button, &QPushButton::clicked, this, [=]{this->remaining_time_timer.stop();});

    connect(this->ui.cells_count_spin_box, QOverload<int>::of(&QSpinBox::valueChanged), this, [=]{this->cells_count_spin_box_changed();});
}

void FrescoWindow::parse_csv(const QString &data_file_path, const QString &rating_col_name) {
    QFile table_file(data_file_path);
    table_file.open(QFile::ReadOnly);

    const int rating_col_index = QString(table_file.readLine()).split(',').indexOf(rating_col_name);
    const QRegularExpression regex(",\s*(?=([^\"]*\"[^\"]*\")*[^\"]*$)");
    while (!table_file.atEnd()) {
        const QStringList line = QString(table_file.readLine()).split(regex);
        if (line.size() > rating_col_index && line[rating_col_index] != "" && this->is_name(line[0])) {
            this->add_student_to_combo_box(line[0].simplified(), line[rating_col_index].toInt());
        }
    }
}

void FrescoWindow::add_student_to_combo_box(const QString &name, const int rating) {
    this->ui.name_combo_box->addItem(name);
    this->rating_by_name[name] = rating;
}

void FrescoWindow::set_time_label() {}
    // self.total_time = self.calculate_time(self.rating[self.name_combo_box.currentText()])
    // self.total_time_label.setText(f'{self.total_time} секунд{self.last_letter(self.total_time)}')

void FrescoWindow::set_question_label() {
    if (this->questions_list.empty()) {
        this->ui.question_label->setText(this->config["fresco_question_placeholder"].toString());
        return;
    }
    this->ui.question_label->setText(this->questions_list.takeLast());
    this->ui.questions_count_label->setText(QString("Осталось вопросов: %1").arg(this->questions_list.size()));
}

void FrescoWindow::init_remaining_time_label() {}
    // self.remaining_time_label.setText(f'{self.total_time}{"." if self.time_label_len > 0 else ""}{"0" * self.time_label_len}')

void FrescoWindow::generate_riddle() {
    const QString current_name = this->ui.name_combo_box->currentText();
    if (current_name == this->config["fresco_name_combo_box_placeholder"].toString()) {
        return;
    }

    if (this->ui.name_combo_box->findText(current_name, Qt::MatchExactly) == -1) {
        this->add_student_to_combo_box(current_name, this->config["fresco_custom_student_rating"].toInt());
        this->ui.name_combo_box->setCurrentIndex(this->ui.name_combo_box->count() - 1);
    }
    // try:
    //     self.rating[self.name_combo_box.currentText()]
    // except KeyError:
    //     self.rating[self.name_combo_box.currentText()] = self.config.custom_student_rating

    QPixmap icon(this->ui.name_combo_box->size().height(), this->ui.name_combo_box->size().height());
    icon.fill(QColor(this->config["fresco_name_icon_color"].toString()));
    this->ui.name_combo_box->setItemData(this->ui.name_combo_box->currentIndex(), icon, Qt::DecorationRole);
    // self.set_time_label()
    this->set_question_label();
    // self.remaining_time_timer.stop()
    // self.init_remaining_time_label()
    this->ui.img_label->setPixmap(QPixmap(":good_fresco_image"));
    this->ui.const_upper_label->show();
    this->ui.const_lower_label->show();
    // self.remaining_time_label.show()
    // self.total_time_label.show()
    this->ui.question_label->show();
    this->setWindowTitle(QString("%1 ~ %2").arg(this->config["fresco_window_title"].toString()).arg(current_name));
}

void FrescoWindow::select_questons_file() {
    const QString questions_file_path = QFileDialog::getOpenFileName(this, "Выбрать файл с вопросами", ".", "TeX(*.tex);;");
    if (questions_file_path == "") {
        return;
    }
    this->questions_file_path = questions_file_path;
    this->import_questions_from_TeX();
}

void FrescoWindow::import_questions_from_TeX() {
    if (this->questions_file_path == "") {
        return;
    }

    QFile questions_file(this->questions_file_path);
    questions_file.open(QFile::ReadOnly);

    this->questions_list = QStringList();
    while (!questions_file.atEnd()) {
        const QString line = questions_file.readLine();
        this->questions_list.append(line.split('%')[0].simplified().replace("$", ""));
    }

    std::shuffle(this->questions_list.begin(), this->questions_list.end(),
                 std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    this->ui.questions_count_label->setText(QString("Осталось вопросов: %1").arg(this->questions_list.size()));
}

void FrescoWindow::set_evil_style() {
    this->ui.remaining_time_label->setText(this->config["fresco_time_over_text"].toString());
    this->ui.img_label->setPixmap(QPixmap(":evil_fresco_image"));
}

void FrescoWindow::start_timer() {}
    // if self.total_time is None:
    //     return
    // if self.total_time <= 0:
    //     self.set_evil_style()
    //     return
    // self.remaining_time_timer.stop()
    // self.img_label.setPixmap(self.good_fresco)
    // self.init_remaining_time_label()
    // self.remaining_time_timer.start()

void FrescoWindow::update_remaining_time_label() {}
    // current_time = round(float(self.remaining_time_label.text()) - self.time_interval / 1000, self.time_label_len)
    // self.remaining_time_label.setText(f'{current_time:g}')
    // if current_time <= 0:
    //     self.set_evil_style()
    //     self.remaining_time_timer.stop()

void FrescoWindow::cells_count_spin_box_changed() {
    this->ui.win_cells_count_spin_box->setMaximum(this->ui.cells_count_spin_box->value());
    this->ui.attempts_count_spin_box->setMaximum(this->ui.cells_count_spin_box->value());
}

void FrescoWindow::show_roulette_dialog() {}
    // self.roulette_window.close()
    // self.roulette_window = RouletteDialog(self.cells_count_spin_box.value(), self.win_cells_count_spin_box.value(), self.attempts_count_spin_box.value())
    // self.roulette_window.show()

void FrescoWindow::clear() {
    this->setWindowTitle(this->config["fresco_window_title"].toString());

    this->ui.const_upper_label->hide();
    this->ui.const_lower_label->hide();
    this->ui.question_label->hide();
    this->ui.remaining_time_label->hide();
    this->ui.total_time_label->hide();

    this->ui.img_label->setPixmap(QPixmap(":good_fresco_image"));
    this->ui.name_combo_box->setCurrentIndex(0);

    // self.remaining_time_timer.stop()
    // self.total_time = None
}

void FrescoWindow::switch_theme(const Theme theme) {
    this->current_theme = theme;

    QString theme_string;
    switch(this->current_theme) {
        case Theme::dark:
            theme_string = "dark";
            break;
        case Theme::light:
            theme_string = "light";
            break;
    }

    QFile qss_file(QString(":%1_theme").arg(theme_string));
    qss_file.open(QFile::ReadOnly);
    this->setStyleSheet(QLatin1String(qss_file.readAll()));
}

bool FrescoWindow::is_name(const QString &name) const {
    return name != "";
}

int FrescoWindow::calculate_time(const int rating) const {
    // expression = eval(self.config.calculate_time_function)
    // return max(0, expression)
    return rating;
}

QString FrescoWindow::last_letter(const int rating) const {
    if (rating >= 11 && rating <= 20) {
        return "";
    }
    if (QString::number(rating).at(-1).digitValue() >= 2 && QString::number(rating).at(-1).digitValue() <= 4) {
        return "ы";
    }
    if (QString::number(rating).at(-1).digitValue() == 1) {
        return "а";
    }
    return "";
}
