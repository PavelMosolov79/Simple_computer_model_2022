#pragma once

#include <unistd.h>

#include "terminal.h"

/* �������� �������������: */

#define FILL 	   'a'		// ���������
#define L_U_CORNER 'l'		// ����� ������� ����
#define R_U_CORNER 'k'		// ������ ������� ����
#define R_D_CORNER 'j' 		// ������ ������ ����
#define L_D_CORNER 'm'		// ����� ������ ����
#define H_LINE     'q'		// �������������� �����
#define V_LINE     'x'		// ������������ �����


/* ����������: */
extern unsigned int big_chars[][2];	// ������ � ���������


/* �������: */
int print_char(char symbol);													// ������� ������ ������� �� �����
int print_box(int x, int y, int width, int height);								// ������� ������ �������������� �� �����
int print_big_char(unsigned int *symbol, int x, int y);							// ������� ������ �������� ������� �� �����
int set_big_char_position(unsigned int *symbol, int x, int y, bool value);		// ������� ��������� �������� ���� � ������� �������
int get_big_char_position(unsigned int *symbol, int x, int y, bool *value);		// ������� ��������� �������� ���� � ������� �������
int big_char_write(int file, unsigned int *symbol, int count);					// ������� ������ ��������� ���������� ������� ��������
																				// � ����
int big_char_read(int file, unsigned int *symbol, int need_count, int *count);	// ������� ������ �� ����� ��������� ���������� �������
																				// ��������
