#pragma once

#include <stdio.h>
#include <stdlib.h>

#define OVERFLOW				0   //������������
#define DIVISION_BY_ZERO		1	//������� �� ����
#define OUT_OF_MEMORY			2	//������������ ������
#define IGNORING_TACT_PULSES	3	//������������� ��������� ��������
#define INCORRECT_COMMAND		4	//���������� ��������

const int MEMORY_SIZE = 100;
const int REGISTER_SIZE = 5;

extern short int memory[MEMORY_SIZE];
extern short int register_flags;

extern short int accumulator;
extern short int instruction_counter;

int memory_init();
int register_init();
int memory_set(int address, short int value);
int memory_get(int address, short int *value);
int memory_save(char* file_name);
int memory_load(char* file_name);
int register_set(short int reg, bool value);
int register_get(short int reg, bool *value);
int command_encode(short int command, short int operand, short int* value);
int command_decode(short int value, short int* command, short int* operand);
