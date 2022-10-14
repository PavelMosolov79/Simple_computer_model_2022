#include "big_chars.h"

unsigned int big_chars[][2]  = {
	{0xDBC3C3FF, 0xFFC3C3DB},	// 0
    {0x1C1C1CFC, 0xFF1C1C1C},	// 1
    {0xFF0303FF, 0xFFC0C0C0},	// 2
    {0xFF0303FF, 0xFF0303FF},	// 3
	{0xC3C3C3C3, 0x030303FF},	// 4
	{0xFFC0C0FF, 0xFF030303},	// 5
	{0xFFC0C0FF, 0xFFC3C3C3},	// 6
	{0x0C0603FF, 0x18181818},	// 7
	{0xFFC3C3FF, 0xFFC3C3FF},	// 8
	{0xC3C3C3FF, 0xFF0303FF},	// 9
	{0xC3C36618, 0xC3C3C3FF},	// A
	{0xFEC3C6FC, 0xFCC6C3FE},	// B
	{0xC0C3C37E, 0x7EC3C3C0},	// C
	{0xC3C3C6FC, 0xFCC6C3C3}, 	// D
	{0xFFE0FFFF, 0xFFFFE0FF},	// E
	{0xFFE0FFFF, 0xE0E0E0FF},	// F
	{0x7E180000, 0x00000018},	// +
	{0x7E000000, 0x00000000},	// -
};

// функция вывода символа на экран
int print_char(char symbol) {
	printf("\033(0%c\033(B", symbol);	// последовательность для вывода символа на экран
	
    return 0;
}

// функция вывода прямоугольника на экран
int print_box(int x, int y, int width, int height) {
	int rows;		// текущее количество строчек в терминале
	int columns;	// текущее количество столбцов в терминале
	
    // получаем размер терминала и выходим, если функция завершилась с ошибкой
    if (get_screen_size(&rows, &columns) == -1) {
    	return -1;
	}
	
	// проверяем на корректность области в которой рисовать прямоугольник
	if ((x <= 0) || (x + width - 1 > columns)) {
		return -1;
	}
	
	// проверяем на корректность области в которой рисовать прямоугольник
	if ((y <= 0) || (y + height - 1 > rows)) {
		return -1;
	}
	
	// проверяем на корректность области в которой рисовать прямоугольник
	if ((width <= 1) || (height <= 1)) {
		return -1;
	}
	
	// отрисовка верхнего левого угла
    goto_xy(x, y);
    print_char((char)L_U_CORNER);
    
    // отрисовка верхнего правого угла
    goto_xy(x + width - 1, y);
    print_char((char)R_U_CORNER);
    
    // отрисовка нижнего правого угла
    goto_xy(x + width - 1, y + height - 1);
    print_char((char)R_D_CORNER);
    
    // отрисовка нижнего левого угла
    goto_xy(x, y + height - 1);
    print_char((char)L_D_CORNER);

    // отрисовка горизонтальных линий
    for (int i = 1; i < width - 1; ++i) {
        // отрисовка верхней линии 
        goto_xy(x + i, y);
        print_char((char)H_LINE);
        
        // отрисовка нижней линии
        goto_xy(x + i, y + height - 1);
        print_char((char)H_LINE);
    }

    // отрисовка вертикальных линий
    for (int i = 1; i < height - 1; ++i) {
        // отрисовка левой линии 
        goto_xy(x, y + i);
        print_char((char)V_LINE);
        
		// отрисовка правой линии
        goto_xy(x + width - 1, y + i);
        print_char((char)V_LINE);
    }
    
    return 0;
}

// функция вывода большого символа на экран
int print_big_char(unsigned int *symbol, int x, int y) {
	// цикл по столбцам
    for (int i = 0; i < 8; ++i) {
    	// цикл по строчкам
    	for (int j = 0; j < 8; ++j) {
    		bool value;		// значение бита
    		
    		// получаем значение бита большого символа, если не получилось то выходим
    		if (get_big_char_position(symbol, i, j, &value)) {
    			return -1;
			}
    		
    		// перемещаем курсор
			goto_xy(x + i, y + j);
    		
    		// если значение бита '1'
    		if (value) {
    			print_char((char)FILL);		// закрашиваем клетку терминала
			} else {
				printf(" ");				// иначе оставляем пустой
			}
		}
	}
        
    return 0;
}

// функция установка значения бита в большои символе
int set_big_char_position(unsigned int *symbol, int x, int y, bool value) {
	// проверяем выход за границы большого символа
	if ((x < 0) || (x > 7)) {
		return -1;
	}
	
	// проверяем выход за границы большого символа
	if ((y < 0) || (y > 7)) {
		return -1;
	}
	
	// если необходимо установить '1'
	if (value) {
		symbol[int(y / 4)] |= (1 << (8 * (y % 4) + (7 - x)));	// в строку символа записываем строку где на нужном месте стоит '1'
	} else {
		symbol[int(y / 4)] |= ~(1 << (8 * (y % 4) + (7 - x)));	// иначе в строку символа записываем строку где на нужном месте стоит '0'
	}

    return 0;
}

// функция получения значения бита в большом символе
int get_big_char_position(unsigned int *symbol, int x, int y, bool *value) {
	// проверяем выход за границы большого символа
	if ((x < 0) || (x > 7)) {
		return -1;
	}
	
	// проверяем выход за границы большого символа
	if ((y < 0) || (y > 7)) {
		return -1;
	}
	
	int tmp = symbol[int(y / 4)] & (1 << (8 * (y % 4) + (7 - x)));	// вычисляем что стоит на месте нужного бита (0 / 1)
	
	// если стоит '1'
	if (tmp) {
		*value = true;		// передаём в value начение true
	} else {
		*value = false;		// иначе передаём в value начение false
	}
	
    return 0;
}

// функция записи заданного количества больщих символов в файл
int big_char_write(int file, unsigned int *symbol, int count) {
	// записываем большие символы в файл, если не получается, то выходим
	if (write(file, symbol, count * 2 * sizeof(unsigned int))) {
		return -1;
	}
	
    return 0;
}

// функция чтения из файла заданного количества больщих символов
int big_char_read(int file, unsigned int *symbol, int need_count, int *count) {
	*count = 0;
	
	// цикл по символам в файле
    for (int i = 0; i < need_count * 2; ++i) {
    	// считываем из файла, если не получается, то выходим
        if (read(file, &symbol[i], sizeof(unsigned int)) == -1) {
        	return -1;
		}
		
		// проверяем что был считан полностью символ,а не его половина и прибавляем к счётчику 1
        if (!((i + 1) % 2)) {
        	(*count)++;
		}
    }

    return 0;
}
