#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>

class DownloadWorker : public QObject {
    Q_OBJECT
    signals:
        void finished();
    public:
        DownloadWorker();
        void download(const QString &load_table_url);
    private:
        void download_finished();
        QNetworkAccessManager manager;
        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;
};
