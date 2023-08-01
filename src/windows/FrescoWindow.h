#pragma once

#include <QHash>
#include <QJsonObject>
#include <QMainWindow>
#include <QPointer>
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
        bool evil_style;

        int spin_sound_duration;

        QHash<QString, int> time_by_name;

        QJsonObject config;

        QString questions_file_path;

        QStringList questions_list;

        QTimer timer;

        QPointer<RouletteDialog> roulette;

        Theme current_theme;

        Ui::FrescoWindowUi ui;

        bool is_name(const QString &name) const;

        QString last_letter(const int time) const;

        unsigned int calculate_time(const int rating) const;

        void add_student_to_combo_box(const QString &name, const int rating);

        void cells_count_spin_box_changed();

        void clear();

        void create_connections();

        void generate_riddle();

        void import_questions_from_TeX();

        void init_remaining_time_label();

        void init_sound();

        void init_ui();

        void parse_csv(const QString &file_name, const QString &rating_col_name);

        void set_time_label();

        void set_question_label();

        void select_questons_file();

        void set_evil_style();

        void set_good_style();

        void set_theme(const Theme theme);

        void show_roulette_dialog();

        void start_timer();

        void update_generate_button_status();

        void update_remaining_time_label();

        void win_cells_count_spin_box_changed();
};
