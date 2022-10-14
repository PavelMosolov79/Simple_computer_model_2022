#pragma once

#include "../commands.h"
#include "simple_computer.h"
#include "read_key.h"
#include "ALU.h"

int CU();	
int cu_read(short int operand);	
int cu_write(short int operand);
int cu_load(short int operand);	
int cu_store(short int operand);
int cu_jump(short int operand);	
int cu_jneg(short int operand);	
int cu_jz(short int operand);	
int cu_jns(short int operand);	
int cu_neg(short int operand);	
int cu_addc(short int operand);	

