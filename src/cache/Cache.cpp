#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <QStringList>

#include "Cache.h"
#include "ContentType.h"


Cache::Cache() {}

Cache::Cache(const QString &cache_dir_path) {
    this->cache_file_path = QString("%1.tables_list.json").arg(cache_dir_path);
    if (!QFileInfo::exists(cache_dir_path)) {
        QDir().mkdir(cache_dir_path);
    }

    if (!QFileInfo::exists(this->cache_file_path)) {
        QFile cache_file(this->cache_file_path);
        cache_file.open(QFile::WriteOnly);
    }
}

QJsonArray Cache::get_data(const QList<ContentType> &types_list) const {
    QFile cache_file(this->cache_file_path);
    cache_file.open(QFile::ReadOnly);
    const QJsonArray cache_data = QJsonDocument::fromJson(cache_file.readAll()).array();

    QStringList types;
    if (types_list.contains(ContentType::Url)) {
        types.push_back("url");
    }
    if (types_list.contains(ContentType::TableName)) {
        types.push_back("table_name");
    }
    if (types_list.contains(ContentType::ColumnNames)) {
        types.push_back("column_names");
    }

    QJsonArray data;
    for (const QJsonValueConstRef &object : cache_data) {
        QJsonObject temp_json_object;
        for (const QString &type : types) {
            temp_json_object.insert(type, object[type]);
        }
        data.push_back(temp_json_object);
    }

    return data;
}

void Cache::add(const QJsonObject &current_object) const {
    bool is_new_object_cached = false;
    QJsonArray new_cache;
    QJsonObject new_object;
    new_object.insert("url", current_object[QString("url")]);
    new_object.insert("table_name", current_object[QString("table_name")]);

    const QJsonArray cache_data = this->get_data({ContentType::Url, ContentType::TableName, ContentType::ColumnNames});

    for (const QJsonValueConstRef &object : cache_data) {
        if (object[QString("url")] != current_object[QString("url")]) {
            new_cache.push_back(object);
        }
        else {
            is_new_object_cached = true;
            QJsonArray column_names = object[QString("column_names")].toArray();
            if (!column_names.contains(current_object[QString("column_names")])) {
                column_names.push_back(current_object[QString("column_names")]);
            }

            new_object.insert("column_names", column_names);
            new_cache.push_back(new_object);
        }
    }

    if (!is_new_object_cached) {
        const QJsonArray column_names = QJsonArray({ current_object[QString("column_names")] });
        new_object.insert("column_names", column_names);
        new_cache.push_back(new_object);
    }

    QFile cache_file(this->cache_file_path);
    cache_file.open(QFile::WriteOnly);
    cache_file.write(QJsonDocument(new_cache).toJson());
}
