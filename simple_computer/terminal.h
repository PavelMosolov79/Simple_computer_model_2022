#pragma once

#include <stdio.h>
#include <sys/ioctl.h>

// перечисление используемых цветов
enum colors {
	DEFAULT = 0,		// возврат цвета к дефолтному значению
    RED     = 196,		// красный
    GREEN   = 10,		// зелёный
    BLUE    = 20,		// синий
    BLACK   = 16,		// чёрный
    WHITE   = 15		// белый
};

int clear_screen();											// функция очистки экрана
int get_screen_size(int *count_rows, int *count_columns);	// функция получения размера терминала
int goto_xy(int x, int y);									// функция перемещения курсора на определённые координаты
int set_symbols_color(enum colors color);					// функция изменения цвета всем полседующим выводящимся символам
int set_background_color(enum colors color);				// функция изменения фона всех последующие выводящихся символов
int set_default_color();									// функция возвращения цветов к исходным
