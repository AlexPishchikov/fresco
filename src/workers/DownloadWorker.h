#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>
#include <QString>


class DownloadWorker : public QObject {
    Q_OBJECT
    signals:
        void finished();
    public:
        void download(const QString &load_table_url, const QString &save_path);
        QString get_filename() const;
    private:
        QString filename = "";

        QNetworkAccessManager manager;

        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;

        void download_finished(const QString &save_path);
};
