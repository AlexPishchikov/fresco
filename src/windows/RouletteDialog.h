#pragma once

#include <QCloseEvent>
#include <QDialog>
#include <QJsonObject>
#include <QSoundEffect>
#include <QMouseEvent>
#include <QList>
#include <QPointer>
#include <QShowEvent>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "../configs/RouletteDialogConfig.h"
#include "../custom_widgets/QRoundPushButton.h"


class RouletteDialog : public QDialog {
    public:
        RouletteDialog(const int spin_sound_duration, const int total, const int win, const int attempts, QWidget *parent = nullptr);
    private:
        int attempts;

        int current_spin_duration;

        int total_spin_duration;

        QList<int> win_buttons;

        QList<QPointer<QRoundPushButton>> buttons;

        QPointer<QSoundEffect> spin_sound;

        QPointer<QSoundEffect> shot_sound;

        QPointer<QSoundEffect> empty_sound;

        QPointer<QRoundPushButton> close_button;

        QTimer timer;

        RouletteDialogConfig config;

        void closeEvent(QCloseEvent* event);

        void mousePressEvent(QMouseEvent* event);

        void showEvent(QShowEvent* event);

        void init_sounds(const int spin_sound_duration);

        void place_buttons(const int r, const int total, const int window_size);

        void set_buttons_enabled(const bool status);

        void set_buttons_labels();

        void shoot(const int button_number);

        void spin_buttons(const int r, const int total, const int window_size);

        void stop_sounds();
};
