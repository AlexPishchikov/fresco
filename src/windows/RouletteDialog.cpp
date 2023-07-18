#include <QDialog>
#include <QString>
#include <QWidget>

#include "../load_config/load_config.h"
// #include "../custom_widgets/QHoleLabel.h"
// #include "../custom_widgets/QRoundButton.h"
#include "RouletteDialog.h"


RouletteDialog::RouletteDialog(const int total, const int win, const int attempts, QWidget *parent) : QDialog(parent) {}
    // super().__init__()
    // self.setWindowFlag(Qt.FramelessWindowHint)

    // self.config = RouletteDialogConfig()

    // self.shot_sound = QSound(self.config.roulette_shot_sound_file_path)
    // self.spinning_sound = QSound(self.config.roulette_spinning_sound_file_path)
    // self.empty_sound = QSound(self.config.roulette_empty_sound_file_path)

    // self.cells_count = cells_count
    // self.not_empty_cells_count = not_empty_cells_count
    // self.attempts_count = attempts_count

    // self.shot_hole_label_size = self.config.shot_hole_label_size
    // self.button_size = self.config.button_size
    // self.button_margin = self.config.button_margin

    // self.button_offset_angle = -math.pi / 2
    // self.r_h = 3 * (self.button_size + 2 * self.button_margin) / self.cells_count + (self.button_size + self.button_margin) / (2 * math.sin(math.pi / self.cells_count))
    // self.r_v = 3 * (self.button_size + 2 * self.button_margin) / self.cells_count + (self.button_size + self.button_margin) / (2 * math.sin(math.pi / self.cells_count))
    // self.r_h *= self.config.window_height_mult
    // self.r_v *= self.config.window_width_mult
    // self.setFixedSize(int(2 * (self.r_h + self.button_margin) + self.button_size), int(2 * (self.r_v + self.button_margin) + self.button_size))

    // self.selected_cell = 0
    // self.not_empty_cells = list()
    // self.roulette_buttons = list()

    // self.spin_step = self.config.spin_step
    // self.current_time = 0
    // self.spinning_duration = self.get_spinning_duration()
    // self.spinning_timer = QTimer(self)
    // self.spinning_timer.timeout.connect(self.buttons_spinning)
    // self.spinning_timer.setInterval(self.spin_step)

    // self.place_buttons_round()
    // self.loading()

void RouletteDialog::show() {}
    // super().show()
    // self.spinning_sound.play()
    // self.spinning_timer.start()

void RouletteDialog::get_sound_duration(const QString& file_path) const {}
    // spinning_sound_file = self.spinning_sound.fileName()
    // with contextlib.closing(wave.open(spinning_sound_file, 'r')) as file:
    //     return round(file.getnframes() / float(file.getframerate()) * 1000)

void RouletteDialog::place_buttons() {}
    // font_id = QFontDatabase.addApplicationFont(QDir.currentPath() + '/res/fonts/Lobster-Regular.ttf')
    // lobster_font = QFontDatabase.applicationFontFamilies(font_id)[0]

    // angle = -2 * math.pi / self.cells_count
    // button_labels = [letter for letter in string.ascii_uppercase]
    // random.shuffle(button_labels)

    // for i in range(self.cells_count):
    //     coords = [
    //         int(self.r_h * math.cos(self.button_offset_angle + angle * i) + (self.size().width()  - self.button_size) / 2),
    //         int(self.r_v * math.sin(self.button_offset_angle + angle * i) + (self.size().height() - self.button_size) / 2),
    //     ]

    //     roulette_button = QRoundPushButton(self.button_size, self)
    //     roulette_button.setGeometry(QRect(coords[0], coords[1], self.button_size, self.button_size))
    //     roulette_button.setFont(QFont('Comic Sans MS', self.config.roulette_buttons_font_size))
    //     roulette_button.setText(button_labels[i])
    //     roulette_button.setShortcut(QKeySequence(button_labels[i]))
    //     roulette_button.clicked.connect(self.i_hate_python(i))

    //     self.roulette_buttons.append(roulette_button)

    // self.set_all_buttons_status(False)

    // self.close_button = QRoundPushButton(self.button_size * 2, self)
    // self.close_button.setGeometry(QRect(int(self.size().width() / 2 - self.button_size),
    //                                     int(self.size().height() / 2 - self.button_size),
    //                                     self.button_size * 2,
    //                                     self.button_size * 2))
    // self.close_button.setFont(QFont(lobster_font, self.config.close_button_font_size))
    // self.close_button.setText(self.config.close_button_init_text)
    // self.close_button.clicked.connect(self.close)

    // self.setStyleSheet(f'border-radius: {math.floor(self.size().width() / 2)}px; border: 2px solid black;')
    // self.mask = QRegion(self.rect(), QRegion.Ellipse)
    // self.setMask(self.mask)

void RouletteDialog::loading() {}
    // letters = list(range(self.cells_count))
    // for _ in range(self.not_empty_cells_count):
    //     cell = random.choice(letters)
    //     self.not_empty_cells.append(cell)
    //     del letters[letters.index(cell)]

void RouletteDialog::spin_buttons() {}
    // self.current_time += self.spin_step
    // angle = -2 * math.pi / self.cells_count
    // width = self.size().width()
    // height = self.size().height()
    // for i, button in enumerate(self.roulette_buttons):
    //     button.move(int(self.r_h * math.cos(self.button_offset_angle + angle * i + self.spin_function_horisontal(i, self.current_time)) + (width  - self.button_size) / 2),
    //                 int(self.r_v * math.sin(self.button_offset_angle + angle * i + self.spin_function_vertical(i, self.current_time))   + (height - self.button_size) / 2))
    // if self.current_time >= self.spinning_duration:
    //     self.spinning_timer.stop()
    //     self.set_all_buttons_status(True)

void RouletteDialog::shoot(const int button_number) {}
    // if self.attempts_count < 0:
    //     return
    // self.attempts_count -= 1

    // if button_number not in self.not_empty_cells:
    //     self.close_button.setText(self.config.close_button_lose_text)
    //     self.roulette_buttons[button_number].set_lose_style()
    //     self.shot_sound.play()
    //     self.shot_hole_label = QHoleLabel(self.shot_hole_label_size, self)

    //     click_pos = QPoint(int(self.roulette_buttons[button_number].last_click_pos.x() + self.roulette_buttons[button_number].pos().x()),
    //                        int(self.roulette_buttons[button_number].last_click_pos.y() + self.roulette_buttons[button_number].pos().y()))

    //     self.shot_hole_label.move(click_pos.x() - int(self.shot_hole_label.size / 2), click_pos.y() - int(self.shot_hole_label.size / 2))
    //     self.shot_hole_label.show()

    //     shot_hole_region = QRegion(QRect(click_pos.x() - int(self.shot_hole_label.size / 8),
    //                                      click_pos.y() - int(self.shot_hole_label.size / 8),
    //                                      int(self.shot_hole_label.size / 4), int(self.shot_hole_label.size / 4)), QRegion.Ellipse)

    //     self.mask = self.mask.subtracted(shot_hole_region)
    //     self.setMask(self.mask)
    // else:
    //     self.close_button.setText(self.config.close_button_win_text)
    //     for cell in self.not_empty_cells:
    //         self.roulette_buttons[cell].set_win_style()
    //     self.roulette_buttons[button_number].set_win_style(True)
    //     self.attempts_count = -1
    //     self.set_all_buttons_status(False)
    //     self.empty_sound.play()

    // self.roulette_buttons[button_number].setEnabled(False)

    // if self.attempts_count == 0:
    //     self.set_all_buttons_status(False)
    //     for cell in self.not_empty_cells:
    //         self.roulette_buttons[cell].set_win_style()
    //     self.attempts_count -= 1
    //     return

double RouletteDialog::spin_function_horisontal(const int i, const int t) const {}
    // expression = eval(self.config.spin_function_horisontal)
    // return expression

double RouletteDialog::spin_function_vertical(const int i, const int t) const {}
    // expression = eval(self.config.spin_function_vertical)
    // return expression

void RouletteDialog::set_buttons_enabled(const bool status) {}
    // for button in self.roulette_buttons:
    //     button.setEnabled(status)

void RouletteDialog::close() {}
    // super().close()
    // self.spinning_sound.stop()
    // self.spinning_timer.stop()



// void RouletteDialog::mousePressEvent(, click_pos) {}
//     super().mousePressEvent(click_pos)
//     if (self.size().width() / 2 - click_pos.x()) ** 2 + (self.size().height() / 2 - click_pos.y()) ** 2 <= (self.close_button.size().width() / 2) ** 2:
//         self.close()
//     for i, button in enumerate(self.roulette_buttons):
//         if (click_pos.x() - (button.pos().x() + self.button_size / 2)) ** 2 +
//            + (click_pos.y() - (button.pos().y() + self.button_size / 2)) ** 2 <= (self.button_size / 2) ** 2 and button.isEnabled():
//             button.last_click_pos = QPoint(abs(button.pos().x() - click_pos.x()), abs(button.pos().y() - click_pos.y()))
//             self.shoot(i)
//             return
