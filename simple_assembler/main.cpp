#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib_simple_assembler.h"

int main(int argc, char const *argv[]) {
	/* ���� �������� ��������� �������: */
	
	// ���� ���������� ���������� �� ����� 3
	if (argc != 3) {
		printf("\nThe number of arguments does not match the start condition!");
		printf("\nTo start, use the sample: simple_assembler file.sa file.o");
		printf("\nWhere file.sa - name of the file that contains the program in Simple Assembler,");
		printf("\n      file.o  - translation result.\n");
		
		return -1;
	}
	
	// ���� ���������� ������� ����� �� .sa
	if (strcmp(strrchr(argv[1], '.'), ".sa") != 0) {
		printf("\nThe extension of the first argument [%s]", argv[1]);
		printf("\ndoes not meet the start conditions");
		printf("\nThe file extension must be [.sa].\n");
		
		return -1;
	}
	
	// ���� ���������� ������� ����� �� .o
	if (strcmp(strrchr(argv[2], '.'), ".o") != 0) {
		printf("\nThe extension of the second argument [%s]", argv[2]);
		printf("\ndoes not meet the start conditions");
		printf("\nThe file extension must be [.o].\n");
		
		return -1;
	}
	
	
	/* ���� ��������� ������� ����� */
	
	FILE *file;	// ����
		
	file = fopen(argv[1], "r");	// ������� ������� ���� ��� ������
	
	// ���� �� ���������� ������� ����
    if (file == NULL) {
    	printf("\nUnable to open the file [%s].\n", argv[1]);
        return -1;
    }
    
    // ������ ����
	if (read_file(file)) {
		return -1;
	}
	
	fclose(file);	// ��������� ����
	
	
	/* ���� ��������� ������� ����� */
	
	file = fopen(argv[2], "wb");	// ������� ������� ���� ��� ������
	
	// ���� �� ���������� ������� ����
    if (file == NULL) {
    	printf("\nUnable to open the file [%s].\n", argv[2]);
        return -1;
    }
    
    // ������ � ����
	if (write_file(file)) {
		return -1;
	}
    
	fclose(file);	// ��������� ����
	
	return 0;
}
