#pragma once

#include <QJsonObject>
#include <QString>


class Config {
    protected:
        QJsonObject load_config(const QString &default_config_path) const;
};
