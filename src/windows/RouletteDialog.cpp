#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

#include <QAudioOutput>
#include <QCloseEvent>
#include <QDialog>
#include <QFont>
#include <QFontDatabase>
#include <QKeySequence>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QRect>
#include <QRegion>
#include <QShowEvent>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QWidget>

#include "../load_config/load_config.h"
#include "../custom_widgets/QHoleLabel.h"
#include "../custom_widgets/QRoundPushButton.h"
#include "RouletteDialog.h"


RouletteDialog::RouletteDialog(const int total, const int win, const int attempts, QWidget *parent) : QDialog(parent) {
    this->attempts = attempts;

    this->config = load_config("res/default_config/RouletteDialogConfig.json");

    this->setWindowFlag(Qt::FramelessWindowHint);

    this->init_sounds();

    const int button_size = this->config["roulette_button_size"].toInt();
    const int button_margin = this->config["roulette_button_margin"].toInt();

    const int r = 3 * (button_size + 2 * button_margin) / total + (button_size + button_margin) / (2 * std::sin(M_PI / total));
    const int window_size = 2 * (r + button_margin) + button_size;

    this->setFixedSize(window_size, window_size);

    this->setStyleSheet(QString("border-radius: %1px; border: 2px solid black;").arg(window_size / 2));
    this->setMask(QRegion(this->rect(), QRegion::Ellipse));

    this->place_buttons(r, total, button_size, window_size);
    this->set_buttons_labels();

    connect(&this->timer, &QTimer::timeout, this, [=]{this->spin_buttons(r, total, button_size, window_size);});

    this->set_buttons_enabled(false);

    for (int i = 0; i < total; i++) {
        this->win_buttons.push_back(i);
    }
    std::shuffle(this->win_buttons.begin(), this->win_buttons.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    this->win_buttons = this->win_buttons.mid(total - win);
}

void RouletteDialog::init_sounds() {
    this->spin_sound = new QSoundEffect(this);
    this->shot_sound = new QSoundEffect(this);
    this->empty_sound = new QSoundEffect(this);

    this->spin_sound->setSource(QUrl("qrc:spin_sound"));
    this->shot_sound->setSource(QUrl("qrc:shot_sound"));
    this->empty_sound->setSource(QUrl("qrc:empty_sound"));

    QMediaPlayer* spin_player = new QMediaPlayer;
    spin_player->setSource(QUrl("qrc:spin_sound"));
    this->total_spin_duration = spin_player->duration();
    this->current_spin_duration = spin_player->duration();
}

void RouletteDialog::stop_sounds() {
    this->spin_sound->stop();
    this->shot_sound->stop();
    this->empty_sound->stop();
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

    this->close_button = new QRoundPushButton(button_size * 2, this);
    connect(this->close_button, &QRoundPushButton::clicked, this, [=]{this->close();});

    this->close_button->move(window_size / 2 - button_size, window_size / 2 - button_size);

    const QString lobster_font = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":lobster_font"))[0];
    this->close_button->setFont(QFont(lobster_font, this->config["roulette_close_button_font_size"].toInt()));
    this->close_button->setText(this->config["roulette_close_button_init_text"].toString());
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
    if (this->current_spin_duration <= 0) {
        this->timer.stop();
        this->set_buttons_enabled(true);
        return;
    }

    this->current_spin_duration -= this->timer.interval();

    const double angle = 2 * M_PI / total;
    for (int i = 0; i < this->buttons.size(); i++) {
        const double x = r * std::cos(angle * i + this->spin_offset(i, this->total_spin_duration - this->current_spin_duration)) + (window_size - button_size) / 2;
        const double y = r * std::sin(angle * i + this->spin_offset(i, this->total_spin_duration - this->current_spin_duration)) + (window_size - button_size) / 2;
        this->buttons[i]->move(x, y);
    }
}

void RouletteDialog::shoot(const int button_number) {
    if (this->attempts <= 0) {
        return;
    }
    this->attempts -= 1;

    this->stop_sounds();

    if (!this->win_buttons.contains(button_number)) {
        this->set_buttons_enabled(false);

        this->shot_sound->play();

        this->attempts = 0;

        const int hole_size = this->config["roulette_shot_hole_label_size"].toInt();
        QHoleLabel* shot_hole = new QHoleLabel(hole_size, this);

        const QPoint hole_pos = this->buttons[button_number]->last_click_pos + QPoint(this->buttons[button_number]->x(), this->buttons[button_number]->y());
        shot_hole->move(hole_pos.x() - hole_size / 2, hole_pos.y() - hole_size / 2);
        shot_hole->show();

        const QRegion shot_hole_region = QRegion(QRect(hole_pos.x() - hole_size / 8,
                                                       hole_pos.y() - hole_size / 8,
                                                       hole_size / 4,
                                                       hole_size / 4),
                                                       QRegion::Ellipse);

        this->setMask(this->mask().subtracted(shot_hole_region));

        for (int i = 0; i < this->buttons.size(); i++) {
            if (this->win_buttons.contains(i)) {
                this->buttons[i]->set_win_style();
            }
        }
        this->buttons[button_number]->set_lose_style();

        this->close_button->setText(this->config["roulette_close_button_lose_text"].toString());
    }
    else {
        this->buttons[button_number]->setEnabled(false);

        this->empty_sound->play();

        this->buttons[button_number]->set_win_style(true);
        this->win_buttons.removeAll(button_number);

        if (this->attempts == 0) {
            this->set_buttons_enabled(false);
            this->close_button->setText(this->config["roulette_close_button_win_text"].toString());
            for (int i = 0; i < this->buttons.size(); i++) {
                if (this->win_buttons.contains(i)) {
                    this->buttons[i]->set_win_style();
                }
            }
        }
    }
}

double RouletteDialog::spin_offset(const int i, const int t) const {
    return 15.0 * powf(M_E, -powf(t, 2.0) / powf(2.0, 18.0));
    // return t / 400.0;
}

void RouletteDialog::set_buttons_enabled(const bool status) {
    for (QRoundPushButton* button : buttons) {
        button->setEnabled(status);
    }
}

void RouletteDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    this->spin_sound->play();
    this->timer.start(this->config["roulette_spin_step"].toInt());
}

void RouletteDialog::closeEvent(QCloseEvent* event) {
    QDialog::closeEvent(event);

    this->stop_sounds();
    this->timer.stop();
}

void RouletteDialog::mousePressEvent(QMouseEvent* event) {
    QDialog::mousePressEvent(event);

    if (pow(this->size().width() / 2 - event->pos().x(), 2) + pow(this->size().height() / 2 - event->pos().y(), 2) <= pow(this->close_button->size().width() / 2, 2)) {
        this->close();
    }
}
