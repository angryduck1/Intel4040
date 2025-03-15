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
		if (line == "") {
			continue;
		}
		auto comment_pos = line.find(";");
		auto point_pos = line.find(":");
		if (comment_pos != string::npos) {
			line = line.substr(0, comment_pos);
			asm_code += line = "\n";
		}
		else if (point_pos != string::npos) {
			auto label = line.substr(0, line.find(":"));
			if (commands.find(label) == commands.end()) {
				if (pre_pc != 0) {
					++pre_pc;
				}
				commands[label] = pre_pc;
				commands_support[pre_pc] = ++label_support;
				asm_code += label + "\n";
				continue;
			}
		}
		else {
			asm_code += line + "\n";
		}

		if (line.find("jcn") != string::npos || line.find("isz") != string::npos || line.find("jun") != string::npos || line.find("jms") != string::npos || line.find("fim") != string::npos) {
			pre_pc += 2;
		}
		else {
			pre_pc += 1;
		}

		if (line.find("hlt") != string::npos || line.find("bbs") != string::npos || line.find("lcr") != string::npos || line.find("or4") != string::npos || line.find("or5") != string::npos || line.find("an6") != string::npos || line.find("an7") != string::npos || line.find("db0") != string::npos || line.find("db1") != string::npos || line.find("sb0") != string::npos || line.find("sb1") != string::npos || line.find("ein") != string::npos || line.find("din") != string::npos || line.find("rpm") != string::npos) {
			if (pre_pc == 0) {
				subgroup_command[0] = 2;
			}
			else {
				subgroup_command[pre_pc] = 2;
			}
		}
		else if (line.find("src") != string::npos || line.find("wrm") != string::npos || line.find("wmp") != string::npos || line.find("wrr") != string::npos || line.find("wpm") != string::npos || line.find("wr0") != string::npos || line.find("wr1") != string::npos || line.find("wr2") != string::npos || line.find("wr3") != string::npos || line.find("sbm") != string::npos || line.find("rdm") != string::npos || line.find("rdr") != string::npos || line.find("adm") != string::npos || line.find("rd0") != string::npos || line.find("rd1") != string::npos || line.find("rd2") != string::npos || line.find("rd3") != string::npos) {
			if (pre_pc == 0) {
				subgroup_command[0] = 3;
			}
			else {
				subgroup_command[pre_pc] = 3;
			}
		}
		else if (line.find("jcn") != string::npos || line.find("isz") != string::npos || line.find("jun") != string::npos || line.find("jms") != string::npos || line.find("fim") != string::npos) {
			if (pre_pc == 1) {
				subgroup_command[0] = 1;
			}
			else {
				subgroup_command[pre_pc - 1] = 1;
			}
		}
		else {
			if (pre_pc == 0) {
				subgroup_command[0] = 1;
			}
			else {
				subgroup_command[pre_pc] = 1;
			}
		}

	}

	em.load_program(em.assemble(asm_code));
	em.run();

	return 0;
}