#include <QLabel>
#include <QRect>
#include <QRegion>
#include <QPixmap>

#include "QHoleLabel.h"


QHoleLabel::QHoleLabel(const int size, QWidget *parent) : QLabel(parent) {
    QRegion mask(QRect(0, 0, size, size), QRegion::Ellipse);
    this->setMask(mask);
    this->setStyleSheet("background-color: transparent; border-color: transparent;");
    this->setFixedWidth(size);
    this->setFixedHeight(size);
    this->setPixmap(QPixmap(":shot_hole_image").scaled(size, size));
}
