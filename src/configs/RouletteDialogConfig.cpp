#include <QJsonObject>

#include "Config.h"
#include "RouletteDialogConfig.h"


RouletteDialogConfig::RouletteDialogConfig() : Config() {
    const QJsonObject json_config = this->load_config(":roulette_dialog_config_default");

    this->close_button_init_text = json_config["roulette_close_button_init_text"].toString();

    this->close_button_lose_text = json_config["roulette_close_button_lose_text"].toString();

    this->close_button_win_text = json_config["roulette_close_button_win_text"].toString();

    this->spin_function = json_config["roulette_spin_function"].toString();

    this->button_margin = json_config["roulette_button_margin"].toInt();

    this->button_size = json_config["roulette_button_size"].toInt();

    this->buttons_font_size = json_config["roulette_buttons_font_size"].toInt();

    this->close_button_font_size = json_config["roulette_close_button_font_size"].toInt();

    this->shot_hole_label_size = json_config["roulette_shot_hole_label_size"].toInt();

    this->spin_step = std::max(1, std::min(1000, json_config["roulette_spin_step"].toInt()));
}
