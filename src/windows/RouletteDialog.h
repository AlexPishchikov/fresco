#pragma once

#include <QCloseEvent>
#include <QDialog>
#include <QJsonObject>
#include <QShowEvent>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "../custom_widgets/QRoundPushButton.h"


class RouletteDialog : public QDialog {
    public:
        RouletteDialog(const int total, const int win, const int attempts, QWidget *parent = nullptr);

        // void close();
    private:
        int spin_duration;

        QJsonObject config;

        QTimer timer;

        QVector<QRoundPushButton*> buttons;

        int get_sound_duration(const QString& file_path) const;

        void place_buttons(const int r, const int total, const int button_size, const int window_size);

        void set_buttons_labels();

        void spin_buttons(const int r, const int total, const int button_size, const int window_size);

        void shoot(const int button_number);

        double spin_offset(const int i, const int t) const;

        void set_buttons_enabled(const bool status);

        void showEvent(QShowEvent* event);

        void closeEvent(QCloseEvent* event);

        // void mousePressEvent(click_pos);
};
