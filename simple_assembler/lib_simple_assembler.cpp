#include "lib_simple_assembler.h"

short int memory[MEMORY_SIZE] = {0};	// ���������� ������� ������ 0

// ������� �����������:
// command - �������, operand - �������, value - ��������� �����������
int command_encode(short int command, short int operand, short int *value) {
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

// ������� ������ � ��������� ����� 
// file - ����, � ������� ��������
int read_file(FILE *file) {
	/* ����������: */
	char buffer[256];				// ������ ��� ��������� ������ �� �����
	short int current_line = 1;		// ����� ������� ������ � �����
	short int current_cell;			// ����� ������� ������ ������
	short int previous_cell = -1;	// ����� ���������� ������ ������
	short int command;				// �������
	short int operand;				// �������
	short int result;				// ���������
	char *part;						// �����-���� ����� �������
    
    /* ���� ��������� �����: */
    
    while (fgets(buffer, 256, file)) {
        /* ���� ���������� ������ ������: */
        
        part = strtok(buffer, " ");	// �������� ����� ������
        
        // ���� ����� ������ ������ �� ����� 2
        if (strlen(part) != 2) {
        	printf("\nError! In line [%d].", current_line);
        	printf("\nThe cell address number must be 2 characters long.");
        	
            return -1;
        }
        
        // �������� �� ������������ ������ ������ (����� ������ ���� �� 0 �� 9)
        for (int i = 0 ; i < 2 ; ++i) {
            if (part[i] < '0' || part[i] > '9') {
            	printf("\nError! In line [%d].", current_line);
            	printf("An invalid character was detected. Use the 10 number system.");
            	
                return -1;
            }
        }
        
        current_cell = atoi(part);	// ������� ���������� ���� ������ ������ � ���������
        
        // ���� ����� ������� ������ ������ ��� ����� ������ ���������� ������
        if (current_cell <= previous_cell) {
        	printf("\nError! In line [%d].", current_line);
        	printf("\nThe memory address number must be greater than the previous one and must not be repeated.");
        	
            return -1;
        } else {
        	previous_cell = current_cell; 
		}
		
		
		/* ���� ���������� �������: */
		
		part = strtok(NULL, " ");	// �������� �������. NULL - ������� ��������� ����� � ������������ ������
		
		// ���������� ����� ������� ��������
		if (strcmp(part, "READ") == 0) {
			command = READ;
		} else if (strcmp(part, "WRITE") == 0) {
			command = WRITE;
		} else if (strcmp(part, "LOAD") == 0) {
			command = LOAD;
		} else if (strcmp(part, "STORE") == 0) {
			command = STORE;
		} else if (strcmp(part, "ADD") == 0) {
			command = ADD;
		} else if (strcmp(part, "SUB") == 0) {
			command = SUB;
		} else if (strcmp(part, "DIVIDE") == 0) {
			command = DIVIDE;
		} else if (strcmp(part, "MUL") == 0) {
			command = MUL;
		} else if (strcmp(part, "JUMP") == 0) {
			command = JUMP;
		} else if (strcmp(part, "JNEG") == 0) {
			command = JNEG;
		} else if (strcmp(part, "JZ") == 0) {
			command = JZ;
		} else if (strcmp(part, "HALT") == 0) {
			command = HALT;
		} else if (strcmp(part, "JNS") == 0) {
			command = JNS;
		} else if (strcmp(part, "=") == 0) {
			command = 0;
		} else {
			printf("\nError! In line [%d].", current_line);
        	printf("\nUnknown command.");
			
            return -1;
        }
        
        
        /* ���� ���������� ��������: */
        
        part = strtok(NULL, " ");	// �������� �������. NULL - ������� ��������� ����� � ������������ ������
		
		// ���� �� ������������
        if (command != 0) {
        	// �������� �� ������������ �������� (����� ������ ���� �� 0 �� 9)
	        for (int i = 0; i < 2; ++i) {
	            if (part[i] < '0' || part[i] > '9') {
	            	printf("\nError! In line [%d].", current_line);
	            	printf("\nAn invalid character was detected. Use the 10 number system.");
	            	
	                return -1;
	            }
	        }
        	
        	operand = atoi(part);	// ������� ���������� ���� �������� � ���������
        	
        	// �������� ������� 
            if (command_encode(command, operand, &result)) {
            	printf("\nError! In line [%d].", current_line);
	            printf("\nInvalid command.");
	            
                return -1;
            }
        } else {	// ���� ������������
        	int index = 0;	// ������, ������������ ��� ��������� ������ ����� � ��������
        	
        	// ���� ���������� � '+'
        	if (part[0] == '+' || part[0] == '-') {
        		index = 1;	// �� �������� ��������� � 1 ������� (��������� ����)
			}
			
			unsigned short int number = atoi(&part[index]);	// ������� ���������� ���� �������� � ���������
			
			// ���� ������� ��������� � '+'
			if (index) {
				if (part[0] == '+') {
					result = number;
				} else {
					result = (11 << 14) | number;
				}
			} else {	// ���� ������� �� ��������� � '+'
				result = (1 << 14) | number;	// ���������� ���������
			}
		}  
        
        
        /* ���� ���������� �����������: */
        
        part = strtok(NULL, " ");	// �������� �����������. NULL - ������� ��������� ����� � ������������ ������
        
        // �������� �� �� ��� ���������� ����� �������� ������������
        if ((part != NULL) && (part[0] != ';') && (part[0] != '\n')) {
        	printf("\nError! In line [%d].", current_line);
	        printf("\nInvalid number of arguments or comment doesn't start with ';'.");
        	
            return -1; 
        }
        
        memory[current_cell] = result;	// ���������� ��������� � ������
            
		current_line++;	// ��������� �� ��������� �������
	}
	
    return 0;
}

// ������� ������ ������ � ���� 
// file - ����, � ������� ��������
int write_file(FILE *file) {
	fwrite(memory, sizeof(memory), 1, file);
	
	return 0;
}
