### DownloadDialogConfig.json
Конфиг для окна загрузки данных.
#### Ключи:
- `download_dialog_folder_name`

    **Тип:** `string`

    **Описание:** Путь до папки, в которой сохраняются скачанные таблицы.

- `download_dialog_rating_col_name`

    **Тип:** `string`

    **Описание:** Имя столбца с рейтингом в таблице.

### FrescoWindowConfig.json
Конфиг для основного окна.
#### Ключи:
- `fresco_cells_count_init_value`

    **Тип:** `int`

    **Описание:** Начальное значение для `cells_count_spin_box`(общее количество кнопок в рулетке).

    **Ограничение:** `[3, 26]`

- `fresco_win_cells_count_init_value`

    **Тип:** `int`

    **Описание:** Начальное значение для `not_empty_cells_count_spin_box`(количество выигрышных кнопок в рулетке).

    **Ограничение:** `[0, cells_count_init_value]`

- `fresco_attempts_count_init_value`

    **Тип:** `int`

    **Описание:** Начальное значение для `attempts_count_spin_box`(количество попыток при игре в рулетку).

    **Ограничение:** `[0, cells_count_init_value]`

- `fresco_const_upper_label_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта для надписи «Загадка от Жака Фреско».

- `fresco_const_lower_label_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта для надписи «На размышление даётся».

- `fresco_total_time_label_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта для надписи «n секунд».

- `fresco_question_label_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта для вопроса.

- `fresco_remaining_time_label_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта для обратного отсчёта.

- `fresco_time_interval`

    **Тип:** `int`

    **Описание:** Время между обновлениями таймера(label'a c обратным отсчётом).

    **Ограничение:** `[1, 1000]`

- `fresco_custom_student_rating`

    **Тип:** `int`

    **Описание:** Рейтинг, который назначается по умолчанию тем студентам, имена которых отсутствуют в начальной таблице.

- `fresco_calculate_time_function`

    **Тип:** `string`

    **Описание:** Функция, написанная на `JavaScript`. Вычисляет количество времени на размышления. Переменная `rating` отвечает за количество плюсиков студента.

- `fresco_window_title`

    **Тип:** `string`

    **Описание:** Название окна с Жаком Фреско.

- `fresco_name_combo_box_placeholder`

    **Тип:** `string`

    **Описание:** Значение по умолчанию для выпадающего списка со студентами.

- `fresco_generate_riddle_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке, генерирующей загадку.

- `fresco_start_timer_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке, запускающей таймер.
- `fresco_stop_timer_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке, останавливающей таймер.

- `fresco_select_questions_file_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке для выбора файла с вопросами.

- `fresco_time_over_text`

    **Тип:** `string`

    **Описание:** Текст, который будет показан на месте таймера, когда время на размышление закончится.

- `fresco_roulette_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке, запускающей рулетку.

- `fresco_refresh_questions_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке, обновляющей вопросы.

- `fresco_clear_button_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке, очищающей окно.

- `fresco_question_placeholder`

    **Тип:** `string`

    **Описание:** Текст, который будет показан вместо вопроса, если таковых не осталось.

- `fresco_name_icon_color`

    **Тип:** `string`

    **Описание:** Цвет иконки рядом с именем опрошенного студента. Можно задавать в формате `hex`(`#RRGGBB` или `#AARRGGBB`) или словом. Слова, обозначающие цвет, можно глянуть [тут](https://doc.qt.io/qt-6/qml-color.html#svg-color-reference).

### RouletteDialogConfig.json
Конфиг для рулетки.
#### Ключи:

- `roulette_shot_hole_label_size`

    **Тип:** `int`

    **Описание:** Размер label'а с картинкой дырки от пули. Сама дырка в 4 раза меньше.

- `roulette_button_size`

    **Тип:** `int`

    **Описание:** Размер кнопок рулетки.

- `roulette_button_margin`

    **Тип:** `int`

    **Описание:** Отступ кнопок рулетки.

- `roulette_buttons_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта на кнопках рулетки.

- `roulette_close_button_font_size`

    **Тип:** `int`

    **Описание:** Размер шрифта на кнопке выхода.

- `roulette_close_button_init_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке выхода до выстрела.

- `roulette_close_button_win_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке выхода после удачного выстрела.

- `roulette_close_button_lose_text`

    **Тип:** `string`

    **Описание:** Текст на кнопке выхода после неудачного выстрела.

- `roulette_spin_step`

    **Тип:** `int`

    **Описание:** Время между перемещением кнопок.

    **Ограничение:** `[1, 1000]`

- `roulette_spin_function`

   **Тип:** `string`

   **Описание:** Функция, написанная на `JavaScript`, производная по `t` которой отвечает за скорость вращения кнопки. Можно использовать переменные `t` и `i`(последняя отвечает за номер кнопки).
