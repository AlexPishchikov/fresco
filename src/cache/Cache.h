#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QString>

#include "ContentType.h"


class Cache {
    public:
        Cache();

        Cache(const QString &cache_dir_path);

        QJsonArray get_data(const QList<ContentType> &types_list) const;

        void add(const QJsonObject &current_object) const;
    private:
        QString cache_file_path;
};
