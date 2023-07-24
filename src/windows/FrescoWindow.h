#include <QHash>
#include <QJsonObject>
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QTimer>

#include "../../build/ui/ui_fresco.h"
#include "../enums.h"
#include "RouletteDialog.h"


class FrescoWindow : public QMainWindow {
    public:
        FrescoWindow(const QString &data_file_name, const QString &rating_col_name, const Theme current_theme, QWidget *parent = nullptr);
    private:
        QHash<QString, int> time_by_name;

        QJsonObject config;

        QString questions_file_path;

        QStringList questions_list;

        QTimer timer;

        RouletteDialog* roulette;

        Theme current_theme;

        Ui::FrescoWindowUi ui;

        void init_ui();

        void create_connections();

        void parse_csv(const QString &file_name, const QString &rating_col_name);

        void add_student_to_combo_box(const QString &name, const int rating);

        void set_time_label();

        void set_question_label();

        void init_remaining_time_label();

        void generate_riddle();

        void select_questons_file();

        void import_questions_from_TeX();

        void set_evil_style();

        void start_timer();

        void update_remaining_time_label();

        void cells_count_spin_box_changed();

        void win_cells_count_spin_box_changed();

        void show_roulette_dialog();

        void clear();

        void set_theme(const Theme theme);

        bool is_name(const QString &name) const;

        unsigned int calculate_time(const int rating) const;

        QString last_letter(const int time) const;
};
