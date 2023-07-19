#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

#include <QDialog>
#include <QFont>
#include <QFontDatabase>
#include <QKeySequence>
#include <QRect>
#include <QRegion>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "../load_config/load_config.h"
#include "../custom_widgets/QHoleLabel.h"
#include "../custom_widgets/QRoundPushButton.h"
#include "RouletteDialog.h"


RouletteDialog::RouletteDialog(const int total, const int win, const int attempts, QWidget *parent) : QDialog(parent) {
    this->config = load_config("res/default_config/RouletteDialogConfig.json");

    this->setWindowFlag(Qt::FramelessWindowHint);

    const int button_size = this->config["roulette_button_size"].toInt();
    const int button_margin = this->config["roulette_button_margin"].toInt();

    const int r = 3 * (button_size + 2 * button_margin) / total + (button_size + button_margin) / (2 * std::sin(M_PI / total));
    const int window_size = 2 * (r + button_margin) + button_size;

    this->setFixedSize(window_size, window_size);

    this->setStyleSheet(QString("border-radius: %1px; border: 2px solid black;").arg(window_size / 2));
    this->setMask(QRegion(this->rect(), QRegion::Ellipse));

    this->place_buttons(r, total, button_size, window_size);
    this->set_buttons_labels();
    // this->set_buttons_enabled(false);

    // this->spin_buttons();


    // self.shot_sound = QSound(self.config.roulette_shot_sound_file_path)
    // self.spinning_sound = QSound(self.config.roulette_spinning_sound_file_path)
    // self.empty_sound = QSound(self.config.roulette_empty_sound_file_path)

    // self.shot_hole_label_size = self.config.shot_hole_label_size

    // self.selected_cell = 0
    // self.not_empty_cells = list()
    // self.roulette_buttons = list()

    // self.spin_step = self.config.spin_step
    // self.current_time = 0
    // self.spinning_duration = self.get_spinning_duration()
    // self.spinning_timer = QTimer(self)
    // self.spinning_timer.timeout.connect(self.buttons_spinning)
    // self.spinning_timer.setInterval(self.spin_step)
}

void RouletteDialog::showEvent() {
    // super().show()
    // self.spinning_sound.play()
    // self.spinning_timer.start()
}

void RouletteDialog::get_sound_duration(const QString& file_path) const {}
    // spinning_sound_file = self.spinning_sound.fileName()
    // with contextlib.closing(wave.open(spinning_sound_file, 'r')) as file:
    //     return round(file.getnframes() / float(file.getframerate()) * 1000)

void RouletteDialog::place_buttons(const int r, const int total, const int button_size, const int window_size) {
    const double angle = 2 * M_PI / total;
    for (int i = 0; i < total; i++) {
        const double x = r * std::cos(angle * i) + (window_size - button_size) / 2;
        const double y = r * std::sin(angle * i) + (window_size - button_size) / 2;

        QRoundPushButton* button = new QRoundPushButton(button_size, this);
        connect(button, &QRoundPushButton::clicked, this, [=]{this->shoot(i);});
        button->move(x, y);

        this->buttons.push_back(button);
    }

    QRoundPushButton* close_button = new QRoundPushButton(button_size * 2, this);
    connect(close_button, &QRoundPushButton::clicked, this, [=]{this->close();});

    close_button->move(window_size / 2 - button_size, window_size / 2 - button_size);

    const QString lobster_font = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":lobster_font"))[0];
    close_button->setFont(QFont(lobster_font, this->config["roulette_close_button_font_size"].toInt()));
    close_button->setText(this->config["roulette_close_button_init_text"].toString());
}

void RouletteDialog::set_buttons_labels() {
    QString letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::shuffle(letters.begin(), letters.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    for (int i = 0; i < this->buttons.size(); i++) {
        this->buttons[i]->setText(letters[i]);
        this->buttons[i]->setShortcut(QKeySequence(letters[i]));
        this->buttons[i]->setFont(QFont("Comic Sans MS", this->config["roulette_roulette_buttons_font_size"].toInt()));
    }
}

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

void RouletteDialog::shoot(const int button_number) {
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
}

double RouletteDialog::spin_function_horisontal(const int i, const int t) const {}
    // expression = eval(self.config.spin_function_horisontal)
    // return expression

double RouletteDialog::spin_function_vertical(const int i, const int t) const {}
    // expression = eval(self.config.spin_function_vertical)
    // return expression

void RouletteDialog::set_buttons_enabled(const bool status) {
    for (QRoundPushButton* button : buttons) {
        button->setEnabled(status);
    }
}

// void RouletteDialog::close() {}
    // super().close()
    // self.spinning_sound.stop()
    // self.spinning_timer.stop()



// void RouletteDialog::mousePressEvent(click_pos) {}
//     super().mousePressEvent(click_pos)
//     if (self.size().width() / 2 - click_pos.x()) ** 2 + (self.size().height() / 2 - click_pos.y()) ** 2 <= (self.close_button.size().width() / 2) ** 2:
//         self.close()
//     for i, button in enumerate(self.roulette_buttons):
//         if (click_pos.x() - (button.pos().x() + self.button_size / 2)) ** 2 +
//            + (click_pos.y() - (button.pos().y() + self.button_size / 2)) ** 2 <= (self.button_size / 2) ** 2 and button.isEnabled():
//             button.last_click_pos = QPoint(abs(button.pos().x() - click_pos.x()), abs(button.pos().y() - click_pos.y()))
//             self.shoot(i)
//             return
