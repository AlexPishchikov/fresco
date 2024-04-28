#include <QJsonObject>

#include "../Theme.h"
#include "Config.h"
#include "DownloadDialogConfig.h"


DownloadDialogConfig::DownloadDialogConfig() : Config() {
    const QJsonObject json_config = this->load_config(":download_dialog_config_default");

    this->cache_folder_name = json_config["download_dialog_folder_name"].toString();

    this->rating_column_name = json_config["download_dialog_rating_column_name"].toString();

    this->window_title = json_config["download_dialog_window_title"].toString();

    this->start_theme = static_cast<Theme>(json_config["download_dialog_start_theme"].toInt());
}
