#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../commands.h"

const int MEMORY_SIZE = 100;	// ������ ������� � �������
extern short int memory[MEMORY_SIZE];	// ������ � �������

int read_file(FILE *file);
int write_file(FILE *file);
