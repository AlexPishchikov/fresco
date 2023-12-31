#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QRect>
#include <QRegion>
#include <QSize>

#include "QRoundPushButton.h"


QRoundPushButton::QRoundPushButton(const int size, QWidget *parent) : QPushButton(parent) {
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedSize(QSize(size, size));

    this->setMask(QRegion(QRect(0, 0, size, size), QRegion::Ellipse));
    this->setStyleSheet(QString("border-radius: %1px; border: 3px solid black;").arg(size / 2));

    this->last_click_pos = QPoint(size / 2, size / 2);
}

QPoint QRoundPushButton::get_last_click_pos() const {
    return this->last_click_pos;
}

void QRoundPushButton::set_lose_style() {
    this->setStyleSheet(QString("background: rgb(255, 0, 0); border-radius: %1px; border: 3px solid black;").arg(this->width() / 2));
}

void QRoundPushButton::set_win_style(const bool is_pressed) {
    this->setStyleSheet(QString(":disabled {background: rgb(0, %1, 0); color: %2; border-radius: %3px; border: 3px solid black;}").arg(is_pressed ? 255 : 150)
                                                                                                                                  .arg(is_pressed ? "black" : "white")
                                                                                                                                  .arg(this->width() / 2));
}

void QRoundPushButton::mousePressEvent(QMouseEvent* event) {
    QPushButton::mousePressEvent(event);

    if (this->isEnabled()) {
        this->last_click_pos = event->pos();
    }
}
