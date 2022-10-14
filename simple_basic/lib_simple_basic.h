#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include <map>
#include <queue>
#include <stdlib.h>

using namespace std;

// ���������, ����������� ����������
struct variable_struct {
	string name;	// ��� ����������
	int address;	// ����� ����� ������
	int value;		// ��������
};

extern vector<variable_struct> variables;

// ���������, ����������� ������ � ����������
struct line_simple_assembler {
	string address_cell;		// ����� ������
	string command;				// �������
	string operand;				// �������
};

extern std::vector<line_simple_assembler> assembler_lines;

extern map<string, short int> sb_accordance_sa;

//���������� �������
int INPUT(string parameters);
int OUTPUT(string parameters);
int GOTO(string parameters);
int IF(string line);
int LET(string line);
int END();

string convert(int x);
int read_file(const char *file_name);
void split(string str, vector<string> &return_vector);

void write(const char *file_name);
