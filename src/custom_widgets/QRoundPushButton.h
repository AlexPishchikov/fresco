#pragma once

#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>


class QRoundPushButton : public QPushButton {
    public:
        QRoundPushButton(const int size, QWidget *parent = nullptr);

        QPoint get_last_click_pos() const;

        void set_lose_style();

        void set_win_style(const bool is_pressed = false);
    private:
        QPoint last_click_pos;

        void mousePressEvent(QMouseEvent* event);
};
