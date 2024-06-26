#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QString>
#include <QUrl>

#include "DownloadWorker.h"


int DownloadWorker::get_status_code() const {
    return this->reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

QString DownloadWorker::get_file_path() const {
    return this->file_path;
}

void DownloadWorker::download(const QUrl &load_table_url, const QString &save_path) {
    this->file_path = "";

    this->manager.setTransferTimeout(10000);

    QNetworkRequest request = QNetworkRequest(load_table_url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    this->reply.reset(this->manager.get(request));

    connect(this->reply.get(), &QNetworkReply::finished, this, [=]{DownloadWorker::download_finished(save_path);});
    this->reply.get();
}

void DownloadWorker::download_finished(const QString &save_path) {
    if (this->reply->error() != QNetworkReply::NoError) {
        emit finished();
        return;
    }

    if (this->reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
        QStringList data = QString(this->reply->readAll()).split(',');
        const QRegularExpression regex("\"[^\"]+\"");
        for (int i = 0; i < data.size(); i++) {
            if (regex.match(data[i]).hasMatch()) {
                data[i] = data[i].replace('\n', ' ');
            }
        }

        const QString filename = QUrl::fromPercentEncoding(this->reply->header(QNetworkRequest::ContentDispositionHeader).toByteArray());
        this->file_path = QString("%1%2").arg(save_path, filename.split("filename*=UTF-8''").last());
        QFile table(this->file_path);
        table.open(QFile::WriteOnly);
        table.write(data.join(',').toUtf8());
    }

    emit finished();
}
