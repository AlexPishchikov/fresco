#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>
#include <QString>
#include <QUrl>


class DownloadWorker : public QObject {
    Q_OBJECT
    signals:
        void finished();
    public:
        int get_status_code() const;

        QString get_file_path() const;

        void download(const QUrl &load_table_url, const QString &save_path);
    private:
        QNetworkAccessManager manager;

        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;

        QString file_path;

        void download_finished(const QString &save_path);
};
