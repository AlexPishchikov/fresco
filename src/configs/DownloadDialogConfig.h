#pragma once

#include <QString>

#include "../enums.h"
#include "Config.h"


class DownloadDialogConfig : private Config {
    public:
        DownloadDialogConfig();

        QString cache_folder_name;

        QString rating_column_name;

        QString window_title;

        Theme start_theme;
};
