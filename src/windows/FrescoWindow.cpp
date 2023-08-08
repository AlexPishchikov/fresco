#include <algorithm>
#include <chrono>
#include <random>

#include <QAudioOutput>
#include <QColor>
#include <QComboBox>
#include <QFont>
#include <QFontDatabase>
#include <QFile>
#include <QFileDialog>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueList>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QPixmap>
#include <QPointer>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QTimer>

#include "../enums.h"
#include "../load_config/load_config.h"
#include "FrescoWindow.h"
#include "RouletteDialog.h"


FrescoWindow::FrescoWindow(const QString &data_file_path, const QString &rating_col_name, const Theme current_theme, QWidget *parent) : QMainWindow(parent) {
    this->config = load_config(":fresco_window_config_default");
    this->check_config();

    this->evil_style = false;

    this->init_sound();
    this->init_ui();
    this->create_connections();
    this->parse_csv(data_file_path, rating_col_name);

    this->set_theme(current_theme);
}

bool FrescoWindow::is_name(const QString &name) const {
    return name != "";
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

unsigned int FrescoWindow::calculate_time(const int rating) const {
    QJSEngine engine;
    QJSValue fn = engine.evaluate(QString("(function(rating) { return %1; })").arg(this->config["fresco_calculate_time_function"].toString()));
    QJSValueList arg;
    arg << rating;

    return std::max(0, fn.call(arg).toInt());
}

void FrescoWindow::add_student_to_combo_box(const QString &name, const int rating) {
    this->ui.names_combo_box->addItem(name);
    this->time.push_back(this->calculate_time(rating));
}

void FrescoWindow::cells_count_spin_box_changed() {
    this->ui.win_cells_count_spin_box->setMaximum(this->ui.cells_count_spin_box->value());
    this->ui.attempts_count_spin_box->setMaximum(std::min(this->ui.cells_count_spin_box->value(), this->ui.win_cells_count_spin_box->value()));
}

void FrescoWindow::check_config() {
    this->config["fresco_cells_count_init_value"] = QJsonValue(std::max(3, std::min(26, this->config["fresco_cells_count_init_value"].toInt())));
    this->config["fresco_win_cells_count_init_value"] = QJsonValue(std::max(0, std::min(this->config["fresco_cells_count_init_value"].toInt(),
                                                                                        this->config["fresco_win_cells_count_init_value"].toInt())));
    this->config["fresco_attempts_count_init_value"] = QJsonValue(std::max(1, std::min(this->config["fresco_win_cells_count_init_value"].toInt(),
                                                                                       this->config["fresco_attempts_count_init_value"].toInt())));

    this->config["fresco_time_interval"] = QJsonValue(std::max(1, std::min(1000, this->config["fresco_time_interval"].toInt())));
}

void FrescoWindow::clear() {
    this->set_good_style();

    this->setWindowTitle(this->config["fresco_window_title"].toString());

    this->ui.start_timer_button->setEnabled(false);
    this->ui.stop_timer_button->setEnabled(false);

    this->ui.const_upper_label->hide();
    this->ui.const_lower_label->hide();
    this->ui.question_label->hide();
    this->ui.remaining_time_label->hide();
    this->ui.total_time_label->hide();

    this->ui.names_combo_box->setCurrentIndex(0);

    this->timer.stop();
}

void FrescoWindow::create_connections() {
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this, [=]{this->set_theme(static_cast<Theme>((this->current_theme + 1) % Theme::count));});
    connect(this->ui.clear_button, &QPushButton::clicked, this, [=]{this->clear();});
    connect(this->ui.generate_riddle_button, &QPushButton::clicked, this, [=]{this->generate_riddle();});
    connect(this->ui.refresh_questions_button, &QPushButton::clicked, this, [=]{this->import_questions_from_TeX();});
    connect(this->ui.roulette_button, &QPushButton::clicked, this, [=]{this->show_roulette_dialog();});
    connect(this->ui.select_questions_file_button, &QPushButton::clicked, this, [=]{this->select_questons_file();});
    connect(this->ui.start_timer_button, &QPushButton::clicked, this, [=]{this->start_timer();});
    connect(this->ui.stop_timer_button, &QPushButton::clicked, this, [=]{this->timer.stop();});

    connect(this->ui.names_combo_box, &QComboBox::currentTextChanged, this, [=]{this->update_buttons_status();});

    connect(this->ui.cells_count_spin_box, QOverload<int>::of(&QSpinBox::valueChanged), this, [=]{this->cells_count_spin_box_changed();});
    connect(this->ui.win_cells_count_spin_box, QOverload<int>::of(&QSpinBox::valueChanged), this, [=]{this->win_cells_count_spin_box_changed();});

    connect(&this->timer, &QTimer::timeout, this, [=]{this->update_remaining_time_label();});
}

void FrescoWindow::generate_riddle() {
    const QString current_name = this->ui.names_combo_box->currentText();

    if (this->ui.names_combo_box->findText(current_name, Qt::MatchExactly) == -1) {
        this->add_student_to_combo_box(current_name, this->config["fresco_custom_student_rating"].toInt());
        this->ui.names_combo_box->setCurrentIndex(this->ui.names_combo_box->count() - 1);
    }

    if (this->ui.names_combo_box->currentIndex() == 0) {
        return;
    }

    this->ui.start_timer_button->setEnabled(true);
    this->ui.stop_timer_button->setEnabled(true);

    this->timer.stop();

    this->set_good_style();

    this->set_question_label();
    this->ui.question_label->show();

    this->ui.const_upper_label->show();
    this->ui.const_lower_label->show();
    this->ui.total_time_label->show();

    this->init_remaining_time_label();
    this->ui.remaining_time_label->show();

    this->set_time_label();

    this->setWindowTitle(QString("%1 ~ %2").arg(this->config["fresco_window_title"].toString()).arg(current_name));

    QPixmap icon(this->ui.names_combo_box->size().height(), this->ui.names_combo_box->size().height());
    icon.fill(QColor(this->config["fresco_name_icon_color"].toString()));

    this->ui.names_combo_box->setItemData(this->ui.names_combo_box->currentIndex(), icon, Qt::DecorationRole);
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
    this->ui.refresh_questions_button->setEnabled(true);
}

void FrescoWindow::init_remaining_time_label() {
    const int precision = 3 - QString::number(this->config["fresco_time_interval"].toInt()).size() +
                            + QString::number(this->config["fresco_time_interval"].toInt()).remove(QRegularExpression("0+$")).size();

    const QString label_text = QString("%1%2%3").arg(this->time[this->ui.names_combo_box->currentIndex()])
                                                .arg(precision > 0 ? "." : "")
                                                .arg(QString("0").repeated(precision));

    this->ui.remaining_time_label->setText(label_text);
}

void FrescoWindow::init_sound() {
    QPointer<QMediaPlayer> spin_player = new QMediaPlayer(this);
    QPointer<QAudioOutput> audio_output = new QAudioOutput(this);
    audio_output->setMuted(true);
    spin_player->setAudioOutput(audio_output);
    spin_player->setSource(QUrl("qrc:spin_sound"));
    connect(spin_player, &QMediaPlayer::durationChanged, this, [=]{
        this->spin_sound_duration = spin_player->duration();
        this->ui.roulette_button->setEnabled(true);
    });
}

void FrescoWindow::init_ui() {
    this->ui.setupUi(this);

    this->setFixedSize(this->size());

    this->setWindowTitle(this->config["fresco_window_title"].toString());
    this->ui.cells_count_spin_box->setValue(this->config["fresco_cells_count_init_value"].toInt());
    this->ui.win_cells_count_spin_box->setValue(this->config["fresco_win_cells_count_init_value"].toInt());
    this->ui.attempts_count_spin_box->setValue(this->config["fresco_attempts_count_init_value"].toInt());

    this->cells_count_spin_box_changed();
    this->win_cells_count_spin_box_changed();

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

    this->ui.names_combo_box->clear();
    this->ui.names_combo_box->addItem(this->config["fresco_name_combo_box_placeholder"].toString());

    this->ui.generate_riddle_button->setEnabled(false);
    this->ui.refresh_questions_button->setEnabled(false);
    this->ui.roulette_button->setEnabled(false);
    this->ui.start_timer_button->setEnabled(false);
    this->ui.stop_timer_button->setEnabled(false);
}

void FrescoWindow::parse_csv(const QString &data_file_path, const QString &rating_col_name) {
    QFile table_file(data_file_path);
    table_file.open(QFile::ReadOnly);

    this->time.push_back(-1);

    const int rating_col_index = QString(table_file.readLine()).split(',').indexOf(rating_col_name);
    const QRegularExpression regex(",\s*(?=([^\"]*\"[^\"]*\")*[^\"]*$)");
    while (!table_file.atEnd()) {
        const QStringList line = QString(table_file.readLine()).split(regex);
        if (line.size() > rating_col_index && line[rating_col_index] != "" && this->is_name(line[0])) {
            this->add_student_to_combo_box(line[0].simplified(), line[rating_col_index].toInt());
        }
    }
}

void FrescoWindow::set_time_label() {
    this->ui.total_time_label->setText(QString("%1 секунд%2").arg(this->time[this->ui.names_combo_box->currentIndex()])
                                                             .arg(this->last_letter(this->time[this->ui.names_combo_box->currentIndex()])));
}

void FrescoWindow::set_question_label() {
    if (this->questions_list.empty()) {
        this->ui.question_label->setText(this->config["fresco_question_placeholder"].toString());
        return;
    }
    this->ui.question_label->setText(this->questions_list.takeLast());
    this->ui.questions_count_label->setText(QString("Осталось вопросов: %1").arg(this->questions_list.size()));
}

void FrescoWindow::select_questons_file() {
    const QString questions_file_path = QFileDialog::getOpenFileName(this, "Выбрать файл с вопросами", ".", "TeX(*.tex);;");
    if (questions_file_path == "") {
        return;
    }
    this->questions_file_path = questions_file_path;
    this->import_questions_from_TeX();
}

void FrescoWindow::set_evil_style() {
    this->evil_style = true;
    this->ui.remaining_time_label->setText(this->config["fresco_time_over_text"].toString());
    this->ui.img_label->setPixmap(QPixmap(QString(":evil_fresco_image_%1").arg(this->current_theme)));
}

void FrescoWindow::set_good_style() {
    this->evil_style = false;
    this->ui.img_label->setPixmap(QPixmap(QString(":good_fresco_image_%1").arg(this->current_theme)));
}

void FrescoWindow::set_theme(const Theme theme) {
    this->current_theme = theme;

    if (this->evil_style) {
        this->set_evil_style();
    }
    else {
        this->set_good_style();
    }

    QFile qss_file(QString(":theme_%1").arg(this->current_theme));
    qss_file.open(QFile::ReadOnly);
    this->setStyleSheet(QLatin1String(qss_file.readAll()));
}

void FrescoWindow::show_roulette_dialog() {
    if (!this->roulette.isNull()) {
        this->roulette->close();
    }
    this->roulette = new RouletteDialog(this->spin_sound_duration,
                                        this->ui.cells_count_spin_box->value(),
                                        this->ui.win_cells_count_spin_box->value(),
                                        this->ui.attempts_count_spin_box->value(),
                                        this);
    this->roulette->show();
}

void FrescoWindow::start_timer() {
    this->timer.stop();
    if (this->ui.names_combo_box->currentText() == this->config["fresco_name_combo_box_placeholder"].toString() || this->ui.question_label->text() == "") {
        return;
    }
    if (this->time[this->ui.names_combo_box->currentIndex()] == 0) {
        this->set_evil_style();
        return;
    }

    this->set_good_style();
    this->init_remaining_time_label();

    this->timer.setInterval(this->config["fresco_time_interval"].toInt());

    this->timer.start();
}

void FrescoWindow::update_buttons_status() {
    this->ui.start_timer_button->setEnabled(false);
    this->ui.stop_timer_button->setEnabled(false);

    if (this->ui.names_combo_box->currentText() == this->config["fresco_name_combo_box_placeholder"].toString()) {
        this->ui.generate_riddle_button->setEnabled(false);
    }
    else {
        this->ui.generate_riddle_button->setEnabled(true);
    }
}

void FrescoWindow::update_remaining_time_label() {
    const double current_time = this->ui.remaining_time_label->text().toDouble() - this->config["fresco_time_interval"].toInt() / 1000.0;

    if (current_time <= 0) {
        this->timer.stop();
        this->set_evil_style();
        return;
    }

    this->ui.remaining_time_label->setText(QString::number(current_time));

    if (current_time * 1000 < this->config["fresco_time_interval"].toInt()) {
        this->timer.stop();
        this->timer.setInterval(current_time * 1000);
        this->timer.start();
    }
}

void FrescoWindow::win_cells_count_spin_box_changed() {
    this->ui.attempts_count_spin_box->setMaximum(std::max(this->ui.win_cells_count_spin_box->value(), 1));
}
