#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Emulator.hpp"
#include "HelpFunctions.hpp"

using namespace std;

int main() {
	Emulator em;

	ifstream inFile("main.asm");

	string line;
	string asm_code;

	while (getline(inFile, line)) {
		auto comment_pos = line.find(";");
		auto point_pos = line.find(":");
		if (comment_pos != string::npos) {
			line = line.substr(0, comment_pos);
			asm_code += line = "\n";
			++count_commands;
		}
		else if (point_pos != string::npos) {
			auto label = line.substr(0, line.find(":"));
			if (commands.find(label) == commands.end()) {
				++count_commands;
				commands[label] = count_commands;
				count_commands_real += 1;
				real_commands[count_commands] = count_commands_real;
				asm_code += label + "\n";
				continue;
			}
		}
		else {
			asm_code += line + "\n";
			++count_commands;
		}

		if (line.find("jcn") != string::npos) {
			count_commands_real += 3;
			real_commands[count_commands] = count_commands_real;
		}
		else if (line.find("nop") != string::npos || line.find("hlt") != string::npos || line.find("clc") != string::npos || line.find("stc") != string::npos) {
			count_commands_real += 1;
			real_commands[count_commands] = count_commands_real;
		}
		else {
			count_commands_real += 2;
			real_commands[count_commands] = count_commands_real;
		}
	}

	em.load_program(em.assemble(asm_code));
	em.run();

	cout << em.get_acc() << endl;;

	return 0;
}