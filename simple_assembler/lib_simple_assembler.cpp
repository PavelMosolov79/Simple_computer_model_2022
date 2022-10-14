#include "lib_simple_assembler.h"

short int memory[MEMORY_SIZE] = {0};	// заполнение массива памяти 0

// функция кодирования:
// command - команда, operand - операнд, value - результат кодирования
int command_encode(short int command, short int operand, short int *value) {
	*value = 0;
	
	/* БЛОК ПРОВЕРОК: */
	
	// проверяем, что что размер команды не превышает 7 бит
    if (command > 0x7F) {
    	return  -1;
	}
    
    // проверяем, что что размер операнда не превышает 7 бит
    if (operand > 0x7F) {
    	return  -1;
	}

    
    /* БЛОК С КОДИРОВАНИЕМ: */

    // кодирование операнда
    for (int i = 0; i < 7; ++i) {
        short int bit = (operand >> i) & 1;		// узнаем значение бита (1 / 0)	
        *value |= (bit << i);					// устанавливаем в нужное место
    }
    
    // кодирование команды
    for (int i = 0; i < 7; ++i) {
        short int bit = (command >> i) & 1;		// узнаем значение бита (1 / 0)	
        *value |= (bit << (i + 7));				// устанавливаем в нужное место
    }
    
    return 0;
}

// функция чтения и обработки файла 
// file - файл, с которым работаем
int read_file(FILE *file) {
	/* КОМПОНЕНТЫ: */
	char buffer[256];				// буффер для считанных данных из файла
	short int current_line = 1;		// номер текущей строки в файле
	short int current_cell;			// адрес текущей ячейки памяти
	short int previous_cell = -1;	// адрес предыдущей ячейки памяти
	short int command;				// команда
	short int operand;				// операнд
	short int result;				// результат
	char *part;						// какая-либо часть команды
    
    /* БЛОК ОБРАБОТКИ ФАЙЛА: */
    
    while (fgets(buffer, 256, file)) {
        /* БЛОК СЧИТЫВАНИЯ АДРЕСА ЯЧЕЙКИ: */
        
        part = strtok(buffer, " ");	// получаем адрес ячейки
        
        // если длина адреса ячейки не равна 2
        if (strlen(part) != 2) {
        	printf("\nError! In line [%d].", current_line);
        	printf("\nThe cell address number must be 2 characters long.");
        	
            return -1;
        }
        
        // проверка на корректность адреса ячейки (цифры должны быть от 0 до 9)
        for (int i = 0 ; i < 2 ; ++i) {
            if (part[i] < '0' || part[i] > '9') {
            	printf("\nError! In line [%d].", current_line);
            	printf("An invalid character was detected. Use the 10 number system.");
            	
                return -1;
            }
        }
        
        current_cell = atoi(part);	// перевод строкового вида адреса ячейки в численный
        
        // если адрес текущей ячейки меньше или равен адресу предыдущей ячейки
        if (current_cell <= previous_cell) {
        	printf("\nError! In line [%d].", current_line);
        	printf("\nThe memory address number must be greater than the previous one and must not be repeated.");
        	
            return -1;
        } else {
        	previous_cell = current_cell; 
		}
		
		
		/* БЛОК СЧИТЫВАНИЯ КОМАНДЫ: */
		
		part = strtok(NULL, " ");	// получаем команду. NULL - функция продолжит поиск в оригинальной строке
		
		// определяем какая команда записана
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
        
        
        /* БЛОК СЧИТЫВАНИЯ ОПЕРАНДА: */
        
        part = strtok(NULL, " ");	// получаем операнд. NULL - функция продолжит поиск в оригинальной строке
		
		// если не присваивание
        if (command != 0) {
        	// проверка на корректность операнда (цифры должны быть от 0 до 9)
	        for (int i = 0; i < 2; ++i) {
	            if (part[i] < '0' || part[i] > '9') {
	            	printf("\nError! In line [%d].", current_line);
	            	printf("\nAn invalid character was detected. Use the 10 number system.");
	            	
	                return -1;
	            }
	        }
        	
        	operand = atoi(part);	// перевод строкового вида операнда в численный
        	
        	// кодируем команду 
            if (command_encode(command, operand, &result)) {
            	printf("\nError! In line [%d].", current_line);
	            printf("\nInvalid command.");
	            
                return -1;
            }
        } else {	// если присваивание
        	int index = 0;	// индекс, определяющий где находится начала ЧИСЛА в операнде
        	
        	// если начинается с '+'
        	if (part[0] == '+' || part[0] == '-') {
        		index = 1;	// то начинать обработку с 1 индекса (пропустив плюс)
			}
			
			unsigned short int number = atoi(&part[index]);	// перевод строкового вида операнда в численный
			
			// если операнд начинался с '+'
			if (index) {
				if (part[0] == '+') {
					result = number;
				} else {
					result = (11 << 14) | number;
				}
			} else {	// если операнд не начинался с '+'
				result = (1 << 14) | number;	// записываем результат
			}
		}  
        
        
        /* БЛОК СЧИТЫВАНИЯ КОММЕНТАРИЯ: */
        
        part = strtok(NULL, " ");	// получаем комментарий. NULL - функция продолжит поиск в оригинальной строке
        
        // проверка на то что полученная часть является комментарием
        if ((part != NULL) && (part[0] != ';') && (part[0] != '\n')) {
        	printf("\nError! In line [%d].", current_line);
	        printf("\nInvalid number of arguments or comment doesn't start with ';'.");
        	
            return -1; 
        }
        
        memory[current_cell] = result;	// записываем результат в память
            
		current_line++;	// переходим на следующую строчку
	}
	
    return 0;
}

// функция записи памяти в файл 
// file - файл, с которым работаем
int write_file(FILE *file) {
	fwrite(memory, sizeof(memory), 1, file);
	
	return 0;
}
