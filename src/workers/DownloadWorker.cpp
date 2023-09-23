#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMimeData>
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

    QMimeData headers;
    headers.setData("text/uri-list", this->reply->header(QNetworkRequest::ContentDispositionHeader).toByteArray());

    if (this->reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
        QStringList data = QString(this->reply->readAll()).split(',');
        const QRegularExpression regex("\"[^\"]+\"");
        for (int i = 0; i < data.size(); i++) {
            if (regex.match(data[i]).hasMatch()) {
                data[i] = data[i].replace('\n', ' ');
            }
        }

        this->file_path = save_path + headers.text().split('\'').last().remove(QRegularExpression("%([A-F0-9]{2})"));
        QFile table(this->file_path);
        table.open(QFile::WriteOnly);
        table.write(data.join(',').toUtf8());
    }

    emit finished();
}
