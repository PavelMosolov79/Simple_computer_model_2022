#include "ALU.h"

int alu_add(short int accumulator_tmp, short int value);
int alu_sub(short int accumulator_tmp, short int value);
int alu_divide(short int accumulator_tmp, short int value);
int alu_mul(short int accumulator_tmp, short int value);

int ALU(short int command,  short int operand) {
	short int accumulator_tmp;
    short int value;
    
    memory_get(operand, &value);
    accumulator_tmp = accumulator;
    
    switch (command) {
        case ADD:
            alu_add(accumulator_tmp, value);
            break;
            
        case SUB:
        	alu_sub(accumulator_tmp, value);
            break;
        
        case DIVIDE:
        	alu_divide(accumulator_tmp, value);
            break ;
        
        case MUL:
        	alu_mul(accumulator_tmp, value);
            break;
        default:
        	register_set(INCORRECT_COMMAND, true);
        	return -1;
    }

    return 0 ;
}

							
int alu_add(short int accumulator_tmp, short int value) {
	short int value_calc = 0;
	short int accumulator_calc = 0;
	
    if ((value >> 15) & 1) {
    	value_calc = -(value & 0x3FFF);
	} else {
		value_calc = value & 0x3FFF;
	}
	
	if ((accumulator_tmp >> 15) & 1) {
    	accumulator_calc = -(accumulator_tmp & 0x3FFF);
	} else {
		accumulator_calc = accumulator_tmp & 0x3FFF;
	}
	
	short int result = value_calc + accumulator_calc;
	
	if (result > 0x3FFF || -result > 0x3FFF) {
		register_set(OVERFLOW, true);
		return -1;
	}
	
	if (result < 0) {
		result |= (11 << 14);
	} else {
		result |= (1 << 14);
	}
	
	accumulator = result;
	
	return 0;
}

int alu_sub(short int accumulator_tmp, short int value) {
	short int value_calc = 0;
	short int accumulator_calc = 0;
	
    if ((value >> 15) & 1) {
    	value_calc = -(value & 0x3FFF);
	} else {
		value_calc = value & 0x3FFF;
	}
	
	if ((accumulator_tmp >> 15) & 1) {
    	accumulator_calc = -(accumulator_tmp & 0x3FFF);
	} else {
		accumulator_calc = accumulator_tmp & 0x3FFF;
	}
	
	short int result = accumulator_calc - value_calc;
	
	if(result < 0)
	{
		result = ~result + 1;
		result |= (11 << 14);
	}
	
	if (result > 0x3FFF || -result > 0x3FFF) {
		register_set(OVERFLOW, true);
		return -1;
	}
	
	result |= (1 << 14);
	
	accumulator = result;
	
	return 0;
}

int alu_divide(short int accumulator_tmp, short int value) {
	short int value_calc = 0;
	short int accumulator_calc = 0;
	
    if ((value >> 15) & 1) {
    	value_calc = -(value & 0x3FFF);
	} else {
		value_calc = value & 0x3FFF;
	}
	
	if (value_calc == 0) {
		register_set(DIVISION_BY_ZERO, true);
		return -1;
	}
	
	if ((accumulator_tmp >> 15) & 1) {
    	accumulator_calc = -(accumulator_tmp & 0x3FFF);
	} else {
		accumulator_calc = accumulator_tmp & 0x3FFF;
	}
	
	short int result = accumulator_calc / value_calc;
	
	if (result > 0x3FFF || -result > 0x3FFF) {
		register_set(OVERFLOW, true);
		return -1;
	}
	
	if (result < 0) {
		result |= (11 << 14);
	} else {
		result |= (1 << 14);
	}
	
	accumulator = result;
	
	return 0;
}

int alu_mul(short int accumulator_tmp, short int value) {
	short int value_calc = 0;
	short int accumulator_calc = 0;
	
    if ((value >> 15) & 1) {
    	value_calc = -(value & 0x3FFF);
	} else {
		value_calc = value & 0x3FFF;
	}
	
	if ((accumulator_tmp >> 15) & 1) {
    	accumulator_calc = -(accumulator_tmp & 0x3FFF);
	} else {
		accumulator_calc = accumulator_tmp & 0x3FFF;
	}
	
	short int result = accumulator_calc * value_calc;
	
	if (result > 0x3FFF || -result > 0x3FFF) {
		register_set(OVERFLOW, true);
		return -1;
	}
	
	if (result < 0) {
		result |= (11 << 14);
	} else {
		result |= (1 << 14);
	}
	
	accumulator = result;
	
	return 0;
}
