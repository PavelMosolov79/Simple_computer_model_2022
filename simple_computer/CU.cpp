#include "CU.h"

// функци€ работы устройства управлени€
int CU() {
	/*  ќћѕќЌ≈Ќ“џ: */
	
	short int current_cell;				// содержимое текущей €чейки пам€ти
	short int command = 0;				// команда
	short int operand = 0;				// операнд
	
	
	memory_get(instruction_counter, &current_cell);		// получаем содержимое €чейки пам€ти
	
	// пробуем декодировать содержимое €чейки пам€ти, если не получаетс€ то выходим
	if (command_decode(current_cell, &command, &operand)) {
		return -1;
	}
    
    // выбор действи€ в зависимости от команды
    switch (command) {
    	// если команда "READ"
		case READ:
			if (cu_read(operand)) {
				return -1;
			}
			
            break;
        
        // если команда "WRITE"
        case WRITE:
            if (cu_write(operand)) {
            	return -1;
			}
			
            break;
        
        // если команда "LOAD"
        case LOAD:
        	if (cu_load(operand)) {
        		return -1;
			}
			
            break;
            
        // если команда "STORE"
		case STORE:
			if (cu_store(operand)) {
				return -1;
			}
			
			break;
        
        // если команда "JUMP"
        case JUMP:
        	if (cu_jump(operand)) {
        		return -1;
			}
			
			break;
        
        // если команда "JNEG"
        case JNEG:
        	if (cu_jneg(operand)) {
        		return -1;
			}
			
			break;
            
        // если команда "JZ"
        case JZ:
        	if (cu_jz(operand)) {
        		return -1;
			}
			
			break;
		
		// если команда "HALT"
		case HALT:
			return -1;
			
			break;
			
		// если команда "JNS"
		case JNS:
        	if (cu_jns(operand)) {
        		return -1;
			}
			
			break;
		
		// если команда "NEG"	
		case NEG:
        	if (cu_neg(operand)) {
        		return -1;
			}
			
			break;
		
		// если команда "ADDC"	
		case ADDC:
        	if (cu_addc(operand)) {
        		return -1;
			}
			
			break;
			
		// дл€ команд ALU
        default:
        	if (ALU(command, operand)) {
        		return -1;
			}
			
			break;
	}
	
	return 0;		
}

int cu_read(short int operand) {
	terminal_restore();		// восстанавливаем настройки терминала дл€ ввода
	
	printf("\nCell %d, input value: ", operand);
	
	char string_num[14]; // строка ввода
	fgets(string_num, 14, stdin);	 // получение строки из ввода
    
    change_terminal_regime(false, 30, 0, false, false);	// мен€ем режим терминала обратно
    
    int result = 0;	// переменна€ результата
    
	bool is_minus = false;	//есть ли в введенной строке минус
	if(string_num[0] == '-') //если первый знак во введЄнной строке - минус
	{
		result |= 11 << 14; //поставим единицу в признак команды и единицу, так как минус
		is_minus = true; // минус есть
	}
	else if(string_num[0] != '+') // если первый знак - число
	{
		result |= 1 << 14; // поставим единицу в признак команды
	}
	
	int number = strtol(&string_num[0], NULL, 16);	//перевод строки в число в 16 системе счислени€
	
	if(number > 16383 || number < -16383)	//14 единиц, выход за границы
	{
		register_set(OVERFLOW, true); // устанавливаем флаг - переполнение
		number = 0;	//обнул€ем значение
	}
	
	if(is_minus) // если число отрицательное
	{
		number = ~number + 1; //декодируем его в нормальное
	}
	
	result |= number; // и сложим с результатом
    
    if(memory_set(operand, (short int)result)) //запишем значение в пам€ть
    {
    	return -1;
	}
	
	return 0;
}

int cu_write(short int operand) {
	short int value;	// значение, хран€щаес€ в €чейки пам€ти
	
	// пробуем получить значение в €чейки пам€ти, если не получилось - выходим
	if (memory_get(operand, &value)) {	
		return -1;
	}
	
	printf("Value int cell [%d] is ", operand);	// вывод сообщени€ о значении €чейки пам€ти
	
	
	/* ЅЋќ  ќЅ–јЅќ“ » ѕќЋ”„≈ЌЌќ√ќ «Ќј„≈Ќ»я: */
	
	// если число отрицательное или положительное число
    if ((value >> 14) & 1) {
		// если отрицательное число							
    	if ((value >> 15) & 1) {
    		printf("-%4x", (~(value - 1)) & 0x3FFF);	// декодируем и выводим со знаком '-'
		} else {	// если положительное число
			printf("%4x", value & 0x1FFF);				// выдел€ем и выводим 
		}
	} else {	// если команда
		printf("+%4x", value);
	}
    
	return 0;
}

int cu_load(short int operand) {
	short int value;	// значение, хран€щаес€ в €чейки пам€ти
	
	// пробуем получить значение в €чейки пам€ти, если не получилось - выходим
	if (memory_get(operand, &value)) {	
		return -1;
	}
	
	// если в €чейки пам€ти лежала команда
	if (!((value >> 14) & 1)) {
		register_set(INCORRECT_COMMAND, true);	// устанавливаем флаг о Ќ≈ ќ––≈ “Ќќ…  ќћјЌƒ≈
		
		return -1;
	}
	
	accumulator = value;		// загружаем значение в аккумул€тор
    
    return 0;
}

int cu_store(short int operand) {
	// пробуем установить в €чейку пам€ти значение из аккумул€тора, если не получилось - выходим
	if (memory_set(operand, accumulator)) {
		return -1;
	}
	
    return 0;
}

int cu_jump(short int operand) {
	// если переход к €чейке пам€ти, выход€щий за пределы пам€ти
	if (operand > 99) {
		register_set(OUT_OF_MEMORY, true);	// устанавливаем флаг о ¬џ’ќƒ≈ «ј ѕјћя“№
		
		return -1;
	}
	
    instruction_counter = operand - 1;	// измен€ем instruction counter (переходим к нужной €чейке)
    
	return 0;
}

int cu_jneg(short int operand) {
	// если в аккумул€торе не команада и число отрицательное
	if (((accumulator >> 14) & 1) && ((accumulator >> 15) & 1)) {
		// если переход к €чейке пам€ти, выход€щий за пределы пам€ти
		if (operand > 99) {
			register_set(OUT_OF_MEMORY, true);	// устанавливаем флаг о ¬џ’ќƒ≈ «ј ѕјћя“№
			
			return -1;
		} else {
			instruction_counter = operand - 1;	// измен€ем instruction counter (переходим к нужной €чейке)
		}
	}
	
    return 0;
}

int cu_jz(short int operand) {
	// если в аккумул€торе не команада и в аккумул€торе 0
	if (((accumulator >> 14) & 1) && ((accumulator & 0x3FFF) == 0)) {
		// если переход к €чейке пам€ти, выход€щий за пределы пам€ти
		if (operand > 99) {
			register_set(OUT_OF_MEMORY, true);	// устанавливаем флаг о ¬џ’ќƒ≈ «ј ѕјћя“№
			
			return -1;
		} else {
			instruction_counter = operand - 1;	// измен€ем instruction counter (переходим к нужной €чейке)
		}
	}
	
    return 0;
}

int cu_jns(short int operand) {
	// если в аккумул€торе не команада и число положительное
	if (((accumulator >> 14) & 1) && !((accumulator >> 15) & 1)) {
		// если переход к €чейке пам€ти, выход€щий за пределы пам€ти
		if (operand > 99) {
			register_set(OUT_OF_MEMORY, true);	// устанавливаем флаг о ¬џ’ќƒ≈ «ј ѕјћя“№
			
			return -1;
		} else {
			instruction_counter = operand - 1;	// измен€ем instruction counter (переходим к нужной €чейке)
		}	
	}
	
    return 0;
}

//ѕолучение дополнительного кода содержимого указанной €чейки пам€ти (результат в аккумул€торе)
int cu_neg(short int operand) {
	
	short int cell_value = 0;
	// получаем значение из €чейки пам€ти
	if (memory_get(operand, &cell_value)) {
		return -1;
	}
	
	if(cell_value > 16383 || cell_value < -16383)	//14 единиц, выход за границы
	{
		return -1;
	}
	
	if(cell_value >= 0)	//если число положительное
	{
		cell_value |= 1 << 14;	//бит числа
		accumulator = cell_value;	//аккумул€тор
	}
	else
	{	// int уже хранит отрицательное значение в дополнительном коде
		cell_value |= 1 << 14;	//бит числа
		cell_value |= 1 << 15;	//бит минуса
		accumulator = cell_value;	// аккумул€тор
	}

    return 0;
}

//—ложение содержимого указанной €чейки пам€ти с €чейкой пам€ти, адрес которой находитс€ в аккумул€торе (результат в аккумул€торе)
int cu_addc(short int operand) {
	
	short int result = 0;
	
	short int cell_value = 0;
	// получаем значение из €чейки пам€ти
	if (memory_get(operand, &cell_value)) {
		return -1;
	}
	
	// декодируем аккумул€тор и получаем адрес €чейки (операнд)
	short int acc_command = 0, acc_operand = 0;
	command_decode(accumulator, &acc_command, &acc_operand);
	if (acc_operand < 0 || acc_operand > 99) // провер€ем его границы
	{
		return -1;
	}
	
	short int acc_value = 0;
	if (memory_get(acc_operand, &acc_value)) { // получаем значение €чейки из адреса аккумул€тора
		return -1;
	}
	
	result = cell_value + acc_value; // делаем сложение
	if(result > 16383 || result < -16383)	//14 единиц, выход за границы сложением
	{
		register_set(OVERFLOW, true);
		return -1;
	}
	
	if(result < 0) //если отрицательное - ставим бит минуса
	{
		result |= 1 << 15;	
	}
	
	result |= 1 << 14; // бит числа
	
	accumulator = result; // значение в аккумул€тор
	
    return 0;
}
