#include <QDialog>
#include <QString>
#include <QWidget>


class RouletteDialog : public QDialog {
    public:
        RouletteDialog(const int total, const int win, const int attempts, QWidget *parent = nullptr);

        void show();
    private:
        void get_sound_duration(const QString& file_path) const;

        void place_buttons();

        void loading();

        void spin_buttons();

        void shoot(const int button_number);

        double spin_function_horisontal(const int i, const int t) const;

        double spin_function_vertical(const int i, const int t) const;

        void set_buttons_enabled(const bool status);

        void close();

        // void mousePressEvent(click_pos);
};
