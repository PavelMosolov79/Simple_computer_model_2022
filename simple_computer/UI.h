#pragma once

#include <iostream>
#include <string.h>

#include "simple_computer.h"
#include "terminal.h"
#include "read_key.h"
#include "big_chars.h"

/* ����������: */

extern short int current_memory_cell;


/* �������: */

int ui_initial();							// ������� ������������ ����������
int ui_update();							// ������� ���������� ���������
int drawing_boxes();						// ������� ��������� ���� ���������������
int drawing_texts();						// ������� ��������� ���������� � ������� � ��������
int drawing_memory();						// ������� ��������� ������� � �������
int drawing_accumulator();					// ������� ��������� �������� � ������������
int drawing_instruction_counter();			// ������� ��������� instruction counter
int drawing_operation();					// ������� ������������� ����������� ������ � ������ ��������
int drawing_flags();						// ������� ��������� ������
int drawing_big_char();						// ������� ��������� ����������� ������ �������� ���������
int move_current_memory_pointer(keys key);	// ������� ����������� ���������� ������
int load_memory_from_file();				// ������� �������� ������ �� �����
int save_memory_in_file();					// ������� ���������� ������ � ����
int set_instruction_counter();				// ������� ��������� instruction counter � ����������� �� ����� ������������
