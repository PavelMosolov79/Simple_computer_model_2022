#include "UI.h"

// ������� ������������ ����������
int ui_initial() {
	instruction_counter = 0;	// ��������� �� ������� ������ ������ ������������� � 0
	
	// ��������� ��������� ���������, ���� �� ���������� - �������
	if (terminal_save()) {
		return -1;
	}
	
	accumulator = 0;	// �������� �����������
	
	memory_init();		// �������������� ������
	register_init();	// ������������� �������
	
	register_set(IGNORING_TACT_PULSES, true);	// �������� ���� ������������� �������� ���������
	
	return 0;
}

// ������� ���������� ���������
int ui_update() {
	//system("clear");
	clear_screen();		// ������� �����
	
	// ������������ ��� ��������������, ���� �� ���������� - �����
    if (drawing_boxes()) {
    	return -1;
	}
	
	// ������������ ��� ������, ���� �� ���������� - �����
    if (drawing_texts()) {
    	return -1;
	}
	
	// ��������� ������, ���� �� ���������� - �����
    if (drawing_memory()) {
    	return -1;
	}
	
	// ��������� �������� � ������������, ���� �� ���������� - �����
    if (drawing_accumulator()) {
    	return -1;
	}
	
	// ��������� �������� instruction counter, ���� �� ���������� - �����
    if (drawing_instruction_counter()) {
    	return -1;
	}
	
	// ��������� �������� operation, ���� �� ���������� - �����
    if (drawing_operation()) {
    	return -1;
	}
	
	// ��������� ������, ���� �� ���������� - �����
    if (drawing_flags()) {
    	return -1;
	}
	
	// ��������� ������� ��������, ���� �� ���������� - �����
    if (drawing_big_char()) {
    	return -1;
	}
	
    goto_xy(1, 23);				// ���������� ������
    
    printf("Input/Output:\n");	// ������� ��������� � ����� / ������
    
    return 0;
}

// ������� ��������� ���� ���������������
int drawing_boxes() {
	// ������������ ���� memory, ���� �� ���������� - �����
    if (print_box(1, 1, 61, 12)) {
    	return -1;
	}
	
	// ������������ ���� accumulator, ���� �� ���������� - �����
    if (print_box(62, 1, 22, 3)) {
    	return -1;
	}
	
	// ������������ ���� instruction counter, ���� �� ���������� - �����
    if (print_box(62, 4, 22, 3)) {
    	return -1;
	}
	
	// ������������ ���� operation, ���� �� ���������� - �����
    if (print_box(62, 7, 22, 3)) {
    	return -1;
	}
	
	// ������������ ���� flags, ���� �� ���������� - �����
    if (print_box(62, 10, 22, 3)){
    	return -1;
	}
	
	// ������������ ���� Big Chars, ���� �� ���������� - �����
    if (print_box(1, 13, 52, 10)) {
    	return -1;
	}
	
	// ������������ ���� Big Chars, ���� �� ���������� - �����
    if (print_box(53, 13, 31, 10)) {
    	return -1;
	}
	
    return 0;
}

// ������� ��������� ���������� � ������� � ��������
int drawing_texts() {
	/* ���� ��������� ����������: */
	
    goto_xy(30, 1);
    printf(" Memory ");
    
    goto_xy(66, 1);
    printf(" Accumulator ");
    
    goto_xy(63, 4);
    printf(" InstructionCounter ");
    
    goto_xy(67, 7);
    printf(" Operation ");
    
    goto_xy(69, 10);
    printf(" Flags ");
    
    goto_xy(54, 13);
    printf(" Keys: ");

    
    /* ���� ��������� ������� � �������������� ��������: */
    
    char* hot_keys[7] = {
		(char *)"l  - load",
        (char *)"s  - save",
        (char *)"r  - run",
        (char *)"t  - step",
        (char *)"i  - reset",
        (char *)"F5 - accumulator",
        (char *)"F6 - instructionCounter"
	};

    for (int i = 0; i < 7; ++i) {
        goto_xy(54, i + 14);		// ���������� ������
        printf("%s", hot_keys[i]);	// ������� ������� �� �������������� �������
    }
    
    return 0;
}

// ������� ��������� ������� � ������� 
int drawing_memory() {
	// ���� �� ��������
    for (int i = 0; i < 10; ++i) {
    	// ���� �� ��������
    	for (int j = 0; j < 10; ++j) {
            goto_xy(2 + (5 * j + j), 2 + i);	// ���������� ������
            
            short int value;
            memory_get(i * 10 + j, &value);		// �������� �������� ������ ������
            
            // ���� ������ ������ ������ �������� ���������
            if ((i * 10 + j) == instruction_counter) {
            	set_background_color(colors::GREEN);
			}
			
			// ���� �����
            if((value >> 14) & 1) {
            	// ���� ������������� �����
            	if((value >> 15) & 1) {
            		printf("-%04x", value & 0x3FFF);
				} else {	// ���� �������������
					printf(" %04x", value & 0x3FFF);
				}
			} else {	// ���� �������
				printf("+%04x", value);
			}
			
            set_default_color();	// ��������������� ����� � ��������
        }
	}
        
    return 0;
}

// ������� ��������� �������� � ������������
int drawing_accumulator() {
    goto_xy(70, 2);		// ���������� ������
	
	// ���� � ������������ �����
    if((accumulator >> 14) & 1) {
    	// ���� ������������� �����
    	if((accumulator >> 15) & 1) {
    		printf("-%04x", accumulator & 0x3FFF);
		} else {	// ���� ������������� �����
			printf(" %04x", accumulator & 0x3FFF);
		}
	} else {	// ���� �������
		printf("+%04x", accumulator);
	}
	
    return 0;
}

// ������� ��������� instruction counter
int drawing_instruction_counter() {
    goto_xy(71, 5);		// ���������� ������

    printf("%04x", instruction_counter);		// ������� �� �����

    return 0;
}

// ������� ������������� ����������� ������ � ������ ��������
int drawing_operation() {
    goto_xy(69, 8);		// ���������� ������

    short int value;	// ��������, ���������� � ������
    memory_get(instruction_counter, &value);	// �������� �������� 
    
    // ���� � ������ �������� �������
    if (!((value >> 14) & 1)) {
        short int command, operand;
        
        command_decode(value, &command, &operand);	// ���������� ����������� ������
        
        printf("+%x:%x", command, operand);	// ������� �� �����
    } else {	// ���� �����
        // ���� ������������� �����
    	if((value >> 15) & 1) {
    		printf("-%04x", value & 0x3FFF);
		} else {	// ���� ������������� �����
			printf(" %04x", value & 0x3FFF);
		}
    }
    
    return 0;
}

// ������� ��������� ������
int drawing_flags() {
    char flags[] = {'O', 'Z', 'M', 'I', 'C'};	// ��������� �����
    
    for (int i = 0; i < REGISTER_SIZE; ++i) {
        bool value;
        
        if (register_get(i, &value)) {		// �������� �������� �����
        	return -1;
		}
		
        goto_xy(68 + (i * 2), 11);		// ���������� ������
        
        // ���� ���� ����������
        if (value) {
            printf("%c", flags[i]);
        } else {	// ���� ���� �� ����������
            printf(" ");
        }
    }
    
    return 0;
}

// ������� ��������� ����������� ������ �������� ���������
int drawing_big_char() {
    short int value;
    
    memory_get(instruction_counter, &value);	// �������� ��������, ���������� � ������
    
    // ���� �������
    if(!((value >> 14) & 1)) {
    	print_big_char(big_chars[16], 2, 14);		// ������� +
	} else if((value >> 13) & 1) {		// ���� ������������� �����
        print_big_char(big_chars[17], 2, 14); 		// ������� -
        value = (~(value - 1)) & 0x3FFF;			// ����������
    } else {	// ���� ������������� �����
    	value &= 0x3FFF;
	}
    
    // ����� ������� ��������
    for (int i = 0; i < 4; ++i) {
        int char_id = (value & (0xF << (4 * (3 - i)) )) >> (4 * (3 - i));		// ���������� ����� ������ �������
        
        print_big_char(big_chars[char_id], 2 + 8 * (i + 1) + 2 * (i + 1), 14);	// ������� ������
    }

    return 0;
}

// ������� ����������� ���������� ������
int move_current_memory_pointer(keys key) {
    switch (key) {
        case keys::KEY_UP:
        	if (instruction_counter <= 9) {
        		instruction_counter += 90;
			} else {
				instruction_counter -= 10;
			}
			
			return 0;
			
		case keys::KEY_DOWN:
        	if (instruction_counter >= 90) {
        		instruction_counter -= 90;
			} else {
				instruction_counter += 10;
			}
			
			return 0;
        
        case keys::KEY_RIGHT:
        	if (!((instruction_counter + 1) % 10)) {
        		instruction_counter -= 9;
			} else {
				instruction_counter += 1;
			}
			
			return 0;
		
        case keys::KEY_LEFT:
			if (!(instruction_counter % 10)) {
				instruction_counter +=9;
			} else {
				instruction_counter -= 1;
			}
			
			return 0;
			
		default:
			break;
    }
    
    return -1;
}

// ������� �������� ������ �� �����
int load_memory_from_file() {
    char file_name[102];	// ���� �� �����
    
    // ����� ��������� � ��� ��� ����� �������
    printf("Loading file...\n");
    printf("Enter the file name to load > ");
    
    fgets(file_name, 102, stdin);	// �������� ���� �� ������������
    
    // �������� ��������� ������ �� �����, ���� �� ���������� - �������
    if (memory_load(file_name)) {
    	printf("Failed to load memory!");
        return -1;
    } else {	// ���� ����������
    	printf("Successful load!");
	}
    
	return 0;
}

// ������� ���������� ������ � ����
int save_memory_in_file() {
    char file_name[102];		// ���� �� �����
    
    // ����� ��������� � ��� ��� ����� �������
    printf("Saving file...\n");
    printf("Enter the file name to save > ");

    fgets(file_name, 102, stdin);		// �������� ���� �� ������������

	// �������� ��������� ������ � ����, ���� �� ���������� - �������
    if (memory_save(file_name)) {
    	printf("Failed to save memory!");
        return -1;
    } else {	// ���� ����������
    	printf("Successful save!");
	}
	
    return 0;
}

// ������� ��������� instruction counter � ����������� �� ����� ������������
int set_instruction_counter() {
    int value = 0;
    
    // ����� ��������� � ��� ��� ����� �������
    printf("\nSet a value InstructionCounter between 0 and 99 inclusive");
    printf("\nEnter value in DEC format > ");
    
    scanf("%d", &value);							// �������� ���� �� ������������
	
	// ��������� ����� �� �������
    if (value > 99 || value < 0) {
        printf("\nError! Out of memory!");			// ������� ��������� �� ������
    	return -1;
    }
    
    instruction_counter = (uint8_t)value;		// ������������� ����� �������� instruction counter

    return 0;
}

// ������� ��������� instruction counter � ����������� �� ����� ������������
int set_accumulator() {
    char buffer[10];	// ������ ��� �����
    
    // ����� ��������� � ��� ��� ����� �������
    printf("\nSet a value accumulator");
    printf("\nEnter value in HEX format > ");
    
    fgets(buffer, 10, stdin);	// �������� ���� �� ������������
    
    // �������� ������������ �����
    for (int i = 0; i < int(strlen(buffer)); ++i) {
    	if (!(buffer[i] >= '0' && buffer[i] <= '9')) {
    		printf("\nError! Invalid input!");			// ������� ��������� �� ������
    		return -1;
		}
	}
	
    short int value = atoi(buffer);
	
	// ��������� ����� �� �������
    if (value > 99 || value < 0) {
        printf("\nError! Out of memory!");			// ������� ��������� �� ������
    	return -1;
    }
    
    instruction_counter = (uint8_t)value;		// ������������� ����� �������� instruction counter

    return 0;
}
