#include <QChar>
#include <QFile>
#include <QMainWindow>
#include <QString>

#include "FrescoWindow.h"
#include "../load_config/load_config.h"


FrescoWindow::FrescoWindow(const QString &data_file_name, const QString &rating_col_name, const QString &current_theme, QWidget *parent) : QMainWindow(parent) {
    this->config = load_config("res/default_config/FrescoWindowConfig.json");

    ui.setupUi(this);

    this->setFixedSize(this->size());

    this->current_theme = current_theme;
    this->switch_theme(this->current_theme);
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this, [=]{this->switch_theme(this->current_theme == "dark" ? "light" : "dark");});
}
    // self.config = FrescoWindowConfig()

    // self.current_theme = theme
    // self.switch_palette(self.current_theme)

    // self.setFixedSize(self.size())

    // self.setWindowTitle(self.config.window_title)

    // self.cells_count_spin_box.setValue(self.config.cells_count_init_value)
    // self.cells_count_spin_box_changed()
    // self.win_cells_count_spin_box.setValue(self.config.not_empty_cells_count_init_value)
    // self.attempts_count_spin_box.setValue(self.config.attempts_init_value)
    // self.roulette_window = RouletteDialog(self.cells_count_spin_box.value(), self.win_cells_count_spin_box.value(), self.attempts_count_spin_box.value())

    // font_id = QFontDatabase.addApplicationFont(QDir.currentPath() + '/res/fonts/Lobster-Regular.ttf')
    // lobster_font = QFontDatabase.applicationFontFamilies(font_id)[0]

    // self.const_upper_label.setFont(QFont(lobster_font, self.config.const_upper_label_font_size))
    // self.const_lower_label.setFont(QFont(lobster_font, self.config.const_lower_label_font_size))
    // self.total_time_label.setFont(QFont(lobster_font, self.config.total_time_label_font_size))
    // self.question_label.setFont(QFont(lobster_font, self.config.question_label_font_size))
    // self.remaining_time_label.setFont(QFont(lobster_font, self.config.remaining_time_label_font_size))
    // self.question_label.setWordWrap(True)

    // self.generate_riddle_button.setText(self.config.generate_riddle_button_text)
    // self.start_timer_button.setText(self.config.start_timer_button_text)
    // self.stop_timer_button.setText(self.config.stop_timer_button_text)
    // self.select_questions_file_button.setText(self.config.select_questions_file_button_text)
    // self.roulette_button.setText(self.config.roulette_button_text)
    // self.refresh_questions_button.setText(self.config.refresh_questions_button_text)
    // self.clear_button.setText(self.config.clear_button_text)

    // self.const_upper_label.hide()
    // self.const_lower_label.hide()

    // self.rating = dict()
    // self.questions = list()
    // self.file_path = str()
    // self.total_time = None

    // self.good_fresco = QPixmap('res/images/good_fresco.png')
    // self.evil_fresco = QPixmap('res/images/evil_fresco.png')
    // self.img_label.setPixmap(self.good_fresco)

    // self.time_interval = self.config.time_interval
    // self.time_label_len = 3 - len(str(self.time_interval)) + len(str(self.time_interval).strip('0'))
    // self.remaining_time_timer = QTimer()
    // self.remaining_time_timer.timeout.connect(self.update_remaining_time_label)
    // self.remaining_time_timer.setInterval(self.time_interval)

    // self.generate_riddle_button.clicked.connect(self.generate_riddle)
    // self.select_questions_file_button.clicked.connect(self.select_questons_file)
    // self.refresh_questions_button.clicked.connect(lambda: self.import_questions_from_TeX(self.file_path))
    // self.roulette_button.clicked.connect(self.show_roulette_dialog)
    // self.cells_count_spin_box.valueChanged.connect(self.cells_count_spin_box_changed)
    // self.start_timer_button.clicked.connect(self.start_timer)
    // self.stop_timer_button.clicked.connect(self.remaining_time_timer.stop)
    // self.clear_button.clicked.connect(self.clear)
    // self.switch_palette_button.clicked.connect(lambda: self.switch_palette('light' if self.current_theme == 'dark' else 'dark'))

    // self.name_combo_box.clear()
    // self.name_combo_box.addItem(self.config.name_combo_box_placeholder)

    // self.parse_csv(file_name, rating_col_name)

void FrescoWindow::parse_csv(const QString &file_name, const QString &rating_col_name) {}
    // with open(file_name, encoding='utf8') as csv_file:
    //     names_col = csv.reader(csv_file).__next__()[0]
    //     csv_file.seek(0)
    //     reader = csv.DictReader(csv_file)
    //     for element in reader:
    //         if element[rating_col_name] == '' or not self.is_name(element[names_col]) {}
    //             continue
    //         try:
    //             self.add_student_to_combo_box(element[names_col].strip(), int(element[rating_col_name]))
    //         except:
    //             self.add_student_to_combo_box(element[names_col].strip(), self.config.custom_student_rating)

void FrescoWindow::add_student_to_combo_box(const QString &name, const double rating) {}
    // self.name_combo_box.addItem(name)
    // self.rating[name] = rating

void FrescoWindow::set_time_label() {}
    // self.total_time = self.calculate_time(self.rating[self.name_combo_box.currentText()])
    // self.total_time_label.setText(f'{self.total_time} секунд{self.last_letter(self.total_time)}')

void FrescoWindow::set_question_label(const QString &question) {}
    // if len(self.questions) == 0:
    //     self.question_label.setText(self.config.question_placeholder)
    //     return

    // self.question_label.setText(self.questions.pop())
    // self.questions_count_label.setText(f'Осталось вопросов: {len(self.questions)}')

void FrescoWindow::init_remaining_time_label() {}
    // self.remaining_time_label.setText(f'{self.total_time}{"." if self.time_label_len > 0 else ""}{"0" * self.time_label_len}')

void FrescoWindow::generate_riddle() {}
    // current_name = self.name_combo_box.currentText()
    // if self.name_combo_box.findText(current_name, Qt.MatchExactly) == -1:
    //     self.add_student_to_combo_box(current_name, self.config.custom_student_rating)
    //     self.name_combo_box.setCurrentIndex(self.name_combo_box.count() - 1)

    // try:
    //     self.rating[self.name_combo_box.currentText()]
    // except KeyError:
    //     self.rating[self.name_combo_box.currentText()] = self.config.custom_student_rating

    // if current_name != self.config.name_combo_box_placeholder:
    //     icon = QPixmap(self.name_combo_box.size().height(), self.name_combo_box.size().height())
    //     icon.fill(QColor(self.config.name_icon_color))
    //     self.name_combo_box.setItemData(self.name_combo_box.currentIndex(), icon, Qt.DecorationRole)
    //     self.set_time_label()
    //     self.set_question_label()
    //     self.remaining_time_timer.stop()
    //     self.init_remaining_time_label()
    //     self.img_label.setPixmap(self.good_fresco)
    //     self.const_upper_label.show()
    //     self.const_lower_label.show()
    //     self.remaining_time_label.show()
    //     self.total_time_label.show()
    //     self.question_label.show()
    //     self.setWindowTitle(f'{self.config.window_title} ~ {current_name}')

void FrescoWindow::select_questons_file() {}
    // file_path = str(QFileDialog.getOpenFileName('Выбрать файл с вопросами', '.', 'TeX(*.tex);;')[0])
    // self.import_questions_from_TeX(file_path)

void FrescoWindow::import_questions_from_TeX(const QString &file_path) {}
    // if file_path == '':
    //     return
    // self.file_path = file_path
    // self.questions = list()
    // with open(self.file_path, 'r') as questions_file:
    //     for line in questions_file:
    //         self.questions.append(line.split('%')[0].replace('$', '').strip() + '\n')

    // random.shuffle(self.questions)
    // self.questions_count_label.setText(f'Осталось вопросов: {len(self.questions)}')

void FrescoWindow::set_evil_style() {}
    // self.remaining_time_label.setText(self.config.time_over_text)
    // self.img_label.setPixmap(self.evil_fresco)

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

void FrescoWindow::cells_count_spin_box_changed() {}
    // self.win_cells_count_spin_box.setMaximum(self.cells_count_spin_box.value())
    // self.attempts_count_spin_box.setMaximum(self.cells_count_spin_box.value())

void FrescoWindow::show_roulette_dialog() {}
    // self.roulette_window.close()
    // self.roulette_window = RouletteDialog(self.cells_count_spin_box.value(), self.win_cells_count_spin_box.value(), self.attempts_count_spin_box.value())
    // self.roulette_window.show()

void FrescoWindow::closeEvent() {}
    // super().closeEvent(event)
    // self.roulette_window.close()

void FrescoWindow::clear() {}
    // self.remaining_time_timer.stop()
    // self.img_label.setPixmap(self.good_fresco)
    // self.const_upper_label.hide()
    // self.const_lower_label.hide()
    // self.remaining_time_label.hide()
    // self.total_time_label.hide()
    // self.question_label.hide()
    // self.name_combo_box.setCurrentIndex(0)
    // self.total_time = None
    // self.setWindowTitle(self.config.window_title)

void FrescoWindow::switch_theme(const QString &theme) {
    this->current_theme = theme;

    QFile qss_file(QString(":%1_theme").arg(theme));
    qss_file.open(QFile::ReadOnly);
    this->setStyleSheet(QLatin1String(qss_file.readAll()));
}

bool FrescoWindow::is_name(const QString &name) const {}
    // if name == '':
    //     return False
    // return True

double FrescoWindow::calculate_time(const double rating) const {}
    // expression = eval(self.config.calculate_time_function)
    // return max(0, expression)

QChar FrescoWindow::last_letter(const double rating) const {}
    // if int(rating) in range(11, 20)
    //     return ''
    // if int(str(rating)[-1]) in range(2, 5)
    //     return 'ы'
    // if str(rating)[-1] == '1':
    //     return 'а'
    // return ''
