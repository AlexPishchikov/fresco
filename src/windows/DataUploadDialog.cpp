#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QLineEdit>
#include <QMovie>
#include <QPoint>
#include <QPushButton>
#include <QRadialGradient>
#include <QString>
#include <QStringList>
#include <QThreadPool>
#include <QWidget>

#include <future>
#include <thread>

#include "DataUploadDialog.h"
#include "../../ui_data_upload.h"
// #include "../config_load/DataUploadDialogConfig.h"
// #include "../windows/FrescoWindow.h"
#include "../workers/DownloadWorker.h"


DataUploadDialog::DataUploadDialog(QWidget *parent) : QDialog(parent) {
    // this->config = &DataUploadDialogConfig();

    this->init_cached_tables_dir();

    ui.setupUi(this);

    this->setFixedSize(this->size());

    connect(this->ui.load_from_url_button, &QPushButton::clicked, this, [=]{this->load_table_by_url();});
    connect(this->ui.load_from_file_button, &QPushButton::clicked, this, [=]{this->load_table_from_file();});
    connect(this->ui.load_from_folder_button, &QPushButton::clicked, this, [=]{this->load_table_from_cache();});

    this->current_theme = QString("dark");
    this->switch_theme(this->current_theme);
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this, [=]{this->switch_theme(this->current_theme == "dark" ? "light" : "dark");});

    QRadialGradient gradient = QRadialGradient(QPoint(339, 20), 320, QPoint(319, 20), 20);
    gradient.setColorAt(0.0, QColor(0, 0, 0, 255));
    gradient.setColorAt(0.9, QColor(0, 0, 0, 255));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 0));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacityMask(gradient);
    this->ui.status_label->setGraphicsEffect(effect);

    this->fill_combo_box();
}

void DataUploadDialog::init_cached_tables_dir() const {
    if (!QFileInfo::exists("uploaded_tables")) {
        QDir().mkdir("uploaded_tables");
    }
}

QStringList DataUploadDialog::get_tables_list() const {
    QFile tables_list_file("uploaded_tables/tables_list");
    tables_list_file.open(QFile::ReadOnly);

    QStringList tables_list;

    while (!tables_list_file.atEnd()) {
        QStringList line = QString(tables_list_file.readLine()).split(' ');
        line.pop_back();
        tables_list.append(line.join(' '));
    }
    return tables_list;
}

void DataUploadDialog::fill_combo_box() {
    const QDir tables_folder("uploaded_tables/");
    const QStringList files_list = tables_folder.entryList(QStringList() << "*.csv", QDir::Files);
    const QStringList tables_list = get_tables_list(); 
    for (int i = 0; i < files_list.size(); i++) {
        if (tables_list.contains(files_list[i])) {
            this->ui.tables_combo_box->addItem(files_list[i]);
        }
    }
}

void DataUploadDialog::load_table_from_file() {
    const QString file_path = QFileDialog::getOpenFileName(this, "Выбрать файл с таблицей", ".", "CSV(*.csv);;");
    if (file_path == "") {
        return;
    }

    this->run_loading_gif();

    if (this->check_table_structure(file_path)) {
        this->show_fresco_window(file_path);
    }
    else {
        this->ui.status_label->setText("Некорректная структура таблицы");
    }
}

void DataUploadDialog::load_table_from_cache() {
    if (this->ui.tables_combo_box->currentText() == "") {
        return;
    }

    const QString url = QString("https://docs.google.com/spreadsheets/d/%1/edit#gid=%2")
                                .arg(this->get_sheet_id_from_file(this->ui.tables_combo_box->currentText()))
                                .arg(this->get_table_id_from_file(this->ui.tables_combo_box->currentText()));
    this->ui.url_line_edit->setText(url);
    this->load_table_by_url(true);
}

void DataUploadDialog::load_table_by_url(const bool cache) {
    // this->ui.status_label->setText("");
    this->switch_widgets_status();
    this->run_loading_gif();

    if (!this->is_correct_url()) {
        this->ui.status_label->setText(QString("Некорректная ссылка"));
        this->switch_widgets_status();
        return;
    }

    const QString table_id = this->get_table_id_from_url(this->ui.url_line_edit->text());
    const QString sheet_id = this->get_sheet_id_from_url(this->ui.url_line_edit->text());

    const QString load_table_url = QString("https://docs.google.com/spreadsheets/d/%1/export?format=csv&id=%1&gid=%2").arg(table_id, sheet_id);

    // QThread thread;
    static DownloadWorker worker;
    // worker.moveToThread(&thread);
    // thread.start();

    // auto f = std::async(std::launch::async, [=]{worker.download(load_table_url);});


    // worker.signals.finished.connect([=]{this->process_downloaded_table(this->ui.url_line_edit->text(), worker.result, cache};))
    // connect(worker, &DownloadWorker::finished, this, [=]{this->process_downloaded_table(this->ui.url_line_edit->text(), worker.result, cache)};)
    // threadpool.start([=]{worker.download(load_table_url);});

    worker.download(load_table_url);
    // connect(threadpool, &QThreadPool::start, this, [=]{worker.download(load_table_url);});
    // QThreadPool::globalInstance()->start([=]{worker.download(load_table_url);});
    // qDebug() << "sdf";

    // f.wait();
    // qDebug() << "sdf2";

}

QString DataUploadDialog::get_table_id_from_url(const QString &url) const {
    const QStringList url_split = url.split('/');
    return url_split[url_split.size() - 2];
}

QString DataUploadDialog::get_sheet_id_from_url(const QString &url) const {
    return url.split('=')[1];
}

QString DataUploadDialog::get_table_id_from_file(const QString &filename) const {
    // QStringList tables_list = DataUploadDialog::get_tables_list();
    // return DataUploadDialog::get_table_id_from_url(tables_list[filename]);
    return QString();
}

QString DataUploadDialog::get_sheet_id_from_file(const QString &filename) const {
    // QStringList tables_list = DataUploadDialog::get_tables_list();
    // return DataUploadDialog::get_sheet_id_from_url(tables_list[filename]);
    return QString();
}

bool DataUploadDialog::is_correct_url() const {
    if (this->is_url() && this->ui.url_line_edit->text().split('=').size() >= 2 && this->ui.url_line_edit->text().split('/').size() >= 2) {
        bool is_number;
        this->ui.url_line_edit->text().split('=')[1].toUInt(&is_number, 10);
        return is_number;
    }
    return false;
}

bool DataUploadDialog::is_url() const {
    // regex = re.compile(r'^(?:https://|http://)?(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?))|(?:/?|[/?]\S+)$', re.IGNORECASE)
    // return re.match(regex, self.url_line_edit.text()) is not None
    return true;
}

void DataUploadDialog::process_downloaded_table(const QString &table_url, const int response, const bool cache) {
    // if response == None:
    //     if not cache:
    //         self.status_label.setText('Нет интернета')
    //         self.switch_widgets_status()
    //         return
    //     else:
    //         filename = list(self.get_tables_list())[list(self.get_tables_list().values()).index(table_url)]
    //         data_file_name = f'{self.config.upload_data_folder_name}/{filename}'
    //         self.switch_widgets_status()
    // else:
    //     try:
    //         data_file_name = f'{self.config.upload_data_folder_name}/' + urllib.parse.unquote(response.headers["Content-Disposition"].split("; filename*=UTF-8''")[-1])
    //     except KeyError:
    //         if cache:
    //             filename = list(self.get_tables_list())[list(self.get_tables_list().values()).index(table_url)]
    //             data_file_name = f'{self.config.upload_data_folder_name}/{filename}'
    //             self.switch_widgets_status()
    //         else:
    //             self.status_label.setText('Не удалось загрузить по ссылке')
    //             self.switch_widgets_status()
    //             return
    //     if response.status_code == 200:
    //         if not os.path.exists(self.config.upload_data_folder_name):
    //             os.mkdir(self.config.upload_data_folder_name)
    //         if not os.path.exists(f'{self.config.upload_data_folder_name}/tables_list'):
    //             with open(f'{self.config.upload_data_folder_name}/tables_list', 'x') as _:
    //                 pass
    //         with open(data_file_name, 'w') as table_file:
    //             table_file.write(response.content.decode('utf-8'))

    //         tables_list = self.get_tables_list()

    //         tables_list[data_file_name.split('/')[-1]] = self.url_line_edit.text()
    //         with open(f'{self.config.upload_data_folder_name}/tables_list', 'w') as tables_list_file:
    //             for name in tables_list:
    //                 tables_list_file.write(f'{name} {tables_list[name]}\n')
    //     elif not cache:
    //         self.status_label.setText('Не удалось загрузить по ссылке')
    //         self.switch_widgets_status()
    //         return

    // if self.check_table_structure(data_file_name):
    //     self.show_main_window(data_file_name)
    // else:
    //     self.status_label.setText('Некорректная структура таблицы')
    //     os.remove(data_file_name)
    //     tables_list = self.get_tables_list()
    //     with open(f'{self.config.upload_data_folder_name}/tables_list', 'w') as tables_list_file:
    //         tables_list.pop(data_file_name.split('/')[-1])
    //         for name in tables_list:
    //             tables_list_file.write(f'{name} {tables_list[name]}\n')
    //     self.switch_widgets_status()
    //     return
}

bool DataUploadDialog::check_table_structure(const QString &file_path) const {
    QFile table_file(file_path);
    table_file.open(QFile::ReadOnly);
    table_file.readLine();
    const QString line = table_file.readLine();

    return line.split(',').contains("Промежуточный рейтинг");
}

void DataUploadDialog::show_fresco_window(const QString &data_file_name) {
    this->close();
    // FrescoWindow(data_file_name, this->config.rating_col_name, this->current_theme).show();
}

void DataUploadDialog::run_loading_gif() {
    this->loading_gif.stop();
    this->ui.status_label->setMovie(&this->loading_gif);
    this->loading_gif.start();
}

void DataUploadDialog::switch_widgets_status() {
    this->ui.url_line_edit->setEnabled(!this->ui.url_line_edit->isEnabled());
    this->ui.load_from_url_button->setEnabled(!this->ui.load_from_url_button->isEnabled());
    this->ui.load_from_file_button->setEnabled(!this->ui.load_from_file_button->isEnabled());
    this->ui.load_from_folder_button->setEnabled(!this->ui.load_from_folder_button->isEnabled());
    this->ui.switch_theme_button->setEnabled(!this->ui.switch_theme_button->isEnabled());
}

void DataUploadDialog::switch_theme(const QString &theme) {
    this->current_theme = theme;
    this->loading_gif.setFileName(QString(":load_gif_%1").arg(theme));

    QFile qss_file(QString(":%1_theme").arg(theme));
    qss_file.open(QFile::ReadOnly);
    this->setStyleSheet(QLatin1String(qss_file.readAll()));
}
