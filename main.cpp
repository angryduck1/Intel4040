#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Emulator.hpp"

using namespace std;

int main() {
	Emulator em;

	ifstream inFile("main.i4040");

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

		if (line.find("jcn") != string::npos || line.find("isz") != string::npos) {
			count_commands_real += 3;
			real_commands[count_commands] = count_commands_real;
		}
		else if (line.find("nop") != string::npos || line.find("hlt") != string::npos || line.find("clc") != string::npos || line.find("stc") != string::npos || line.find("ret") != string::npos || line.find("iac") != string::npos || line.find("dac") != string::npos) {
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

	//

	cout << "RR0-RR1 " << em.get_rr0() << " " << em.get_rr1() << " ACC " << em.get_acc() << endl;
	cout << "RR2-RR3 " << em.get_rr2() << " " << em.get_rr3() << " CARRY " << em.get_carry() << endl;
	cout << "RR4-RR5 " << em.get_rr4() << " " << em.get_rr5() << endl;
	cout << "RR6-RR7 " << em.get_rr6() << " " << em.get_rr7() << endl;
	cout << "RR8-RR9 " << em.get_rr8() << " " << em.get_rr9() << endl;
	cout << "RR10-RR11 " << em.get_rr10() << " " << em.get_rr11() << endl;
	cout << "RR12-RR13 " << em.get_rr12() << " " << em.get_rr13() << endl;
	cout << "RR14-RR15 " << em.get_rr14() << " " << em.get_rr15() << endl;
	cout << "RR16-RR17 " << em.get_rr16() << " " << em.get_rr17() << endl;
	cout << "RR18-RR19 " << em.get_rr18() << " " << em.get_rr19() << endl;
	cout << "RR20-RR21 " << em.get_rr20() << " " << em.get_rr21() << endl;
	cout << "RR22-RR23 " << em.get_rr22() << " " << em.get_rr23() << endl;

	//

	return 0;
}