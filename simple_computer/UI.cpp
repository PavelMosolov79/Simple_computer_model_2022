#include "UI.h"

// функция инциализации интерфейса
int ui_initial() {
	instruction_counter = 0;	// укащатель на текущую ячейку памяти устанавливаем в 0
	
	// сохраняем настройки терминала, если не получается - выходим
	if (terminal_save()) {
		return -1;
	}
	
	accumulator = 0;	// обнуляем аккумулятор
	
	memory_init();		// инициализируем память
	register_init();	// инициализиуем регистр
	
	register_set(IGNORING_TACT_PULSES, true);	// включаем флаг ИГНОРИРОВАНИЕ ТАКТОВЫХ ИМПУЛЬСОВ
	
	return 0;
}

// функция обновления терминала
int ui_update() {
	//system("clear");
	clear_screen();		// очищаем экран
	
	// отрисовываем все прямоугольники, если не получается - выход
    if (drawing_boxes()) {
    	return -1;
	}
	
	// отрисовываем все текста, если не получается - выход
    if (drawing_texts()) {
    	return -1;
	}
	
	// отрисовка памяти, если не получается - выход
    if (drawing_memory()) {
    	return -1;
	}
	
	// отрисовка значения в аккумуляторе, если не получается - выход
    if (drawing_accumulator()) {
    	return -1;
	}
	
	// отрисовка значения instruction counter, если не получается - выход
    if (drawing_instruction_counter()) {
    	return -1;
	}
	
	// отрисовка значения operation, если не получается - выход
    if (drawing_operation()) {
    	return -1;
	}
	
	// отрисовка флагов, если не получается - выход
    if (drawing_flags()) {
    	return -1;
	}
	
	// отрисовка больших символов, если не получается - выход
    if (drawing_big_char()) {
    	return -1;
	}
	
    goto_xy(1, 23);				// перемещаем курсор
    
    printf("Input/Output:\n");	// выводим сообщение о вводе / выводе
    
    return 0;
}

// функция отрисовки всех прямоугольников
int drawing_boxes() {
	// отрисовываем окно memory, если не получается - выход
    if (print_box(1, 1, 61, 12)) {
    	return -1;
	}
	
	// отрисовываем окно accumulator, если не получается - выход
    if (print_box(62, 1, 22, 3)) {
    	return -1;
	}
	
	// отрисовываем окно instruction counter, если не получается - выход
    if (print_box(62, 4, 22, 3)) {
    	return -1;
	}
	
	// отрисовываем окно operation, если не получается - выход
    if (print_box(62, 7, 22, 3)) {
    	return -1;
	}
	
	// отрисовываем окно flags, если не получается - выход
    if (print_box(62, 10, 22, 3)){
    	return -1;
	}
	
	// отрисовываем окно Big Chars, если не получается - выход
    if (print_box(1, 13, 52, 10)) {
    	return -1;
	}
	
	// отрисовываем окно Big Chars, если не получается - выход
    if (print_box(53, 13, 31, 10)) {
    	return -1;
	}
	
    return 0;
}

// функция отрисовки заголовков и справки о клавишах
int drawing_texts() {
	/* БЛОК ОТРИСОВКИ ЗАГОЛОВКОВ: */
	
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

    
    /* БЛОК ОТРИСОВКИ СПРАВКИ О ФУНКЦИОНАЛЬНЫХ КЛАВИШАХ: */
    
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
        goto_xy(54, i + 14);		// перемещаем курсор
        printf("%s", hot_keys[i]);	// выводим справку по функциональной клавише
    }
    
    return 0;
}

// функция отрисовки массива с памятью 
int drawing_memory() {
	// цикл по столбцам
    for (int i = 0; i < 10; ++i) {
    	// цикл по строчкам
    	for (int j = 0; j < 10; ++j) {
            goto_xy(2 + (5 * j + j), 2 + i);	// перемещаем курсор
            
            short int value;
            memory_get(i * 10 + j, &value);		// получаем значение ячейки памяти
            
            // если данная ячейка памяти является выбранной
            if ((i * 10 + j) == instruction_counter) {
            	set_background_color(colors::GREEN);
			}
			
			// если число
            if((value >> 14) & 1) {
            	// если отрицательное число
            	if((value >> 15) & 1) {
            		printf("-%04x", value & 0x3FFF);
				} else {	// если положительное
					printf(" %04x", value & 0x3FFF);
				}
			} else {	// если команда
				printf("+%04x", value);
			}
			
            set_default_color();	// восстанавливаем цвета к исходным
        }
	}
        
    return 0;
}

// функция отрисовки значения в аккумуляторе
int drawing_accumulator() {
    goto_xy(70, 2);		// перемещаем курсор
	
	// если в аккумуляторе число
    if((accumulator >> 14) & 1) {
    	// если отрицательное число
    	if((accumulator >> 15) & 1) {
    		printf("-%04x", accumulator & 0x3FFF);
		} else {	// если положительное число
			printf(" %04x", accumulator & 0x3FFF);
		}
	} else {	// если команда
		printf("+%04x", accumulator);
	}
	
    return 0;
}

// функция отрисовки instruction counter
int drawing_instruction_counter() {
    goto_xy(71, 5);		// перемещаем курсор

    printf("%04x", instruction_counter);		// выводим на экран

    return 0;
}

// функция декодирования содержимого ячейки и вывода операции
int drawing_operation() {
    goto_xy(69, 8);		// перемещаем курсор

    short int value;	// значение, хранящаеся в ячейке
    memory_get(instruction_counter, &value);	// получаем значение 
    
    // если в ячейке записана команда
    if (!((value >> 14) & 1)) {
        short int command, operand;
        
        command_decode(value, &command, &operand);	// декодируем содерижимое ячейки
        
        printf("+%x:%x", command, operand);	// выводим на экран
    } else {	// если число
        // если отрицательное число
    	if((value >> 15) & 1) {
    		printf("-%04x", value & 0x3FFF);
		} else {	// если положительное число
			printf(" %04x", value & 0x3FFF);
		}
    }
    
    return 0;
}

// функция отрисовки флагов
int drawing_flags() {
    char flags[] = {'O', 'Z', 'M', 'I', 'C'};	// возможные флаги
    
    for (int i = 0; i < REGISTER_SIZE; ++i) {
        bool value;
        
        if (register_get(i, &value)) {		// получаем значение флага
        	return -1;
		}
		
        goto_xy(68 + (i * 2), 11);		// перемещаем курсор
        
        // если флаг установлен
        if (value) {
            printf("%c", flags[i]);
        } else {	// если флаг не установлен
            printf(" ");
        }
    }
    
    return 0;
}

// функция отрисовки содержимого ячейки большими символами
int drawing_big_char() {
    short int value;
    
    memory_get(instruction_counter, &value);	// получаем значение, хранящееся в ячейке
    
    // если команда
    if(!((value >> 14) & 1)) {
    	print_big_char(big_chars[16], 2, 14);		// выводим +
	} else if((value >> 13) & 1) {		// если отрицательное число
        print_big_char(big_chars[17], 2, 14); 		// выводим -
        value = (~(value - 1)) & 0x3FFF;			// декодируем
    } else {	// если положительное число
    	value &= 0x3FFF;
	}
    
    // вывод больших символов
    for (int i = 0; i < 4; ++i) {
        int char_id = (value & (0xF << (4 * (3 - i)) )) >> (4 * (3 - i));		// определяем какой символ записан
        
        print_big_char(big_chars[char_id], 2 + 8 * (i + 1) + 2 * (i + 1), 14);	// выводим символ
    }

    return 0;
}

// функция перемещения выделенной ячейки
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

// функция загрузки памяти из файла
int load_memory_from_file() {
    char file_name[102];	// путь до файла
    
    // вывод сообщения о том что нужно сделать
    printf("Loading file...\n");
    printf("Enter the file name to load > ");
    
    fgets(file_name, 102, stdin);	// получаем ввод от пользователя
    
    // пытаемся загрузить память из файла, если не получается - выходим
    if (memory_load(file_name)) {
    	printf("Failed to load memory!");
        return -1;
    } else {	// если получается
    	printf("Successful load!");
	}
    
	return 0;
}

// функция сохранения памяти в файл
int save_memory_in_file() {
    char file_name[102];		// путь до файла
    
    // вывод сообщения о том что нужно сделать
    printf("Saving file...\n");
    printf("Enter the file name to save > ");

    fgets(file_name, 102, stdin);		// получаем ввод от пользователя

	// пытаемся сохранить память в файл, если не получается - выходим
    if (memory_save(file_name)) {
    	printf("Failed to save memory!");
        return -1;
    } else {	// если получается
    	printf("Successful save!");
	}
	
    return 0;
}

// функция изменения instruction counter в зависимости от ввода пользователя
int set_instruction_counter() {
    int value = 0;
    
    // вывод сообщения о том что нужно сделать
    printf("\nSet a value InstructionCounter between 0 and 99 inclusive");
    printf("\nEnter value in DEC format > ");
    
    scanf("%d", &value);							// получаем ввод от пользователя
	
	// проверяем выход за границы
    if (value > 99 || value < 0) {
        printf("\nError! Out of memory!");			// выводим сообщение об ошибке
    	return -1;
    }
    
    instruction_counter = (uint8_t)value;		// устанавливаем новое значение instruction counter

    return 0;
}

// функция изменения instruction counter в зависимости от ввода пользователя
int set_accumulator() {
    char buffer[10];	// буффер для ввода
    
    // вывод сообщения о том что нужно сделать
    printf("\nSet a value accumulator");
    printf("\nEnter value in HEX format > ");
    
    fgets(buffer, 10, stdin);	// получаем ввод от пользователя
    
    // проверям корректность ввода
    for (int i = 0; i < int(strlen(buffer)); ++i) {
    	if (!(buffer[i] >= '0' && buffer[i] <= '9')) {
    		printf("\nError! Invalid input!");			// выводим сообщение об ошибке
    		return -1;
		}
	}
	
    short int value = atoi(buffer);
	
	// проверяем выход за границы
    if (value > 99 || value < 0) {
        printf("\nError! Out of memory!");			// выводим сообщение об ошибке
    	return -1;
    }
    
    instruction_counter = (uint8_t)value;		// устанавливаем новое значение instruction counter

    return 0;
}
