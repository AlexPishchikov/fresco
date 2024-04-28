#pragma once

#include <QDialog>
#include <QMovie>
#include <QPointer>
#include <QString>
#include <QWidget>

#include "../../build/ui/ui_download_dialog.h"
#include "../Theme.h"
#include "../cache/Cache.h"
#include "../configs/DownloadDialogConfig.h"
#include "../workers/DownloadWorker.h"


class DownloadDialog : public QDialog {
    public:
        DownloadDialog(QWidget *parent = nullptr);
    private:
        Cache cache;

        DownloadDialogConfig config;

        QMovie loading_gif;

        QPointer<DownloadWorker> worker;

        QString current_column_name;

        QString current_table_name;

        QString current_url;

        Theme current_theme;

        Ui::DownloadDialogUi ui;

        bool is_valid_table(const QString &file_path) const;

        bool is_valid_url(const QString &url) const;

        bool is_url(const QString &url) const;

        QString get_sheet_id_from_url(const QString &url) const;

        QString get_table_id_from_url(const QString &url) const;

        QString get_url_by_name(const QString &filename) const;

        void fill_tables_combo_box();

        void load_table_by_url(const bool is_cached = false);

        void load_table_from_cache();

        void load_table_from_file();

        void run_loading_gif();

        void set_theme(const Theme theme);

        void set_widgets_enabled(const bool status);

        void show_fresco_window(const QString &data_file_name);

        void update_cache(const bool cache);

        void update_rating_column_names_combo_box(const QString &current_table_name);
};
