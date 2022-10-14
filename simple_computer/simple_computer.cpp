#include "simple_computer.h"

//� �������� ����������� ������ ������������ ������ ����� �����
//������������ ���������� � ������ ����������
short int memory[MEMORY_SIZE];
short int register_flags;

short int accumulator;
short int instruction_counter;

//////////////////������ � ����������� �������///////////////////

//������������� ����������� ������, ������� ������� ������� ��������
int memory_init() {
	for (int i = 0; i < MEMORY_SIZE; ++i) {
		memory[i] = 0;
	}
	
	return 0;
}

//������ �������� (value) ��������� ������ ����������� ������ (address)
int memory_set(int address, short int value) {
	//���� ����� ������ ������ ������ "MEMORY_SIZE", ������ ���� OUT_OF_MEMORY � ���������� -1
    if (address >= MEMORY_SIZE) {
        //sc_regSet(OUT_OF_MEMORY, true);
        return -1;
    }

    memory[address] = value;
    return 0;
}

//������������� �������� (value) ��������� ������ ����������� ������ (address)
int memory_get(int address, short int * value) {
    //���� ����� ������ ������ ������ "MEMORY_SIZE", ������ ���� OUT_OF_MEMORY � ���������� -1
	if (address >= MEMORY_SIZE) {
        //sc_regSet(OUT_OF_MEMORY, true);
        return -1;
    }
    
    *value = memory[address];
    return 0;
}

//��������� ���������� ������ � ����(file_name) � ��������� ����, ��������� ������� fwrite
int memory_save(char* file_name) {
	FILE *file = fopen(file_name, "wb");
	//���� ���� �� ���������� �������, ���������� -1
	if (file == NULL) {
		return -1;
	}
	
    fwrite(memory, sizeof(memory), 1, file);
    
    fclose(file);
    
    return 0;
}

//��������� �� ���������� �����(file_name) ���������� ����������� ������, ��������� ������� fwrite
int memory_load(char* file_name) {
	FILE *file = fopen(file_name, "rb");
	//���� ���� �� ���������� �������, ���������� -1
	if (file == NULL) {
		return -1;
	}
	
	fread(memory, sizeof(memory), 1, file);
	
    fclose(file);
    
    return 0;
}

///////////////////////����������� ������///////////////////////

//�������� �������  � ��������� ������� � ��������� � �������� ��������� � value
int command_encode(short int command, short int operand, short int* value) {
	*value = 0;
	
	/* ���� ��������: */
	
	// ���������, ��� ��� ������ ������� �� ��������� 7 ���
    if (command > 0x7F) {
    	return  -1;
	}
    // ���������, ��� ��� ������ �������� �� ��������� 7 ���
    if (operand > 0x7F) {
    	return  -1;
	}

    
    /* ���� � ������������: */

    // ����������� ��������
    for (int i = 0; i < 7; ++i) {
        short int bit = (operand >> i) & 1;		// ������ �������� ���� (1 / 0)	
        *value |= (bit << i);					// ������������� � ������ �����
    }  
    // ����������� �������
    for (int i = 0; i < 7; ++i) {
        short int bit = (command >> i) & 1;		// ������ �������� ���� (1 / 0)	
        *value |= (bit << (i + 7));				// ������������� � ������ �����
    }
    
    return 0;
}

//���������� �������� ��� ������� SimpleComputer
int command_decode(short int value, short int* command, short int* operand) {
	short int tmp_command = 0;
	short int tmp_operand = 0;
	
    if ((value >> 14) & 1) {
        register_set(INCORRECT_COMMAND, true);
        return  -1;
    }

    for (int i = 0; i < 7; ++i) {
        short int bit = (value >> i) & 1;
        tmp_operand |= (bit << i) ;
    }

    for (int i = 0; i < 7; ++i) {
        int bit = (value >> (i + 7)) & 1;
        tmp_command |= (bit << i) ;
    }
 	
	*command = tmp_command;
	*operand = tmp_operand;
	
	return 0;  
}

//////////////////������ � ���������� ������///////////////////

//������������� ������� ������, �������� ����������
int register_init() {
	register_flags = 0;
	
    return 0;
}

//������������� ��������(bool) ���������� �������� ������(reg)
int register_set(short int reg, bool value) {
	//���� ������� ����� �� �������, ���������� -1
    if (reg < 0 || reg >= REGISTER_SIZE) {
    	return -1;
	}
	
	if (value == true) {
		register_flags |= (1 << reg);
	} else {
		register_flags &= ~(1 << reg);
	}

    return 0;
}

int register_get(short int reg, bool *value) {
	//���� ������� ����� �� �������, ���������� -1
	if (reg < 0 || reg >= REGISTER_SIZE) {
    	return -1;
	}

    *value = register_flags & (1 << reg);
    
    return 0;
}
