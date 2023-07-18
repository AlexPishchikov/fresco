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
#include <QTimer>

#include "../enums.h"
#include "../load_config/load_config.h"
#include "FrescoWindow.h"
#include "RouletteDialog.h"


FrescoWindow::FrescoWindow(const QString &data_file_path, const QString &rating_col_name, const Theme current_theme, QWidget *parent) : QMainWindow(parent) {
    this->config = load_config("res/default_config/FrescoWindowConfig.json");
    this->timer.setInterval(this->config["fresco_time_interval"].toInt());

    this->current_theme = current_theme;

    this->init_ui();
    this->create_connections();
    this->parse_csv(data_file_path, rating_col_name);
}

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
    connect(this->ui.stop_timer_button, &QPushButton::clicked, this, [=]{this->timer.stop();});
    connect(this->ui.cells_count_spin_box, QOverload<int>::of(&QSpinBox::valueChanged), this, [=]{this->cells_count_spin_box_changed();});

    connect(&this->timer, &QTimer::timeout, this, [=]{this->update_remaining_time_label();});
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
    this->time_by_name[name] = this->calculate_time(rating);
}

void FrescoWindow::set_time_label() {
    this->ui.total_time_label->setText(QString("%1 секунд%2").arg(this->time_by_name[this->ui.name_combo_box->currentText()])
                                                             .arg(this->last_letter(this->time_by_name[this->ui.name_combo_box->currentText()])));
}

void FrescoWindow::set_question_label() {
    if (this->questions_list.empty()) {
        this->ui.question_label->setText(this->config["fresco_question_placeholder"].toString());
        return;
    }
    this->ui.question_label->setText(this->questions_list.takeLast());
    this->ui.questions_count_label->setText(QString("Осталось вопросов: %1").arg(this->questions_list.size()));
}

void FrescoWindow::init_remaining_time_label() {
    const int precision = 3 - QString::number(this->config["fresco_time_interval"].toInt()).size() +
                            + QString::number(this->config["fresco_time_interval"].toInt()).remove(QRegularExpression("0+$")).size();

    const QString label_text = QString("%1%2%3").arg(this->time_by_name[this->ui.name_combo_box->currentText()])
                                                .arg(precision > 0 ? "." : "")
                                                .arg(QString("0").repeated(precision));

    this->ui.remaining_time_label->setText(label_text);
}

void FrescoWindow::generate_riddle() {
    const QString current_name = this->ui.name_combo_box->currentText();
    if (current_name == this->config["fresco_name_combo_box_placeholder"].toString()) {
        return;
    }

    if (this->ui.name_combo_box->findText(current_name, Qt::MatchExactly) == -1) {
        this->add_student_to_combo_box(current_name, this->config["fresco_custom_student_rating"].toInt());
        this->ui.name_combo_box->setCurrentIndex(this->ui.name_combo_box->count() - 1);
    }

    this->timer.stop();

    this->ui.img_label->setPixmap(QPixmap(":good_fresco_image"));

    this->set_question_label();
    this->ui.question_label->show();

    this->ui.const_upper_label->show();
    this->ui.const_lower_label->show();
    this->ui.total_time_label->show();

    this->init_remaining_time_label();
    this->ui.remaining_time_label->show();

    this->set_time_label();

    this->setWindowTitle(QString("%1 ~ %2").arg(this->config["fresco_window_title"].toString()).arg(current_name));

    QPixmap icon(this->ui.name_combo_box->size().height(), this->ui.name_combo_box->size().height());
    icon.fill(QColor(this->config["fresco_name_icon_color"].toString()));

    this->ui.name_combo_box->setItemData(this->ui.name_combo_box->currentIndex(), icon, Qt::DecorationRole);
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

void FrescoWindow::start_timer() {
    this->timer.stop();
    if (this->time_by_name[this->ui.name_combo_box->currentText()] == 0) {
        this->set_evil_style();
        return;
    }

    this->ui.img_label->setPixmap(QPixmap(":good_fresco_image"));
    this->init_remaining_time_label();

    this->timer.start();
}

void FrescoWindow::update_remaining_time_label() {
    const double current_time = this->ui.remaining_time_label->text().toDouble() - this->config["fresco_time_interval"].toInt() / 1000.0;

    if (current_time <= 0) {
        this->timer.stop();
        this->set_evil_style();
    }
    else {
        this->ui.remaining_time_label->setText(QString::number(current_time));
    }
}

void FrescoWindow::cells_count_spin_box_changed() {
    this->ui.win_cells_count_spin_box->setMaximum(this->ui.cells_count_spin_box->value());
    this->ui.attempts_count_spin_box->setMaximum(this->ui.cells_count_spin_box->value());
}

void FrescoWindow::show_roulette_dialog() {
    RouletteDialog(this->ui.cells_count_spin_box->value(), this->ui.win_cells_count_spin_box->value(), this->ui.attempts_count_spin_box->value()).show();
}

void FrescoWindow::clear() {
    this->setWindowTitle(this->config["fresco_window_title"].toString());

    this->ui.const_upper_label->hide();
    this->ui.const_lower_label->hide();
    this->ui.question_label->hide();
    this->ui.remaining_time_label->hide();
    this->ui.total_time_label->hide();

    this->ui.img_label->setPixmap(QPixmap(":good_fresco_image"));
    this->ui.name_combo_box->setCurrentIndex(0);

    this->timer.stop();
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

unsigned int FrescoWindow::calculate_time(const int rating) const {
    // expression = eval(self.config.calculate_time_function)
    // return max(0, expression)
    return rating;
}

QString FrescoWindow::last_letter(const int time) const {
    if (QString::number(time).last(2).toInt() >= 11 && QString::number(time).last(2).toInt() <= 20) {
        return "";
    }
    if (QString::number(time).last(1).toInt() >= 2 && QString::number(time).last(1).toInt() <= 4) {
        return "ы";
    }
    if (QString::number(time).last(1).toInt() == 1) {
        return "а";
    }
    return "";
}