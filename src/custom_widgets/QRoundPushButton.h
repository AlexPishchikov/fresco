#include <QPoint>
#include <QPushButton>


class QRoundPushButton : public QPushButton {
    public:
        QRoundPushButton(const int size, QWidget *parent = nullptr);

        QPoint last_click_pos;

        void set_win_style(const bool is_pressed = false);

        void set_lose_style();
    private:
        // void mousePressEvent(QEvent click_pos);
};
