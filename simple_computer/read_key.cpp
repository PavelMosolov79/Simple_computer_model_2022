#include "read_key.h"

termios saved_terminal;

// ������� ���������� �������� ���������
int terminal_save() {
	tcgetattr(0, &saved_terminal);	// �������� ������� ��������� ��������� � ��������� ��
    
	return 0;
}

// ������� �������������� �������� ��������� �� ����������
int terminal_restore() {
    tcsetattr(0, TCSAFLUSH, &saved_terminal);	// ������������� ��������� ��������� �� ����������
    
    return 0;
}

// ������� ����� ������ ������ ���������
int change_terminal_regime(bool regime, int vtime, int vmin, bool echo, bool sigint) {
    termios current_terminal;	// ���������, ����������� ������� ��������� ���������
    
    tcgetattr(0, &current_terminal);	// �������� ������� ��������� ��������� 
    
    /*
	 * � ������������ ������ �������� ��� ������� ����� �� ������������
	 * � �������������� ������ �������� ����� ���������� �� ��������� �������� ������
	*/

	// ���� ����� �������� ������������ �����
	if (regime) {
		current_terminal.c_lflag |= ICANON;		// �������� ������������ �����
	} else {	// ���� �������������� �����
        current_terminal.c_lflag &= ~ICANON;	// ��������� ������������ ����� (��������� � �������������� �����)
        
        if (sigint) {
        	current_terminal.c_lflag |= ISIG;
		} else {
			current_terminal.c_lflag &= ~ISIG;
		}
		
		// ����������� �������� ��������
		if (echo) {
			current_terminal.c_lflag |= ECHO;	// ��������
		} else {
			current_terminal.c_lflag &= ~ECHO;	// ���������
		}
		
        current_terminal.c_cc[VMIN] = vmin;		// ����������� ���������� �������� ��� ��������������� ������
        current_terminal.c_cc[VTIME] = vtime;	// ����� �������� � ������������ ��� ��������������� ������
    }
    
    tcsetattr(0, TCSAFLUSH, &current_terminal);	// ������������� ����� ��������� ���������
    
    return 0;
}

// ������� ���������� ������� �������
int read_key(keys *key) {
	fflush(stdout);		// ���������� ������� ������ ������
	
    char buffer[5] = "\0";	// ������ ��� �����
    
    change_terminal_regime(false, 30, 0, false, false);	// ��������� �������� � �������������� �����, ��������� ����������� �������� �������� + ...
    
	read(fileno(stdin), buffer, 5);		// �������� ������ � ������� ������� � �����
    
    terminal_restore();		// ��������������� ��������� ���������
    
    
    /* ���� ��������� ������: */
    
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
				*key = KEY_OTHER;													// ��������� �������
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
			*key = KEY_OTHER;														// ��������� �������
		}
	}
        
    return 0;
}
