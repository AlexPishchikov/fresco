#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

#include <QCloseEvent>
#include <QDialog>
#include <QFont>
#include <QFontDatabase>
#include <QKeySequence>
// #include <QMediaPlayer>
#include <QRect>
#include <QRegion>
#include <QShowEvent>
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

    this->spin_duration = this->get_sound_duration(":spin_sound");

    connect(&this->timer, &QTimer::timeout, this, [=]{this->spin_buttons(r, total, button_size, window_size);});

    // qDebug() << this->get_sound_duration(":spin_sound");
    this->set_buttons_enabled(false);

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


int RouletteDialog::get_sound_duration(const QString& file_path) const {
    // QMediaPlayer* player = new QMediaPlayer;
    // player->setSource(file_path);
    // return player->duration();
    return 1000;
}

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

void RouletteDialog::spin_buttons(const int r, const int total, const int button_size, const int window_size) {
    if (this->spin_duration <= 0) {
        this->timer.stop();
        this->set_buttons_enabled(true);
        return;
    }

    this->spin_duration -= this->timer.interval();

    const double angle = 2 * M_PI / total;
    for (int i = 0; i < this->buttons.size(); i++) {
        const double x = r * std::cos(angle * i + this->spin_offset(i, this->spin_duration)) + (window_size - button_size) / 2;
        const double y = r * std::sin(angle * i + this->spin_offset(i, this->spin_duration)) + (window_size - button_size) / 2;
        this->buttons[i]->move(x, y);
    }
}

void RouletteDialog::shoot(const int button_number) {
    qDebug() << button_number;
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

double RouletteDialog::spin_offset(const int i, const int t) const {
    return 1.0 / (22.0 * powf(M_E, -powf(t, 2.0) / powf(2.0, 18.0)));
    // return t / 400.0;
}

void RouletteDialog::set_buttons_enabled(const bool status) {
    for (QRoundPushButton* button : buttons) {
        button->setEnabled(status);
    }
}

void RouletteDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);
    this->timer.start(this->config["roulette_spin_step"].toInt());
}

void RouletteDialog::closeEvent(QCloseEvent* event) {
    QDialog::closeEvent(event);
    this->timer.stop();
    // self.spinning_sound.stop()
}


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
