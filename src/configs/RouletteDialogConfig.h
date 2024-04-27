#pragma once

#include <QString>

#include "Config.h"


class RouletteDialogConfig : private Config {
    public:
        RouletteDialogConfig();

        QString close_button_init_text;

        QString close_button_lose_text;

        QString close_button_win_text;

        QString spin_function;

        unsigned int button_margin;

        unsigned int button_size;

        unsigned int buttons_font_size;

        unsigned int close_button_font_size;

        unsigned int shot_hole_label_size;

        unsigned int spin_step;
};
