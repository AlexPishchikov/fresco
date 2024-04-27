#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "Config.h"


QJsonObject Config::load_config(const QString &default_config_path) const {
    QFile default_config_file(default_config_path);
    default_config_file.open(QFile::ReadOnly);

    QJsonObject config = QJsonDocument::fromJson(default_config_file.readAll()).object();

    if (QFileInfo::exists("config.json")) {
        QFile custom_config_file("config.json");
        custom_config_file.open(QFile::ReadOnly);

        QJsonObject custom_config = QJsonDocument::fromJson(custom_config_file.readAll()).object();

        const QStringList keys = custom_config.keys();

        for (const QString &key : keys) {
            if (custom_config[key].type() == config[key].type()) {
                config.insert(key, custom_config[key]);
            }
        }
    }

    return config;
}
