#include "simple_computer.h"

//В качестве оперативной памяти используется массив целых чисел
//определенный статически в рамках библиотеки
short int memory[MEMORY_SIZE];
short int register_flags;

short int accumulator;
short int instruction_counter;

//////////////////РАБОТА С ОПЕРАТИВНОЙ ПАМЯТЬЮ///////////////////

//иницилизирует оперативную память, задавая ячейкам нулевое значение
int memory_init() {
	for (int i = 0; i < MEMORY_SIZE; ++i) {
		memory[i] = 0;
	}
	
	return 0;
}

//задает значение (value) указанной ячейке оперативной памяти (address)
int memory_set(int address, short int value) {
	//если адрес ячейки памяти больше "MEMORY_SIZE", ставим флаг OUT_OF_MEMORY и возвращаем -1
    if (address >= MEMORY_SIZE) {
        //sc_regSet(OUT_OF_MEMORY, true);
        return -1;
    }

    memory[address] = value;
    return 0;
}

//устанавливает значение (value) указанной ячейке оперативной памяти (address)
int memory_get(int address, short int * value) {
    //если адрес ячейки памяти больше "MEMORY_SIZE", ставим флаг OUT_OF_MEMORY и возвращаем -1
	if (address >= MEMORY_SIZE) {
        //sc_regSet(OUT_OF_MEMORY, true);
        return -1;
    }
    
    *value = memory[address];
    return 0;
}

//сохраняет содержимое памяти в файл(file_name) в бинароном виде, используя функцию fwrite
int memory_save(char* file_name) {
	FILE *file = fopen(file_name, "wb");
	//если файл не получилось открыть, возвращаем -1
	if (file == NULL) {
		return -1;
	}
	
    fwrite(memory, sizeof(memory), 1, file);
    
    fclose(file);
    
    return 0;
}

//загружает из указанного файла(file_name) содержимое оперативной памяти, используя функцию fwrite
int memory_load(char* file_name) {
	FILE *file = fopen(file_name, "rb");
	//если файл не получилось открыть, возвращаем -1
	if (file == NULL) {
		return -1;
	}
	
	fread(memory, sizeof(memory), 1, file);
	
    fclose(file);
    
    return 0;
}

///////////////////////КОДИРОВАНИЕ КОМАНД///////////////////////

//кодирует команду  с указанным номером и операндом и помещает результат в value
int command_encode(short int command, short int operand, short int* value) {
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

//декодирует значение как команду SimpleComputer
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

//////////////////РАБОТА С РЕГИСТРАМИ ФЛАГОВ///////////////////

//иницилизирует регистр флагов, нулевыми значениями
int register_init() {
	register_flags = 0;
	
    return 0;
}

//устанавливает значение(bool) указанному регистру флагов(reg)
int register_set(short int reg, bool value) {
	//если регистр вышел за границы, возвращаем -1
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
	//если регистр вышел за границы, возвращаем -1
	if (reg < 0 || reg >= REGISTER_SIZE) {
    	return -1;
	}

    *value = register_flags & (1 << reg);
    
    return 0;
}
