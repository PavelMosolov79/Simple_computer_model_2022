#pragma once

#include <stdio.h>
#include <sys/ioctl.h>

// ������������ ������������ ������
enum colors {
	DEFAULT = 0,		// ������� ����� � ���������� ��������
    RED     = 196,		// �������
    GREEN   = 10,		// ������
    BLUE    = 20,		// �����
    BLACK   = 16,		// ������
    WHITE   = 15		// �����
};

int clear_screen();											// ������� ������� ������
int get_screen_size(int *count_rows, int *count_columns);	// ������� ��������� ������� ���������
int goto_xy(int x, int y);									// ������� ����������� ������� �� ����������� ����������
int set_symbols_color(enum colors color);					// ������� ��������� ����� ���� ����������� ����������� ��������
int set_background_color(enum colors color);				// ������� ��������� ���� ���� ����������� ����������� ��������
int set_default_color();									// ������� ����������� ������ � ��������
