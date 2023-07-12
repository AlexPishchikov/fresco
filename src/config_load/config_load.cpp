import json


def config_load(path) -> dict:
    config = dict()
    with open(path) as default_config:
        config = json.load(default_config)
    try:
        with open('config.json') as user_config:
            user_config = json.load(user_config)
            for key in user_config:
                try:
                    if type(config[key]) == type(user_config[key]):
                        config[key] = user_config[key]
                except KeyError:
                    continue
        return config
    except FileNotFoundError:
        return config
    except json.decoder.JSONDecodeError:
        return config
