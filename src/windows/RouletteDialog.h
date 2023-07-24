#pragma once

#include <QCloseEvent>
#include <QDialog>
#include <QJsonObject>
#include <QSoundEffect>
#include <QMouseEvent>
#include <QList>
#include <QShowEvent>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "../custom_widgets/QRoundPushButton.h"


class RouletteDialog : public QDialog {
    public:
        RouletteDialog(const int spin_sound_duration, const int total, const int win, const int attempts, QWidget *parent = nullptr);
    private:
        int current_spin_duration;

        int total_spin_duration;

        int attempts;

        QJsonObject config;

        QList<int> win_buttons;

        QList<QRoundPushButton*> buttons;

        QSoundEffect* spin_sound;

        QSoundEffect* shot_sound;

        QSoundEffect* empty_sound;

        QRoundPushButton* close_button;

        QTimer timer;

        void init_sounds(const int spin_sound_duration);

        void stop_sounds();

        void place_buttons(const int r, const int total, const int button_size, const int window_size);

        void set_buttons_labels();

        void spin_buttons(const int r, const int total, const int button_size, const int window_size);

        void shoot(const int button_number);

        void set_buttons_enabled(const bool status);

        void showEvent(QShowEvent* event);

        void closeEvent(QCloseEvent* event);

        void mousePressEvent(QMouseEvent* event);
};
