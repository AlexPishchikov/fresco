#include <QPoint>
#include <QPushButton>

#include "QRoundPushButton.h"


QRoundPushButton::QRoundPushButton (const int size, QWidget *parent) : QPushButton(parent) {
    // QPushButton.__init__(self, parent)
    // self.button_size = size
    // self.setMask(QRegion(QRect(0, 0, self.button_size, self.button_size), QRegion.Ellipse))
    // self.setStyleSheet(f'background: rgb(190, 190, 190); border-radius: {self.button_size / 2}px; border: 2px solid black;')
    // self.setFocusPolicy(Qt.NoFocus)
    // self.last_click_pos = QPoint(int(self.button_size / 2), int(self.button_size / 2))
}

void set_win_style(const bool is_pressed) {}
//     self.setStyleSheet(f':disabled {{background: rgb(0, {255 if is_pressed else 150}, 0);'
//                        f'color: {"black" if is_pressed else "white"};'
//                        f'border-radius: {self.button_size / 2}px;'
//                        f'border: 2px solid black;}}')

void set_lose_style() {}
//     self.setStyleSheet(f'background: rgb(255, 0, 0); border-radius: {self.button_size / 2}px; border: 2px solid black;')

// def mousePressEvent(self, click_pos):
//     super().mousePressEvent(click_pos)
//     if self.isEnabled:
//         self.last_click_pos = click_pos
