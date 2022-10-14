#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "lib_simple_basic.h"

using namespace std;

int main(int argc, char const *argv[]) {
	/* БЛОК ПРОВЕРКИ АРГУМЕТОВ ЗАПУСКА: */
	
	// если количество аргументов не равно 3
	if (argc != 3) {
		printf("\nThe number of arguments does not match the start condition!");
		printf("\nTo start, use the sample: simple_assembler file.sa file.o");
		printf("\nWhere file.sb - name of the file that contains the program in Simple Assembler,");
		printf("\n      file.sa - translation result.\n");
		
		return -1;
	}
	
	// если расширение первого файла не .sb
	if (strcmp(strrchr(argv[1], '.'), ".sb") != 0) {
		printf("\nThe extension of the first argument [%s]", argv[1]);
		printf("\ndoes not meet the start conditions");
		printf("\nThe file extension must be [.sb].\n");
		
		return -1;
	}
	
	// если расширение второго файла не .sa
	if (strcmp(strrchr(argv[2], '.'), ".sa") != 0) {
		printf("\nThe extension of the second argument [%s]", argv[2]);
		printf("\ndoes not meet the start conditions");
		printf("\nThe file extension must be [.sa].\n");
		
		return -1;
	}
	
	
    // читаем файл
	if (read_file(argv[1])) {
		return -1;
	}	
	
	// читаем файл
	write(argv[2]);
	
	return 0;
}
