#include <QChar>
#include <QJsonObject>
#include <QMainWindow>
#include <QString>

#include "../../build/ui/ui_fresco.h"


class FrescoWindow : public QMainWindow {
    public:
        FrescoWindow(const QString &data_file_name, const QString &rating_col_name, const QString &current_theme, QWidget *parent = nullptr);
    private:
        Ui::FrescoWindowUi ui;

        QJsonObject config;

        QString current_theme;

        void parse_csv(const QString &file_name, const QString &rating_col_name);

        void add_student_to_combo_box(const QString &name, const double rating);

        void set_time_label();

        void set_question_label(const QString &question);

        void init_remaining_time_label();

        void generate_riddle();

        void select_questons_file();

        void import_questions_from_TeX(const QString &file_path);

        void set_evil_style();

        void start_timer();

        void update_remaining_time_label();

        void cells_count_spin_box_changed();

        void show_roulette_dialog();

        void closeEvent();
        // void closeEvent(event);

        void clear();

        void switch_theme(const QString &theme);

        bool is_name(const QString &name) const;

        double calculate_time(const double rating) const;

        QChar last_letter(const double rating) const;
};
