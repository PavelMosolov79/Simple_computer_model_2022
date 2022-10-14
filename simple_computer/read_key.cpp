#include "read_key.h"

termios saved_terminal;

// функция сохранения настроек терминала
int terminal_save() {
	tcgetattr(0, &saved_terminal);	// получаем текущие настройки терминала и сохраняем их
    
	return 0;
}

// функция восстановления настроек терминала из сохранения
int terminal_restore() {
    tcsetattr(0, TCSAFLUSH, &saved_terminal);	// устанавливаем настройки терминала из сохранения
    
    return 0;
}

// функция смены режима работы терминала
int change_terminal_regime(bool regime, int vtime, int vmin, bool echo, bool sigint) {
    termios current_terminal;	// структура, описывающая текущие настройки терминала
    
    tcgetattr(0, &current_terminal);	// получаем текущие настройки терминала 
    
    /*
	 * В каноническом режиме терминал ждёт команды ввода от пользователя
	 * В неканоническом режиме терминал сразу отправляет на обработку введённые данные
	*/

	// если нужно включить канонический режим
	if (regime) {
		current_terminal.c_lflag |= ICANON;		// включаем канонический режим
	} else {	// если неканонический режим
        current_terminal.c_lflag &= ~ICANON;	// выключаем канонический режим (переводим в неканоничсекий режим)
        
        if (sigint) {
        	current_terminal.c_lflag |= ISIG;
		} else {
			current_terminal.c_lflag &= ~ISIG;
		}
		
		// отображение вводимых символов
		if (echo) {
			current_terminal.c_lflag |= ECHO;	// включаем
		} else {
			current_terminal.c_lflag &= ~ECHO;	// выключаем
		}
		
        current_terminal.c_cc[VMIN] = vmin;		// минимальное количество символов для неканонического чтения
        current_terminal.c_cc[VTIME] = vtime;	// время ожидания в децисекундах для неканонического чтения
    }
    
    tcsetattr(0, TCSAFLUSH, &current_terminal);	// устанавливаем новые настройки терминала
    
    return 0;
}

// функция считывания нажатой клавиши
int read_key(keys *key) {
	fflush(stdout);		// производим очистку потока вывода
	
    char buffer[5] = "\0";	// буффер для ввода
    
    change_terminal_regime(false, 30, 0, false, false);	// переводим терминал в неканонический режим, отключаем отображение вводимых символов + ...
    
	read(fileno(stdin), buffer, 5);		// получаем данные о нажатой клавише в буфер
    
    terminal_restore();		// восстанавливаем настройки терминала
    
    
    /* БЛОК ОБРАБОТКИ БУФЕРА: */
    
    if (buffer[0] == '\033') {
    	if (buffer[1] == '\0') {
    		*key = KEY_ESC;															// ESC: \033\0
		} else if (buffer[1] == '[') {
			if (buffer[2] == 'A' && buffer[3] == '\0') {
				*key = KEY_UP;														// UP: \033[A\0
			} else if (buffer[2] == 'B' && buffer[3] == '\0') {		
				*key = KEY_DOWN;													// DOWN: \033[B\0
			} else if (buffer[2] == 'C' && buffer[3] == '\0') {
				*key = KEY_RIGHT;													// RIGHT: \033[C\0
			} else if (buffer[2] == 'D' && buffer[3] == '\0') {
				*key = KEY_LEFT;													// LEFT: \033[D\0
			} else if (buffer[2] == '1' && buffer[3] == '5') {
				*key = KEY_F5;														// F5: \033[15
			} else if (buffer[2] == '1' && buffer[3] == '7') {
				*key = KEY_F6;														// F6: \033[17
			} else {
				*key = KEY_OTHER;													// остальные клавиши
			}    
		}
	} else if (buffer[0] == '\n' && buffer[1] == '\0') {
		*key = KEY_ENTER;															// ENTER: \n\0
	} else {
		if ((buffer[0] == 'l' or buffer[0] == 'L') && buffer[1] == '\0') {
			*key = KEY_L;															// L: L(l)\0
		} else if ((buffer[0] == 's' or buffer[0] == 'S') && buffer[1] == '\0') {
			*key = KEY_S;															// S: S(s)\0
		} else if ((buffer[0] == 'r' or buffer[0] == 'R') && buffer[1] == '\0') {
			*key = KEY_R;															// R: R(r)\0
		} else if ((buffer[0] == 't' or buffer[0] == 'T') && buffer[1] == '\0') {
			*key = KEY_T;															// T: T(t)\0
		} else if ((buffer[0] == 'i' or buffer[0] == 'I') && buffer[1] == '\0') {
			*key = KEY_I;															// I: I(i)\0
		} else {
			*key = KEY_OTHER;														// остальные клавиши
		}
	}
        
    return 0;
}
