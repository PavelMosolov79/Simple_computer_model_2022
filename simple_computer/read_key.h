#pragma once

#include <termio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern termios saved_terminal;	// ��������� ��� ���������� �������� ���������

// ������������ ������������ ������
enum keys {
	KEY_UP,			// ������� �����
    KEY_DOWN,		// ������� ����
    KEY_RIGHT,		// ������� ������
    KEY_LEFT, 		// ������� �����
    KEY_ESC,		// ������� "ESC"
    KEY_ENTER,		// ������� "ENTER"
	KEY_L,			// ������� "L"
    KEY_S,			// ������� "S"
    KEY_R,			// ������� "R"
    KEY_T,			// ������� "T"
    KEY_I,			// ������� "I"
    KEY_F5,			// ������� "F5"
    KEY_F6,			// ������� "F6"		
    KEY_OTHER		// ��� ��������� �������
};


/* �������: */

int terminal_save();			// ������� ���������� �������� ���������
int terminal_restore();			// ������� �������������� �������� ��������� �� ����������
int change_terminal_regime(bool regime, int vtime, int vmin, bool echo, bool sigint);	// ������� ����� ������ ������ ���������
int read_key(keys *key);		// ������� ���������� ������� �������

