#pragma once

#include <iostream>
#include <string.h>

#include "simple_computer.h"
#include "terminal.h"
#include "read_key.h"
#include "big_chars.h"

/* КОМПОНЕНТЫ: */

extern short int current_memory_cell;


/* ФУНКЦИИ: */

int ui_initial();							// функция инциализации интерфейса
int ui_update();							// функция обновления терминала
int drawing_boxes();						// функция отрисовки всех прямоугольников
int drawing_texts();						// функция отрисовки заголовков и справки о клавишах
int drawing_memory();						// функция отрисовки массива с памятью
int drawing_accumulator();					// функция отрисовки значения в аккумуляторе
int drawing_instruction_counter();			// функция отрисовки instruction counter
int drawing_operation();					// функция декодирования содержимого ячейки и вывода операции
int drawing_flags();						// функция отрисовки флагов
int drawing_big_char();						// функция отрисовки содержимого ячейки большими символами
int move_current_memory_pointer(keys key);	// функция перемещения выделенной ячейки
int load_memory_from_file();				// функция загрузки памяти из файла
int save_memory_in_file();					// функция сохранения памяти в файл
int set_instruction_counter();				// функция изменения instruction counter в зависимости от ввода пользователя
