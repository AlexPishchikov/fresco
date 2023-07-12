import json

from src.config_load.config_load import config_load


class DataUploadDialogConfig():
    def __init__(self):
        config = config_load('res/default_config/DataUploadingDialogConfig.json')
        self.upload_data_folder_name = config['data_uploading_folder_name']
        self.rating_col_name = config['data_uploading_rating_col_name']
