#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QRunnable>
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>

class DownloadWorker : public QObject {
    public:
        DownloadWorker();
        int download(const QString &load_table_url);
        void download_finished() const;
        void download_read() const;
    private:
        QNetworkAccessManager manager;
        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;
};
