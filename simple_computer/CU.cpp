#include "CU.h"

// ������� ������ ���������� ����������
int CU() {
	/* ����������: */
	
	short int current_cell;				// ���������� ������� ������ ������
	short int command = 0;				// �������
	short int operand = 0;				// �������
	
	
	memory_get(instruction_counter, &current_cell);		// �������� ���������� ������ ������
	
	// ������� ������������ ���������� ������ ������, ���� �� ���������� �� �������
	if (command_decode(current_cell, &command, &operand)) {
		return -1;
	}
    
    // ����� �������� � ����������� �� �������
    switch (command) {
    	// ���� ������� "READ"
		case READ:
			if (cu_read(operand)) {
				return -1;
			}
			
            break;
        
        // ���� ������� "WRITE"
        case WRITE:
            if (cu_write(operand)) {
            	return -1;
			}
			
            break;
        
        // ���� ������� "LOAD"
        case LOAD:
        	if (cu_load(operand)) {
        		return -1;
			}
			
            break;
            
        // ���� ������� "STORE"
		case STORE:
			if (cu_store(operand)) {
				return -1;
			}
			
			break;
        
        // ���� ������� "JUMP"
        case JUMP:
        	if (cu_jump(operand)) {
        		return -1;
			}
			
			break;
        
        // ���� ������� "JNEG"
        case JNEG:
        	if (cu_jneg(operand)) {
        		return -1;
			}
			
			break;
            
        // ���� ������� "JZ"
        case JZ:
        	if (cu_jz(operand)) {
        		return -1;
			}
			
			break;
		
		// ���� ������� "HALT"
		case HALT:
			return -1;
			
			break;
			
		// ���� ������� "JNS"
		case JNS:
        	if (cu_jns(operand)) {
        		return -1;
			}
			
			break;
		
		// ���� ������� "NEG"	
		case NEG:
        	if (cu_neg(operand)) {
        		return -1;
			}
			
			break;
		
		// ���� ������� "ADDC"	
		case ADDC:
        	if (cu_addc(operand)) {
        		return -1;
			}
			
			break;
			
		// ��� ������ ALU
        default:
        	if (ALU(command, operand)) {
        		return -1;
			}
			
			break;
	}
	
	return 0;		
}

int cu_read(short int operand) {
	terminal_restore();		// ��������������� ��������� ��������� ��� �����
	
	printf("\nCell %d, input value: ", operand);
	
	char string_num[14]; // ������ �����
	fgets(string_num, 14, stdin);	 // ��������� ������ �� �����
    
    change_terminal_regime(false, 30, 0, false, false);	// ������ ����� ��������� �������
    
    int result = 0;	// ���������� ����������
    
	bool is_minus = false;	//���� �� � ��������� ������ �����
	if(string_num[0] == '-') //���� ������ ���� �� �������� ������ - �����
	{
		result |= 11 << 14; //�������� ������� � ������� ������� � �������, ��� ��� �����
		is_minus = true; // ����� ����
	}
	else if(string_num[0] != '+') // ���� ������ ���� - �����
	{
		result |= 1 << 14; // �������� ������� � ������� �������
	}
	
	int number = strtol(&string_num[0], NULL, 16);	//������� ������ � ����� � 16 ������� ���������
	
	if(number > 16383 || number < -16383)	//14 ������, ����� �� �������
	{
		register_set(OVERFLOW, true); // ������������� ���� - ������������
		number = 0;	//�������� ��������
	}
	
	if(is_minus) // ���� ����� �������������
	{
		number = ~number + 1; //���������� ��� � ����������
	}
	
	result |= number; // � ������ � �����������
    
    if(memory_set(operand, (short int)result)) //������� �������� � ������
    {
    	return -1;
	}
	
	return 0;
}

int cu_write(short int operand) {
	short int value;	// ��������, ���������� � ������ ������
	
	// ������� �������� �������� � ������ ������, ���� �� ���������� - �������
	if (memory_get(operand, &value)) {	
		return -1;
	}
	
	printf("Value int cell [%d] is ", operand);	// ����� ��������� � �������� ������ ������
	
	
	/* ���� ��������� ����������� ��������: */
	
	// ���� ����� ������������� ��� ������������� �����
    if ((value >> 14) & 1) {
		// ���� ������������� �����							
    	if ((value >> 15) & 1) {
    		printf("-%4x", (~(value - 1)) & 0x3FFF);	// ���������� � ������� �� ������ '-'
		} else {	// ���� ������������� �����
			printf("%4x", value & 0x1FFF);				// �������� � ������� 
		}
	} else {	// ���� �������
		printf("+%4x", value);
	}
    
	return 0;
}

int cu_load(short int operand) {
	short int value;	// ��������, ���������� � ������ ������
	
	// ������� �������� �������� � ������ ������, ���� �� ���������� - �������
	if (memory_get(operand, &value)) {	
		return -1;
	}
	
	// ���� � ������ ������ ������ �������
	if (!((value >> 14) & 1)) {
		register_set(INCORRECT_COMMAND, true);	// ������������� ���� � ������������ �������
		
		return -1;
	}
	
	accumulator = value;		// ��������� �������� � �����������
    
    return 0;
}

int cu_store(short int operand) {
	// ������� ���������� � ������ ������ �������� �� ������������, ���� �� ���������� - �������
	if (memory_set(operand, accumulator)) {
		return -1;
	}
	
    return 0;
}

int cu_jump(short int operand) {
	// ���� ������� � ������ ������, ��������� �� ������� ������
	if (operand > 99) {
		register_set(OUT_OF_MEMORY, true);	// ������������� ���� � ������ �� ������
		
		return -1;
	}
	
    instruction_counter = operand - 1;	// �������� instruction counter (��������� � ������ ������)
    
	return 0;
}

int cu_jneg(short int operand) {
	// ���� � ������������ �� �������� � ����� �������������
	if (((accumulator >> 14) & 1) && ((accumulator >> 15) & 1)) {
		// ���� ������� � ������ ������, ��������� �� ������� ������
		if (operand > 99) {
			register_set(OUT_OF_MEMORY, true);	// ������������� ���� � ������ �� ������
			
			return -1;
		} else {
			instruction_counter = operand - 1;	// �������� instruction counter (��������� � ������ ������)
		}
	}
	
    return 0;
}

int cu_jz(short int operand) {
	// ���� � ������������ �� �������� � � ������������ 0
	if (((accumulator >> 14) & 1) && ((accumulator & 0x3FFF) == 0)) {
		// ���� ������� � ������ ������, ��������� �� ������� ������
		if (operand > 99) {
			register_set(OUT_OF_MEMORY, true);	// ������������� ���� � ������ �� ������
			
			return -1;
		} else {
			instruction_counter = operand - 1;	// �������� instruction counter (��������� � ������ ������)
		}
	}
	
    return 0;
}

int cu_jns(short int operand) {
	// ���� � ������������ �� �������� � ����� �������������
	if (((accumulator >> 14) & 1) && !((accumulator >> 15) & 1)) {
		// ���� ������� � ������ ������, ��������� �� ������� ������
		if (operand > 99) {
			register_set(OUT_OF_MEMORY, true);	// ������������� ���� � ������ �� ������
			
			return -1;
		} else {
			instruction_counter = operand - 1;	// �������� instruction counter (��������� � ������ ������)
		}	
	}
	
    return 0;
}

//��������� ��������������� ���� ����������� ��������� ������ ������ (��������� � ������������)
int cu_neg(short int operand) {
	
	short int cell_value = 0;
	// �������� �������� �� ������ ������
	if (memory_get(operand, &cell_value)) {
		return -1;
	}
	
	if(cell_value > 16383 || cell_value < -16383)	//14 ������, ����� �� �������
	{
		return -1;
	}
	
	if(cell_value >= 0)	//���� ����� �������������
	{
		cell_value |= 1 << 14;	//��� �����
		accumulator = cell_value;	//�����������
	}
	else
	{	// int ��� ������ ������������� �������� � �������������� ����
		cell_value |= 1 << 14;	//��� �����
		cell_value |= 1 << 15;	//��� ������
		accumulator = cell_value;	// �����������
	}

    return 0;
}

//�������� ����������� ��������� ������ ������ � ������� ������, ����� ������� ��������� � ������������ (��������� � ������������)
int cu_addc(short int operand) {
	
	short int result = 0;
	
	short int cell_value = 0;
	// �������� �������� �� ������ ������
	if (memory_get(operand, &cell_value)) {
		return -1;
	}
	
	// ���������� ����������� � �������� ����� ������ (�������)
	short int acc_command = 0, acc_operand = 0;
	command_decode(accumulator, &acc_command, &acc_operand);
	if (acc_operand < 0 || acc_operand > 99) // ��������� ��� �������
	{
		return -1;
	}
	
	short int acc_value = 0;
	if (memory_get(acc_operand, &acc_value)) { // �������� �������� ������ �� ������ ������������
		return -1;
	}
	
	result = cell_value + acc_value; // ������ ��������
	if(result > 16383 || result < -16383)	//14 ������, ����� �� ������� ���������
	{
		register_set(OVERFLOW, true);
		return -1;
	}
	
	if(result < 0) //���� ������������� - ������ ��� ������
	{
		result |= 1 << 15;	
	}
	
	result |= 1 << 14; // ��� �����
	
	accumulator = result; // �������� � �����������
	
    return 0;
}
