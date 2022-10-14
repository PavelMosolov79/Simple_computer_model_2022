#include "lib_simple_basic.h"

vector<line_simple_assembler> assembler_lines;    							//��������� ������ ��� ���������� 
vector<variable_struct> variables;                                          //��������� ����� ��� ����������
map<string, short int> sb_accordance_sa;									//��������� ����������� ������� � ����������
vector<string> queue_GOTO;													//��������� ������ ��� ������� GoTo

short int current_cell_command = 0;											//����� ������, � ������� ����� �������� ������� �������
bool is_end = false;														//������ ���������� ���������� �� �����

string new_command;															//���������� ���������� ��� ������ ��������
string new_operand;															//���������� ���������� ��� ������ ��������

int read_file(const char *file_name) {										//������� ������ ������� �� �����
	ifstream file(file_name);												//����
	
    if (!file.is_open()) {													//���� �� ���������� ������� ����
        return -1;
	}
	
	vector<string> basic_code;												//��������� ������ ��� �������
	string line_code;														//������ � ������� ������������ ��� �� ����� ������ �������
	
	short int previous_number_line = -1;									//���������� ���������� �� ���������� �������� ������
	
	while(getline(file, line_code)) {										//���� � ������� �������� ������ �� �����
		basic_code.clear();													//������� ������� ��� �������
		
		split(line_code, basic_code);										//��������� ������ (10 INPUT B) �� ������ ("10", "INPUT", "B")
		
		string number_line = basic_code[0];									//������ � ������� ������ :D
		string command = basic_code[1];										//������ � ���������
		string parameters = basic_code[2];									//������ � ����������
		if (stoi(number_line) <= previous_number_line) {					//�������� ������ ������
			printf("Error! The line number must increase and must not be repeated!");
			return -1;
		}
		
		previous_number_line = stoi(number_line);							//� ���������� ������������ ����� ������, ������������ �� ������
		
		if (command == "REM") {												//���� � ������� ������� ����������
			continue;
		} else if (command == "INPUT") {									//������� ����������
			
			sb_accordance_sa[number_line] = current_cell_command;
			
			if (INPUT(parameters)) {
				return -1;
			}
		} else if (command == "OUTPUT") {									//������� ����������
			sb_accordance_sa[number_line] = current_cell_command;
			
			if (OUTPUT(parameters)) {
				return -1;
			}
		} else if (command == "END") {										//����� ���������
			sb_accordance_sa[number_line] = current_cell_command;
			
			if (END()) {
				return -1;
			}
			
			is_end = true;
		} else if (command == "LET") {										//����������� �������� ��������� ����������
			sb_accordance_sa[number_line] = current_cell_command;
			
			if (LET(line_code)) {
				return -1;
			}
		} else if (command == "IF") {										//�������
			sb_accordance_sa[number_line] = current_cell_command;
			
			if (IF(line_code)) {
				return -1;
			}
		} else if (command == "GOTO") {										//�������� ��������
			sb_accordance_sa[number_line] = current_cell_command;
			
			parameters = parameters.substr(0, 2);
			
			if (GOTO(parameters)) {
				return -1;
			}
		}
		
		line_code.clear();													//������� ������
	}

    for (auto  &i : queue_GOTO) {											//��������� ������ �� goto
        if(sb_accordance_sa.find(assembler_lines[stoi(i)].operand) == sb_accordance_sa.end()){
            printf("Invalid operand for goto. GoTo refers to a nonexistent string") ;
            return -1;
        }
        else{
            short addressOperand = sb_accordance_sa.find(assembler_lines[stoi(i)].operand)->second ;
            assembler_lines[stoi(i)].operand = convert(addressOperand);
        }
    }
	
	if (!is_end) {															//���� � ����� ���� ��� ������� "END", ������� ������
	    printf("\nError! The end is not found!");
		return -1;
	}
	
	return 0;
}

string convert(int x) {														//���������� ����, ���� ����� < 10								
	string result;
	
	if (x < 10) {
		result += "0";
	}	
	result += to_string(x);
	
	return result;
}

void split(string str, vector<string> &return_vector) {						//������� ���������� ������ (10 INPUT B) �� ������ ("10", "INPUT", "B")
    string tmp = "";
    
    for (int i = 0; i < (int)str.size(); ++i) {
        if (str[i] != ' ') {
            tmp += str[i];
        } else {
            return_vector.push_back(tmp);
            tmp = "";
        }
    }
    
    return_vector.push_back(tmp);
}

int get_variable_address(string variable) {									//������� ��������� ���������� ��� �������� ����� ���������� ���� �� ��� ���
	int id_find = -1;
		
	for (int i = 0; i < (int)variables.size(); ++i) {
		if (string(1, variable[0]) == variables[i].name) {
			id_find = i;
		}
	}
	
	variable_struct var;
	
	if (id_find != -1) {
		return variables[id_find].address;
	} else {
		var.name = string(1, variable[0]);
		var.address = 99 - (int)variables.size();
		
		variables.push_back(var);
	}
	
	return var.address;
}

int push_line_assemler() {
	line_simple_assembler str;												//������� ������ ���������
	
	if(current_cell_command < 10) {											//���������� ����� ������� �������
		str.address_cell = "0";
		str.address_cell += to_string(current_cell_command++);
	} else {
		str.address_cell = to_string(current_cell_command++);
	}
											
	str.command = new_command;													//����������� �������� ����������
	str.operand = new_operand;													//���������� ���������� 
	
	assembler_lines.push_back(str);	
	
	return 0;		
}

int INPUT(string parameters) {												//������� ��������� �������� �����
	new_command = " READ ";
	new_operand = convert(get_variable_address(parameters));
	push_line_assemler();
	
	return 0;
}

int OUTPUT(string parameters) {												//������� ��������� �������� ������
	new_command = " WRITE ";
	new_operand = convert(get_variable_address(parameters));
	push_line_assemler();
	
	return 0;
}

int END() {																	//������� ��������� ����� ���������
	new_command = " HALT ";
	new_operand = "00";
	push_line_assemler();
	

	return 0;
}

int check_priority(string math_symbol) {									//������� ��� "LET", ��������� ��������� ���������
	if (math_symbol == "*" || math_symbol == "/") {							//���� ����� ��������� ��� �������, ��������� 3
		return 3;
	} else if (math_symbol == "+" || math_symbol == "-") {					//���� ����� �������� ��� ���������, ��������� 2
		return 2;
	} else if (math_symbol == "(") {										//���� ����� ������, ��������� 1
		return 1;
	}
	
	return -1;
}

int calculate(string var_name, string polish_str) {							//������� �� ������� �������� ������ � ��� ����������
	stack<string> values;
	
	string tmp;
	
	for (int i = 0; i < (int)polish_str.size(); ++i) {
		if (polish_str[i] >= '0' && polish_str[i] <= '9') {
			for (int j = i; i < (int)polish_str.size(); ++j) {
				if (polish_str[j] != ' ') {
					tmp += polish_str[j];
				} else {
					values.push(tmp);
					tmp = "";
					i = j - 1;
					break;
				}
			}
		} else if (polish_str[i] >= 'A' && polish_str[i] <= 'Z') {
			values.push(string(1, polish_str[i]));
		} else if (polish_str[i] == ' ') {
			continue;
		} else {
			string num_2 = values.top();
			if (num_2[0] >= '0' && num_2[0] <= '9') {
				num_2 = "_reserved";
			}
			values.pop();
			string num_1 = values.top();
			if (num_1[0] >= '0' && num_1[0] <= '9') {
				num_1 = "_reserved";
			}
			values.pop();
			
			new_command = " LOAD ";
			new_operand = convert(get_variable_address(num_1));
			push_line_assemler();
			
			switch(polish_str[i]) {
				case '*':
					new_command = " MUL ";
					break;
				case '/':
					new_command = " DIVIDE ";
					break;
				case '+':
					new_command = " ADD ";
					break;
				case '-':
					new_command = " SUB ";
					break;
				default:
					cout << "ERROR" << endl;
			}
			
			new_operand = convert(get_variable_address(num_2));
			push_line_assemler();
		}
	}
	
	new_command = " STORE ";
	new_operand = convert(get_variable_address(var_name));
	push_line_assemler();
	
	for (int i = 0; i < (int)values.size(); ++i) {
		values.pop();
	}
	
	return 0;
}


int LET(string line) {														//������� ��������������� �������� ��������� ����������
	string str = line.substr(line.find("=") + 2, line.length() - line.find("=") - 2 - 1);
	
	string out_str = "";
	stack<string> operators;
	queue<string> numbers;	
	
	int start_index = 0;
	int end_index = 0;
	
	for (int i = 0; i < (int)str.size(); ++i) {
		if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'Z')) {
			start_index = i;
			end_index = str.size();
			
			for (int j = start_index; j < (int)str.size(); ++j) {
				if (str[j] == '*' || str[j] == '/' || str[j] == '+' || str[j] == '-' || str[j] == '(' || str[j] == ')') {
					end_index = j;
					break;
				}
			}
			
			numbers.push(str.substr(start_index, end_index - start_index));
		
			i = end_index - 1;
		}
	}
	
	for (int i = 0; i < (int)str.size(); ++i) {
		if (str[i] == ')') {
			while (operators.top() != "(") {
				out_str += operators.top();
				out_str += " ";
				operators.pop();
			}
			
			operators.pop();
		}
		
		if (str[i] >= 'A' && str[i] <= 'Z') {
			out_str += numbers.front();
			numbers.pop();
			
			out_str += " ";
		}
		
		
		if (str[i] >= '0' && str[i] <= '9') {
			out_str += numbers.front();
			numbers.pop();
			
			end_index = str.size();
			
			for (int j = i; j < (int)str.size(); ++j) {
				if (str[j] == '*' || str[j] == '/' || str[j] == '+' || str[j] == '-' || str[j] == '(' || str[j] == ')') {
					end_index = j;
					break;
				}
			}
			
			i = end_index - 1;
			
			out_str += " ";
		}
		
		if (str[i] == '(') {
			operators.push(string(1, str[i]));
		}
		
		if (str[i] == '*' || str[i] == '/' || str[i] == '+' || str[i] == '-') {
			if (operators.empty()) {
				operators.push(string(1, str[i]));
			} else {
				if (check_priority(operators.top()) < check_priority(string(1, str[i]))) {
					operators.push(string(1, str[i]));
				} else {
					while (!(operators.empty()) && (check_priority(operators.top()) >= check_priority(string(1, str[i])))) {
						out_str += operators.top();
						out_str += " ";
						operators.pop();
					}
					
					operators.push(string(1, str[i]));
				}
			}
		}
	}
	
	while (!operators.empty()) {
		out_str += operators.top();
		out_str += " ";
		operators.pop();
	}
	
	bool is_simple = true;

	for (int i = 0; i < (int)out_str.size(); ++i) {
		if (out_str[i] == '*' || out_str[i] == '/' || out_str[i] == '+' || out_str[i] == '-'){
			is_simple = false;
		}
	}
	
	if (is_simple) {		
		new_command = " LOAD ";
		new_operand = convert(get_variable_address(out_str));
		push_line_assemler();

		new_command = " STORE ";
		new_operand = convert(get_variable_address(string(1, line[7])));
		push_line_assemler();
		
	} 
	else {
		if (calculate(string(1, line[7]), out_str)) {
			return -1;
		}
	}
	
	return 0;
}

int IF(string line) {														//������� �������� �������
	string condition = line.substr(6, line.length());
	
	string value_1 = condition.substr(0, condition.find(" "));
	string sign = condition.substr(condition.find(" ") + 1, 1);
	string value_2 = condition.substr(condition.find(sign) + 2, condition.find(" ", condition.find(sign) + 2) - condition.find(sign) - 2);
	
	line_simple_assembler str;
	
	if (sign == "=") {
		new_command = " LOAD ";
		new_operand = convert(get_variable_address(value_1));
		push_line_assemler();
		
		new_command = " SUB ";
		new_operand = convert(get_variable_address(value_2));
		push_line_assemler();
		
		new_command = " JZ ";
		new_operand = convert(current_cell_command + 1);
		push_line_assemler();
		
	} else if (sign == "<") {
		new_command = " LOAD ";
		new_operand = convert(get_variable_address(value_1));
		push_line_assemler();
		
		new_command = " SUB ";
		new_operand = convert(get_variable_address(value_2));
		push_line_assemler();
		
		new_command = " JNEG ";
		new_operand = convert(current_cell_command + 1);
		push_line_assemler();
		
	} else if (sign == ">") {
		new_command = " LOAD ";
		new_operand = convert(get_variable_address(value_2));
		push_line_assemler();	
		
		new_command = " SUB ";
		new_operand = convert(get_variable_address(value_1));
		push_line_assemler();

		new_command = " JNEG ";
		new_operand = convert(current_cell_command + 2);
		push_line_assemler();
		
	} else {
		printf("ERROR! Invalid sign in IF!");
		return -1;
	}
	
	new_command = " JUMP ";
	new_operand = convert(current_cell_command + 2);
	push_line_assemler();
	
	string expression = condition.substr(condition.find(" ", condition.find(value_2)) + 1, condition.length());
	string command = expression.substr(0, expression.find(" "));
	string parameters = expression.substr(expression.find(" ") + 1 , expression.length() - expression.find(" ") - 2);	
	
	if (command == "INPUT") {
		if (INPUT(parameters)) {
			return -1;
		}
	} else if (command == "OUTPUT") {
		if (OUTPUT(parameters)) {
			return -1;
		}
	} else if (command == "END") {
		if (END()) {
			return -1;
		}
		
		is_end = true;
	} else if (command == "LET") {
			//LET(count, line_code);
	} else if (command == "GOTO") {
		if (GOTO(parameters)) {
			return -1;
		}
	} else {
		printf("ERROR! Invalid command!");
		return -1;
	}
	
	return 0;
}
	
int GOTO(string parameters) {												//������� �����������(������ ������)
	line_simple_assembler str;
	
	if (sb_accordance_sa.find(parameters) == sb_accordance_sa.end()) {	
		new_command = " JUMP ";
		new_operand = parameters;
		push_line_assemler();
		
		queue_GOTO.push_back(convert(assembler_lines.size() - 1));
	} else {
		short int address_operand = sb_accordance_sa.find(parameters)->second;

		new_command = " JUMP ";
		new_operand = convert(address_operand);
		push_line_assemler();
	}
	
	return 0;
}

void write(const char *file_name) {											//������ � ����
    std::ofstream outFile(file_name) ;

    for (auto &i: assembler_lines)										    // ������ ������
        outFile << i.address_cell << i.command << i.operand << std::endl ;	
    
    for (int i = variables.size() - 1 ; i >= 0 ; i--) {					    // ������ ����������
    	if(variables[i].name != "1" && variables[i].name != "_") {
            outFile << variables[i].address << " = " << "0" << endl;
		} else if(variables[i].name == "_") {
			outFile << variables[i].address << " = " << "1" << endl;
		} else {
			outFile << variables[i].address << " = " << variables[i].name << endl;
		}
    }
            
    outFile.close();
}
