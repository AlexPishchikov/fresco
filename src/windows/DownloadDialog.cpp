#include <QColor>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QPoint>
#include <QPointer>
#include <QPushButton>
#include <QRadialGradient>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QWidget>

#include "../enums.h"
#include "../load_config/load_config.h"
#include "../workers/DownloadWorker.h"
#include "DownloadDialog.h"
#include "FrescoWindow.h"


DownloadDialog::DownloadDialog(QWidget *parent) : QDialog(parent) {
    this->config = load_config(":download_dialog_config_default");
    if (this->config["download_dialog_folder_name"].toString().at(-1) != '/') {
        this->config["download_dialog_folder_name"] = QJsonValue(QString("%1/").arg(this->config["download_dialog_folder_name"].toString()));
    }

    this->init_cached_tables_dir();

    this->ui.setupUi(this);

    this->setFixedSize(this->size());

    this->setWindowTitle(this->config["download_dialog_window_title"].toString());

    connect(this->ui.load_from_url_button, &QPushButton::clicked, this, [=]{this->load_table_by_url();});
    connect(this->ui.load_from_file_button, &QPushButton::clicked, this, [=]{this->load_table_from_file();});
    connect(this->ui.load_from_folder_button, &QPushButton::clicked, this, [=]{this->load_table_from_cache();});

    connect(this->ui.tables_combo_box, &QComboBox::currentTextChanged, this, [=]{this->update_rating_column_names_combo_box(this->ui.tables_combo_box->currentText());});

    this->set_theme(static_cast<Theme>(this->config["download_dialog_start_theme"].toInt() % Theme::count));
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this, [=]{this->set_theme(static_cast<Theme>((this->current_theme + 1) % Theme::count));});

    QRadialGradient gradient = QRadialGradient(QPoint(339, 20), 320, QPoint(319, 20), 20);
    gradient.setColorAt(0.0, QColor(0, 0, 0, 255));
    gradient.setColorAt(0.9, QColor(0, 0, 0, 255));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 0));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacityMask(gradient);
    this->ui.status_label->setGraphicsEffect(effect);

    this->fill_tables_combo_box();
}

bool DownloadDialog::is_valid_table(const QString &file_path) const {
    QFile table_file(file_path);
    table_file.open(QFile::ReadOnly);

    const QStringList columns_names = QString(table_file.readLine()).split(',');
    for (const QString &string : columns_names) {
        if (string.simplified() == this->ui.rating_column_name_combo_box->currentText().simplified()) {
            return true;
        }
    }

    return false;
}

bool DownloadDialog::is_valid_url() const {
    return this->is_url() && this->ui.url_line_edit->text().split("gid=").size() >= 2 && this->ui.url_line_edit->text().split('/').size() >= 2;
}

bool DownloadDialog::is_url() const {
    const QRegularExpression regex("^(?:https://|http://)?(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?))|(?:/?|[/?]\S+)$");
    return regex.match(this->ui.url_line_edit->text()).hasMatch();
}

QJsonArray DownloadDialog::get_cache_data(const QStringList &types) const {
    QFile cache_file(QString("%1.tables_list.json").arg(this->config["download_dialog_folder_name"].toString()));
    cache_file.open(QFile::ReadOnly);
    const QJsonArray cache_data = QJsonDocument::fromJson(cache_file.readAll()).array();

    QJsonArray data_array;

    for (const QJsonValueConstRef &object : cache_data) {
        QJsonObject temp_json_object;
        for (const QString &type : types) {
            temp_json_object.insert(type, object[QString(type)].toString());
        }
        data_array.push_back(temp_json_object);
    }

    return data_array;
}

QString DownloadDialog::get_sheet_id_from_url(const QString &url) const {
    return url.split("/edit").last().split('#').filter("gid=")[0].split('=').last();
}

QString DownloadDialog::get_table_id_from_url(const QString &url) const {
    const QStringList url_split = url.split('/');
    return url_split[url_split.size() - 2];
}

QString DownloadDialog::get_url_by_name(const QString &filename) const {
    const QJsonArray cache_data = this->get_cache_data(QStringList({ QString("table_name"), QString("url") }));
    for (const QJsonValueConstRef &object : cache_data) {
        if (object[QString("table_name")].toString() == filename) {
            return object[QString("url")].toString();
        }
    }

    return QString();
}

void DownloadDialog::fill_tables_combo_box() {
    const QDir tables_folder(this->config["download_dialog_folder_name"].toString());
    const QStringList files_list = tables_folder.entryList(QStringList() << "*.csv", QDir::Files);
    const QJsonArray tables_list = this->get_cache_data(QStringList("table_name"));
    for (const QJsonValueConstRef &table_name : tables_list) {
        const QString current_name = table_name[QString("table_name")].toString();
        if (files_list.contains(current_name) && this->ui.tables_combo_box->findText(current_name) == -1) {
            this->ui.tables_combo_box->addItem(current_name);
        }
    }
}

void DownloadDialog::init_cached_tables_dir() const {
    if (!QFileInfo::exists(this->config["download_dialog_folder_name"].toString())) {
        QDir().mkdir(this->config["download_dialog_folder_name"].toString());
    }

    if (!QFileInfo::exists(QString("%1.tables_list.json").arg(this->config["download_dialog_folder_name"].toString()))) {
        QFile tables_list_file(QString("%1.tables_list.json").arg(this->config["download_dialog_folder_name"].toString()));
        tables_list_file.open(QFile::WriteOnly);
    }
}

void DownloadDialog::load_table_by_url(const bool cache) {
    this->set_widgets_enabled(false);
    this->run_loading_gif();

    if (!this->is_valid_url()) {
        this->ui.status_label->setText("Некорректная ссылка");
        this->set_widgets_enabled(true);
        return;
    }

    const QString table_id = this->get_table_id_from_url(this->ui.url_line_edit->text());
    const QString sheet_id = this->get_sheet_id_from_url(this->ui.url_line_edit->text());

    this->ui.url_line_edit->setText(QString("%1/edit#gid=%2").arg(this->ui.url_line_edit->text().split("/edit")[0]).arg(sheet_id));

    const QUrl table_url = QString("https://docs.google.com/spreadsheets/d/%1/export?format=csv&id=%1&gid=%2").arg(table_id, sheet_id);

    if (!this->worker.isNull()) {
        delete this->worker;
    }

    this->worker = new DownloadWorker;

    connect(this->worker, &DownloadWorker::finished, this, [=]{this->update_cache(cache);});
    this->worker->download(table_url, this->config["download_dialog_folder_name"].toString());
}

void DownloadDialog::load_table_from_cache() {
    if (this->ui.tables_combo_box->currentText() == "" || this->ui.rating_column_name_combo_box->currentText() == "") {
        return;
    }

    this->setWindowTitle(QString("%1 ~ %2").arg(this->config["download_dialog_window_title"].toString())
                                           .arg(this->ui.tables_combo_box->currentText()));

    this->ui.url_line_edit->setText(this->get_url_by_name(this->ui.tables_combo_box->currentText()));
    this->load_table_by_url(true);
}

void DownloadDialog::load_table_from_file() {
    const QString file_path = QFileDialog::getOpenFileName(this, "Выбрать файл с таблицей", ".", "CSV(*.csv);;");
    if (file_path == "") {
        return;
    }

    this->run_loading_gif();

    QFile table_file(file_path);
    table_file.open(QFile::ReadOnly);

    auto file_data = QString(table_file.readAll()).split(',');
    const QRegularExpression regex("\"[^\"]+\"");

    for (int i = 0; i < file_data.size(); i++) {
        if (regex.match(file_data[i]).hasMatch()) {
            file_data[i] = file_data[i].replace('\n', ' ');
        }
    }

    table_file.close();
    table_file.open(QFile::WriteOnly);

    table_file.write(file_data.join(',').toUtf8());
    table_file.close();

    if (this->is_valid_table(file_path)) {
        this->show_fresco_window(file_path);
    }
    else {
        this->ui.status_label->setText("Некорректная структура таблицы");
    }
}

void DownloadDialog::run_loading_gif() {
    this->loading_gif.stop();
    this->ui.status_label->setMovie(&this->loading_gif);
    this->loading_gif.start();
}

void DownloadDialog::save_cache(const QJsonArray &cache_data) const {
    QFile cache_file(QString("%1.tables_list.json").arg(this->config["download_dialog_folder_name"].toString()));
    cache_file.open(QFile::WriteOnly);
    cache_file.write(QJsonDocument(cache_data).toJson());
}

void DownloadDialog::set_theme(const Theme theme) {
    this->current_theme = theme;

    this->loading_gif.setFileName(QString(":load_gif_%1").arg(this->current_theme));

    QFile qss_file(QString(":theme_%1").arg(this->current_theme));
    qss_file.open(QFile::ReadOnly);
    this->setStyleSheet(QLatin1String(qss_file.readAll()));
}

void DownloadDialog::set_widgets_enabled(const bool status) {
    this->ui.url_line_edit->setEnabled(status);
    this->ui.load_from_url_button->setEnabled(status);
    this->ui.load_from_file_button->setEnabled(status);
    this->ui.load_from_folder_button->setEnabled(status);
    this->ui.rating_column_name_combo_box->setEnabled(status);
    this->ui.switch_theme_button->setEnabled(status);
}

void DownloadDialog::show_fresco_window(const QString &data_file_name) {
    this->close();
    QPointer<FrescoWindow> fresco_window = new FrescoWindow(data_file_name, this->ui.rating_column_name_combo_box->currentText(), this->current_theme);
    fresco_window->show();
}

void DownloadDialog::update_cache(const bool cache) {
    this->setWindowTitle(this->config["download_dialog_window_title"].toString());

    if (this->worker->get_status_code() != 200) {
        if (cache) {
            QJsonArray cache_data = this->get_cache_data(QStringList({ QString("url"), QString("table_name"), QString("column_name") }));
            for (const QJsonValueConstRef &object : cache_data) {
                const QString current_name = object[QString("table_name")].toString();
                const QString current_path = QString("%1%2").arg(this->config["download_dialog_folder_name"].toString()).arg(current_name);
                if (object[QString("url")].toString() == this->ui.url_line_edit->text().simplified()) {
                    if (!this->is_valid_table(current_path)) {
                        this->ui.status_label->setText(QString("Не найден стобец «%1»").arg(this->ui.rating_column_name_combo_box->currentText()));
                        this->set_widgets_enabled(true);
                    }
                    else {
                        const QJsonObject current_data = QJsonObject({{ "url", object[QString("url")].toString() },
                                                                      { "table_name", current_name },
                                                                      { "column_name", this->ui.rating_column_name_combo_box->currentText() }});
                        if (!cache_data.contains(current_data)) {
                            cache_data.push_back(current_data);
                            this->save_cache(cache_data);
                        }
                        this->show_fresco_window(current_path);
                    }
                    break;
                }
            }
        }
        else {
            this->ui.status_label->setText("Не удалось загрузить по ссылке");
            this->set_widgets_enabled(true);
        }
    }
    else {
        const QJsonObject current_table_data = QJsonObject({{ "url", this->ui.url_line_edit->text().simplified() },
                                                            { "table_name", this->worker->get_file_path().split('/').last() },
                                                            { "column_name", this->ui.rating_column_name_combo_box->currentText() }});
        if (this->is_valid_table(this->worker->get_file_path())) {
            QJsonArray cache_data = this->get_cache_data(QStringList({ QString("url"), QString("table_name"), QString("column_name") }));
            if (!cache_data.contains(current_table_data)) {
                cache_data.push_back(current_table_data);
                this->save_cache(cache_data);
            }
            this->show_fresco_window(this->worker->get_file_path());
        }
        else {
            this->ui.status_label->setText(QString("Не найден стобец «%1»").arg(current_table_data[QString("column_name")].toString()));
            QFile::remove(this->worker->get_file_path());
            this->set_widgets_enabled(true);
        }
    }
}

void DownloadDialog::update_rating_column_names_combo_box(const QString &current_table_name) {
    QStringList rating_column_names;
    const QJsonArray cache_data = this->get_cache_data(QStringList({ QString("table_name"), QString("column_name") }));
    for (const QJsonValueConstRef &object : cache_data) {
        if (object[QString("table_name")].toString() == current_table_name) {
            rating_column_names.push_back(object[QString("column_name")].toString());
        }
    }
    if (rating_column_names.size() == 0) {
        rating_column_names.push_back(this->config["download_dialog_rating_column_name"].toString());
    }

    rating_column_names.sort();
    this->ui.rating_column_name_combo_box->clear();
    this->ui.rating_column_name_combo_box->addItems(rating_column_names);
}
