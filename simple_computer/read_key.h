#pragma once

#include <termio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern termios saved_terminal;	// структура для сохранения настроек терминала

// перечисление используемых клавиш
enum keys {
	KEY_UP,			// стрелка вверх
    KEY_DOWN,		// стрелка вниз
    KEY_RIGHT,		// стрелка вправо
    KEY_LEFT, 		// стрелка влево
    KEY_ESC,		// клавиша "ESC"
    KEY_ENTER,		// клавиша "ENTER"
	KEY_L,			// клавиша "L"
    KEY_S,			// клавиша "S"
    KEY_R,			// клавиша "R"
    KEY_T,			// клавиша "T"
    KEY_I,			// клавиша "I"
    KEY_F5,			// клавиша "F5"
    KEY_F6,			// клавиша "F6"		
    KEY_OTHER		// все остальные клавиши
};


/* ФУНКЦИИ: */

int terminal_save();			// функция сохранения настроек терминала
int terminal_restore();			// функция восстановления настроек терминала из сохранения
int change_terminal_regime(bool regime, int vtime, int vmin, bool echo, bool sigint);	// функция смены режима работы терминала
int read_key(keys *key);		// функция считывания нажатой клавиши

