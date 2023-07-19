#pragma once

#include <QDialog>
#include <QJsonObject>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "../custom_widgets/QRoundPushButton.h"


class RouletteDialog : public QDialog {
    public:
        RouletteDialog(const int total, const int win, const int attempts, QWidget *parent = nullptr);

        void showEvent();

        // void close();
    private:
        QJsonObject config;

        QTimer timer;

        QVector<QRoundPushButton*> buttons;

        void get_sound_duration(const QString& file_path) const;

        void place_buttons(const int r, const int total, const int button_size, const int window_size);

        void set_buttons_labels();

        void spin_buttons();

        void shoot(const int button_number);

        double spin_function_horisontal(const int i, const int t) const;

        double spin_function_vertical(const int i, const int t) const;

        void set_buttons_enabled(const bool status);

        // void mousePressEvent(click_pos);
};
