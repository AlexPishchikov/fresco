#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "load_config.h"

QJsonObject load_config(const QString &default_config_path) {
    QFile default_config(default_config_path);
    default_config.open(QFile::ReadOnly);

    QJsonObject config = QJsonDocument::fromJson(default_config.readAll()).object();

    QFile custom_config_file("config.json");
    if (custom_config_file.exists()) {
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
