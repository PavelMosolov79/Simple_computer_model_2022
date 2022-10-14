#pragma once

#include <unistd.h>

#include "terminal.h"

/* ЭЛЕМЕНТЫ ПСЕВДОГРАФИКИ: */

#define FILL 	   'a'		// штриховка
#define L_U_CORNER 'l'		// левый верхний угол
#define R_U_CORNER 'k'		// правый верхний угол
#define R_D_CORNER 'j' 		// правый нижний угол
#define L_D_CORNER 'm'		// левый нижний угол
#define H_LINE     'q'		// горизонтальная линия
#define V_LINE     'x'		// вертикальная линия


/* КОМПОНЕНТЫ: */
extern unsigned int big_chars[][2];	// массив с символами


/* ФУНКЦИИ: */
int print_char(char symbol);													// функция вывода символа на экран
int print_box(int x, int y, int width, int height);								// функция вывода прямоугольника на экран
int print_big_char(unsigned int *symbol, int x, int y);							// функция вывода большого символа на экран
int set_big_char_position(unsigned int *symbol, int x, int y, bool value);		// функция установка значения бита в большои символе
int get_big_char_position(unsigned int *symbol, int x, int y, bool *value);		// функция получения значения бита в большом символе
int big_char_write(int file, unsigned int *symbol, int count);					// функция записи заданного количества больщих символов
																				// в файл
int big_char_read(int file, unsigned int *symbol, int need_count, int *count);	// функция чтения из файла заданного количества больщих
																				// символов
