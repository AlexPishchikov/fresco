#pragma once

#include <QString>

#include "Config.h"


class FrescoWindowConfig : private Config {
    public:
        FrescoWindowConfig();

        QString calculate_time_function;

        QString clear_button_text;

        QString generate_riddle_button_text;

        QString name_combo_box_placeholder;

        QString name_icon_color;

        QString question_placeholder;

        QString refresh_questions_button_text;

        QString roulette_button_text;

        QString select_questions_file_button_text;

        QString start_timer_button_text;

        QString stop_timer_button_text;

        QString time_over_text;

        QString window_title;

        unsigned int attempts_count_init_value;

        unsigned int cells_count_init_value;

        unsigned int const_lower_label_font_size;

        unsigned int const_upper_label_font_size;

        unsigned int custom_student_rating;

        unsigned int question_label_font_size;

        unsigned int remaining_time_label_font_size;

        unsigned int time_interval;

        unsigned int total_time_label_font_size;

        unsigned int win_cells_count_init_value;
};
