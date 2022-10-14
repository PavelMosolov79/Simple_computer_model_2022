#include <signal.h>

#include "UI.h"
#include "simple_computer.h"
#include "read_key.h"
#include "CU.h"

void signal_handler_run(int signal);

int main() {
	ui_initial();
	
	signal(SIGALRM, signal_handler_run);

	memory_load((char*)"simple_assembler/out.o");
	
	keys key;
	
	do {
		ui_update();
		read_key(&key);
		
		bool ignor;
		register_get(IGNORING_TACT_PULSES, &ignor);
		
		if (ignor) {
			switch (key) {
				case keys::KEY_UP:
					move_current_memory_pointer(keys::KEY_UP);
					break;
				
				case keys::KEY_DOWN:
					move_current_memory_pointer(keys::KEY_DOWN);
					break;
				
				case keys::KEY_LEFT:
					move_current_memory_pointer(keys::KEY_LEFT);
					break;
				
				case keys::KEY_RIGHT:
					move_current_memory_pointer(keys::KEY_RIGHT);
					break;
					
				case keys::KEY_L:
					load_memory_from_file();
					break;
					
				case keys::KEY_S:
					save_memory_in_file();
					break;
					
				case keys::KEY_I:
					ui_initial();
					break;
					
				case keys::KEY_R:
					instruction_counter = 0;
					register_set(IGNORING_TACT_PULSES, false);
					raise(SIGALRM);
	                break;
	                
	            case keys::KEY_T:
	            	CU();
	            	instruction_counter++;
	                break;
					
				case keys::KEY_F6:
					set_instruction_counter();
					break;
				
				default:
					continue;
			}
		}
		
		key = KEY_OTHER;
		
	} while(key != keys::KEY_ESC);
	
	return 0;
}

void signal_handler_run(int signal) {
	ui_update();
	
	if (CU()) {
		register_set(IGNORING_TACT_PULSES, true);
		ui_update();
		alarm(0);
	}
			
	bool ignor;
	register_get(IGNORING_TACT_PULSES, &ignor);
			
	if ((instruction_counter >= 0 && instruction_counter < 99) && !ignor) {
		instruction_counter++;
	} else if (instruction_counter >= 99) {
		instruction_counter = 0;
	}
	
	if (!ignor) {
		alarm(1);
	}
}
