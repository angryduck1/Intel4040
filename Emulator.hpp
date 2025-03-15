#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Intel4002.hpp"

using namespace std;

int16_t pre_pc = -1;
unsigned label_support = 0;
map<string, unsigned> commands;
map<unsigned, unsigned> commands_support;

map<uint8_t, uint16_t> subgroup_command;


Intel4002 ram;

enum Assembler : uint16_t {
	rr0 = 0,
	rr1,
	rr2,
	rr3,
	rr4,
	rr5,
	rr6,
	rr7,
	rr8,
	rr9,
	rr10,
	rr11,
	rr12,
	rr13,
	rr14,
	rr15,
	rr16,
	rr17,
	rr18,
	rr19,
	rr20,
	rr21,
	rr22,
	rr23,
	ac = 1,
	c2,
	c3,
	c4,

	// 1-st subgroup
	jcn = 0x1,
	fim = 0x2,
	fin = 0x3,
	jun = 0x4,
	jms = 0x5,
	inc = 0x6,
	isz = 0x7,
	add = 0x8,
	sub = 0x9,
	ld = 0xA,
	xch = 0xB,
	bbl = 0xC,
	ldm = 0xD,
	clb = 0xF,
	clc = 0xF1,
	iac = 0xF2,
	cmc = 0xF3,
	cma = 0xF4,
	ral = 0xF5,
	rar = 0xF6,
	tcc = 0xF7,
	dac = 0xF8,
	tcs = 0xF9,
	stc = 0xFA,
	daa = 0xFB,
	kbp = 0xFC,
	dcl = 0xFD,

	// 2-rd subgroup
	hlt = 0x01,
	bbs = 0x02,
	lcr = 0x03,
	or4 = 0x04,
	or5 = 0x05,
	an6 = 0x06,
	an7 = 0x07,
	db0 = 0x08,
	db1 = 0x09,
	sb0 = 0x0A,
	sb1 = 0x0B,
	ein = 0x0C,
	din = 0x0D,
	rpm = 0x0E,

	// 3-rd subgroup
	src = 0x2,
	wrm = 0xE0,
	wmp = 0xE1,
	wrr = 0xE2,
	wpm = 0xE3,
	wr0 = 0xE4,
	wr1 = 0xE5,
	wr2 = 0xE6,
	wr3 = 0xE7,
	sbm = 0xE8,
	rdm = 0xE9,
	rdr = 0xEA,
	adm = 0xEB,
	rd0 = 0xEC,
	rd1 = 0xED,
	rd2 = 0xEE,
	rd3 = 0xEF
};

map<string, Assembler> opcode_map = {
	{"clc", clc},
	{"hlt", hlt},
	{"add", add},
	{"ldm", ldm},
	{"jun", jun},
	{"src", src},
	{"xch", xch},
	{"clc", stc},
	{"sub", sub},
	{"jms", jms},
	{"bbl", bbl},
	{"stc", stc},
	{"inc", inc},
	{"iac", iac},
	{"dac", dac},
	{"jcn", jcn},
	{"isz", isz},
	{"fim", fim},
	{"ld", ld},
	{"clb", clb},
	{"wrm", wrm},
	{"wr0", wr0},
	{"wr1", wr1},
	{"wr2", wr2},
	{"wr3", wr3},
	{"rr0", rr0},
	{"rr1", rr1},
	{"rr2", rr2},
	{"rr3", rr3},
	{"rr4", rr4},
	{"rr5", rr5},
	{"rr6", rr6},
	{"rr7", rr7},
	{"rr8", rr8},
	{"rr9", rr9},
	{"rr10", rr10},
	{"rr11", rr11},
	{"rr12", rr12},
	{"rr13", rr13},
	{"rr14", rr14},
	{"rr15", rr15},
	{"rr16", rr16},
	{"rr17", rr17},
	{"rr18", rr18},
	{"rr19", rr19},
	{"rr20", rr20},
	{"rr21", rr21},
	{"rr22", rr22},
	{"rr23", rr23},
	{"c2", c2},
	{"c3", c3},
	{"c4", c4}
};

class Emulator {
public:
	vector<uint16_t> instructions;
	vector<uint8_t> rr;

	struct Accumulator {
		uint8_t ac : 4;
	};

	struct PointerCounter {
		uint16_t pc : 12;
	};

	struct Pins {
		bool test = 0;
	};

	bool carry = false;
	bool is_halted = false;

	vector<uint16_t> stack;

	vector<uint16_t> rom;

	uint8_t current_bank;
	uint8_t current_reg_ram;
	uint8_t current_cell;

	std::vector<uint16_t> assemble(const std::string& asm_code) {
		std::vector<uint16_t> instructions;
		std::istringstream stream(asm_code);
		std::string token;

		while (stream >> token) {
			if (opcode_map.find(token) != opcode_map.end()) {
				if (token.find("jcn") != string::npos || token.find("isz") != string::npos) {
					uint8_t temp_value = opcode_map[token];

					uint8_t high_bit = temp_value;
					stream >> token;

					uint8_t low_bit = opcode_map[token];
					uint8_t value = (high_bit << 4) | low_bit;

					instructions.push_back(value);

					stream >> token;
					int8_t address = commands[token];

					instructions.push_back(address);
				}
				else if (token.find("fim") != string::npos) {
					uint8_t temp_value = opcode_map[token];

					uint8_t high_bit = temp_value;
					stream >> token;

					uint8_t low_bit = opcode_map[token];
					uint8_t value = (high_bit << 4) | low_bit;

					instructions.push_back(value);

					stream >> token;
					uint8_t value_8_bit = stoi(token);

					instructions.push_back(value_8_bit);
				}
				else if (token.find("nop") != string::npos || token.find("hlt") != string::npos || token.find("clc") != string::npos || token.find("stc") != string::npos || token.find("iac") != string::npos || token.find("dac") != string::npos || token.find("clb") != string::npos || token.find("wrm") != string::npos || token.find("rdm") != string::npos || token.find("wr0") != string::npos || token.find("wr1") != string::npos || token.find("wr2") != string::npos || token.find("wr3") != string::npos) {
					instructions.push_back(opcode_map[token]);
				}
				else if (token.find("jun") != string::npos || token.find("jms") != string::npos) {
					uint8_t temp_value = opcode_map[token];

					uint8_t high_bit = temp_value;
					stream >> token;

					uint8_t low_bit = commands[token] & 0xF;
					uint8_t value = (high_bit << 4) | low_bit;

					instructions.push_back(value);

					int8_t address = commands[token] >> 4;

					instructions.push_back(address);
				}
				else if (token.find("inc") != string::npos || token.find("xch") != string::npos || token.find("add") != string::npos || token.find("sub") != string::npos || token == "ld") {
					uint8_t temp_value = opcode_map[token];

					uint8_t high_bit = temp_value;
					stream >> token;

					uint8_t low_bit = opcode_map[token];
					uint8_t value = (high_bit << 4) | low_bit;

					instructions.push_back(value);
				}
				else if (token.find("src") != string::npos) {
					uint8_t temp_value = opcode_map[token];

					uint8_t high_bit = temp_value;
					stream >> token;

					uint8_t low_bit = opcode_map[token] & 0xF;

					uint8_t value = (high_bit << 4) | low_bit;

					instructions.push_back(value);
				}
				else {
					uint8_t temp_value = opcode_map[token];

					uint8_t high_bit = temp_value;
					stream >> token;

					uint8_t low_bit = stoi(token);
					uint8_t value = (high_bit << 4) | low_bit;

					instructions.push_back(value);
				}
			}
			else {
				instructions.push_back(commands[token]);
			}
		}

		return instructions;
	}

	void load_program(vector<uint16_t> instructions) {
		rom.resize(270);
		this->instructions = instructions;

		for (size_t i = 0; i < instructions.size(); i++) {
			rom[i] = instructions[i];
		}

		for (size_t i = instructions.size(); i < rom.size(); i++) {
			rom[i] = 0;
		}
	}

	Accumulator a;
	PointerCounter p;
	Pins pin;

	int get_acc() {
		return static_cast<int>(a.ac);
	}

	bool get_carry() {
		return carry;
	}

	int get_rr0() {
		return static_cast<int>(rr[0]);
	}

	int get_rr1() {
		return static_cast<int>(rr[1]);
	}

	int get_rr2() {
		return static_cast<int>(rr[2]);
	}

	int get_rr3() {
		return static_cast<int>(rr[3]);
	}

	int get_rr4() {
		return static_cast<int>(rr[4]);
	}

	int get_rr5() {
		return static_cast<int>(rr[5]);
	}

	int get_rr6() {
		return static_cast<int>(rr[6]);
	}

	int get_rr7() {
		return static_cast<int>(rr[7]);
	}

	int get_rr8() {
		return static_cast<int>(rr[8]);
	}

	int get_rr9() {
		return static_cast<int>(rr[9]);
	}

	int get_rr10() {
		return static_cast<int>(rr[10]);
	}

	int get_rr11() {
		return static_cast<int>(rr[11]);
	}

	int get_rr12() {
		return static_cast<int>(rr[12]);
	}

	int get_rr13() {
		return static_cast<int>(rr[13]);
	}

	int get_rr14() {
		return static_cast<int>(rr[14]);
	}

	int get_rr15() {
		return static_cast<int>(rr[15]);
	}

	int get_rr16() {
		return static_cast<int>(rr[16]);
	}

	int get_rr17() {
		return static_cast<int>(rr[17]);
	}

	int get_rr18() {
		return static_cast<int>(rr[18]);
	}

	int get_rr19() {
		return static_cast<int>(rr[19]);
	}

	int get_rr20() {
		return static_cast<int>(rr[20]);
	}

	int get_rr21() {
		return static_cast<int>(rr[21]);
	}

	int get_rr22() {
		return static_cast<int>(rr[22]);
	}

	int get_rr23() {
		return static_cast<int>(rr[23]);
	}

	int get_current_reg_ram() {
		return static_cast<int>(current_reg_ram);
	}

	int get_current_cell() {
		return static_cast<int>(current_cell);
	}


	Emulator() {
		a.ac = 0;

		p.pc = 0;

		rr.resize(24);

		for (size_t i = 0; i <= 23; ++i) {
			rr[i] = 0;
		}
	}

	int run() {
		uint8_t opcode = rom[p.pc];
		uint8_t opcode_4_bit;
		uint8_t opcode_8_bit;
		uint8_t value_4_bit;
		uint8_t subgr;

		while (is_halted != true && p.pc < instructions.size()) {

			opcode_4_bit = opcode >> 4;
			opcode_8_bit = opcode;
			value_4_bit = opcode & 0xF;
			uint8_t subgr = subgroup_command[p.pc];

			if (stack.size() > 7) {
				for (size_t i = 0; i < 5; i++) {
					stack[i] = stack[i + 1];
				}
				stack.pop_back();
			}
			if (commands_support.find(p.pc) != commands_support.end()) {
				opcode = rom[++p.pc];
				continue;
			}
			switch (subgr) {
			case 1: {
				switch (opcode_4_bit) {
				case ldm: {
					a.ac = value_4_bit;

					break;
				}
				case jcn: {
					int condition = value_4_bit;
					int address = rom[++p.pc] - 1;

					if (carry == false) {
						switch (condition) {
						case c2: {
							if (a.ac == 0) {
								p.pc = address;
							}
							break;
						}
						case c3: {
							if (carry == true) {
								p.pc = address;
							}
							break;
						}
						case c4: {
							if (pin.test == 0) {
								p.pc = address;
							}
							break;
						}
						}
					}
					else {
						switch (condition) {
						case c2: {
							if (a.ac != 0) {
								p.pc = address;
							}
							break;
						}
						case c3: {
							if (carry == true) {
								p.pc = address;
							}
							break;
						}
						case c4: {
							if (pin.test == 1) {
								p.pc = address;
							}
							break;
						}
						}
					}

					break;
				}
				case isz: {
					int value = value_4_bit;
					int address = rom[++p.pc] - 1;

					++rr[value];

					rr[value] &= 0xF;

					if (rr[value] == 0) {
						p.pc = address;
					}

					break;
				}
				case fim: {
					int r_pair = value_4_bit;
					uint8_t value = rom[++p.pc];

					if (r_pair == rr0) {
						rr[r_pair] = value & 0xF;
						rr[r_pair + 1] = (value >> 4) & 0xF;
					}
					else if (r_pair > 0 && r_pair <= 11) {
						rr[r_pair + r_pair] = value & 0xF;
						rr[r_pair + r_pair + 1] = (value >> 4) & 0xF;
					}

					break;
				}
				case bbl: {
					a.ac = value_4_bit;

					p.pc = stack.back();

					stack.pop_back();

					break;
				}
				case ld: {
					a.ac = rr[value_4_bit];

					break;
				}
				case add: {
					int value = value_4_bit;
					int ac = a.ac;

					a.ac += rr[value];
					value = rr[value];

					if (ac + value > 15) {
						carry = true;
					}
					else {
						carry = false;
					}

					break;
				}
				case inc: {
					int value = value_4_bit;
					int result;

					result = rr[value];
					++rr[value];

					if (++result > 15) {
						rr[value] = 0;
					}

					break;
				}
				case xch: {
					int value = value_4_bit;

					uint8_t temp = a.ac;
					a.ac = rr[value];;
					rr[value] = temp;

					break;
				}
				case sub: {
					int value = value_4_bit;
					int8_t result = a.ac;

					a.ac -= rr[value];
					value = rr[value];

					if (result - value < 0) {
						carry = true;
					}
					else {
						carry = false;
					}

					break;
				}
				case jun: {
					uint8_t address = (rom[++p.pc]) << 4;

					p.pc = (address | value_4_bit) - 1;

					break;
				}
				case jms: {
					stack.push_back(p.pc + 1);

					uint8_t address = (rom[++p.pc]) << 4;

					p.pc = (address | value_4_bit) - 1;

					break;
				}
						break;
				}

				switch (opcode_8_bit) {
				case hlt: {
					is_halted = true;

					break;
				}
				case clb: {
					a.ac = 0;
					carry = false;

					break;
				}
				case dac: {
					uint8_t value = a.ac;
					if (--value < 0) {
						carry = true;
					}
					else {
						carry = false;
					}
					--a.ac;

					break;
				}
				case iac: {
					uint8_t value = a.ac;

					if (++value > 15) {
						carry = true;
					}
					else {
						carry = false;
					}
					++a.ac;

					break;
				}
				case clc: {
					carry = false;

					break;
				}
				case stc: {
					carry = true;

					break;
				}
				}
				break;
			}

			case 2: {
			switch (opcode_8_bit) {
			case hlt: {
				is_halted = true;

				break;
			}
			}
			break;
			}

			case 3: {
				switch (opcode_4_bit) {
				case src: {
					int r_pair = value_4_bit;

					if (r_pair == rr0) {
						current_reg_ram = (rr[r_pair]) & 0x3;
						current_cell = rr[r_pair + 1];
					}
					else if (r_pair > 0 && r_pair <= 7) {
						current_reg_ram = rr[(r_pair + r_pair)] & 0x3;
						current_cell = rr[r_pair + r_pair + 1];
					}

					break;
				}
				}
				switch (opcode_8_bit) {
				case wrm: {
					ram.ram[current_reg_ram][current_cell] = a.ac;

					break;
				}
				case rdm: {
					 a.ac = ram.ram[current_reg_ram][current_cell];

					break;
				}
				case wr0: {
					ram.ram[current_reg_ram][16] = a.ac;

					break;
				}
				case wr1: {
					ram.ram[current_reg_ram][17] = a.ac;

					break;
				}
				case wr2: {
					ram.ram[current_reg_ram][18] = a.ac;

					break;
				}
				case wr3: {
					ram.ram[current_reg_ram][19] = a.ac;

					break;
				}
				}
				break;
			}
			}

			opcode = rom[++p.pc];
		}

		cout << "RR0-RR1 " << get_rr0() << " " << get_rr1() << " ACC " << get_acc() << endl;
		cout << "RR2-RR3 " << get_rr2() << " " << get_rr3() << " CARRY " << get_carry() << endl;
		cout << "RR4-RR5 " << get_rr4() << " " << get_rr5() << endl;
		cout << "RR6-RR7 " << get_rr6() << " " << get_rr7() << endl;
		cout << "RR8-RR9 " << get_rr8() << " " << get_rr9() << endl;
		cout << "RR10-RR11 " << get_rr10() << " " << get_rr11() << endl;
		cout << "RR12-RR13 " << get_rr12() << " " << get_rr13() << endl;
		cout << "RR14-RR15 " << get_rr14() << " " << get_rr15() << endl;
		cout << "RR16-RR17 " << get_rr16() << " " << get_rr17() << endl;
		cout << "RR18-RR19 " << get_rr18() << " " << get_rr19() << endl;
		cout << "RR20-RR21 " << get_rr20() << " " << get_rr21() << endl;
		cout << "RR22-RR23 " << get_rr22() << " " << get_rr23() << endl;

		cout << endl;

		unsigned curr_reg = 0;

		for (auto& r : ram.ram) {
			cout << "REG" << curr_reg << ": ";
			for (auto i : r) {
				cout << static_cast<int>(i) << " ";
			}
			cout << endl;
			++curr_reg;
		}

		cout << endl;

		cout << "Current register_ram: " << get_current_reg_ram() << endl;
		cout << "Current cell: " << get_current_cell();

		return 0;
	}
};