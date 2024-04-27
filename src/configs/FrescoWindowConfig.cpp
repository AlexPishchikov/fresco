#include <QJsonObject>

#include "Config.h"
#include "FrescoWindowConfig.h"


FrescoWindowConfig::FrescoWindowConfig() : Config() {
    const QJsonObject json_config = this->load_config(":fresco_window_config_default");

    this->calculate_time_function = json_config["fresco_calculate_time_function"].toString();

    this->clear_button_text = json_config["fresco_clear_button_text"].toString();

    this->generate_riddle_button_text = json_config["fresco_generate_riddle_button_text"].toString();

    this->name_combo_box_placeholder = json_config["fresco_name_combo_box_placeholder"].toString();

    this->name_icon_color = json_config["fresco_name_icon_color"].toString();

    this->question_placeholder = json_config["fresco_question_placeholder"].toString();

    this->refresh_questions_button_text = json_config["fresco_refresh_questions_button_text"].toString();

    this->roulette_button_text = json_config["fresco_roulette_button_text"].toString();

    this->select_questions_file_button_text = json_config["fresco_select_questions_file_button_text"].toString();

    this->start_timer_button_text = json_config["fresco_start_timer_button_text"].toString();

    this->stop_timer_button_text = json_config["fresco_stop_timer_button_text"].toString();

    this->time_over_text = json_config["fresco_time_over_text"].toString();

    this->window_title = json_config["fresco_window_title"].toString();

    this->attempts_count_init_value = std::max(1, std::min(json_config["fresco_win_cells_count_init_value"].toInt(),
                                                           json_config["fresco_attempts_count_init_value"].toInt()));
    this->cells_count_init_value = std::max(3, std::min(26, json_config["fresco_cells_count_init_value"].toInt()));

    this->const_lower_label_font_size = json_config["fresco_const_lower_label_font_size"].toInt();

    this->const_upper_label_font_size = json_config["fresco_const_upper_label_font_size"].toInt();

    this->custom_student_rating = json_config["fresco_custom_student_rating"].toInt();

    this->question_label_font_size = json_config["fresco_question_label_font_size"].toInt();

    this->remaining_time_label_font_size = json_config["fresco_remaining_time_label_font_size"].toInt();

    this->time_interval = std::max(1, std::min(1000, json_config["fresco_time_interval"].toInt()));

    this->total_time_label_font_size = json_config["fresco_total_time_label_font_size"].toInt();

    this->win_cells_count_init_value = std::max(0, std::min(json_config["fresco_cells_count_init_value"].toInt(),
                                                            json_config["fresco_win_cells_count_init_value"].toInt()));
}
