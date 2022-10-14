#include <stdio.h>
#include <stdlib.h>

int main() {
	system("make -C simple_basic");
	system("./simple_basic/lab simple_basic/input.sb simple_basic/out.sa");
	
	system("make -C simple_assembler");
	system("./simple_assembler/sat simple_basic/out.sa simple_assembler/out.o");
	
	system("make -C simple_computer");
	system("./simple_computer/computer");
	
	return 0;
}
