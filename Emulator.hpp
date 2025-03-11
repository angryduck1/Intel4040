#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

//

unsigned pre_pc = 0;
unsigned label_support = 0;
map<string, unsigned> commands;
map<unsigned, unsigned> commands_support;

//

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

	jcn = 0x01,
	fim = 0x02,
	fin = 0x03,
	jun = 0x04,
	jms = 0x05,
	inc = 0x06,
	isz = 0x07,
	add = 0x08,
	sub = 0x09,
	ld = 0xA,
	xch = 0xB,
	bbl = 0xC,
	ldm = 0xD,
	clb = 0xF,
	clc = 0xF1,
	iac = 0xF2,
	dac = 0xF8,
	stc = 0xFA,

	hlt = 0x010,
};

map<string, Assembler> opcode_map = {
	{"clc", clc},
	{"hlt", hlt},
	{"add", add},
	{"ldm", ldm},
	{"jun", jun},
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
					int16_t address = commands[token] & 0xFFF;

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
				else if (token.find("nop") != string::npos || token.find("hlt") != string::npos || token.find("clc") != string::npos || token.find("stc") != string::npos || token.find("iac") != string::npos || token.find("dac") != string::npos || token.find("clb") != string::npos) {
					instructions.push_back(opcode_map[token]);
				}
				else if (token.find("jun") != string::npos || token.find("jms") != string::npos) {
					int8_t value = opcode_map[token];
					instructions.push_back(value);

					stream >> token;
					int16_t address = commands[token] & 0xFFF;

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
		rom.resize(70);
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
		uint8_t opcode_4_bit = (opcode << 4);
		uint8_t opcode_8_bit = opcode;

		uint8_t value_4_bit = (opcode >> 4) & 0xF;

		while (is_halted != true && p.pc < instructions.size()) {

			opcode_4_bit = opcode >> 4;
			opcode_8_bit = opcode;
			value_4_bit = opcode & 0xF;

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
						if (carry == false) {
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

				rr[r_pair] = value & 0xF;
				rr[r_pair + 1] = (value >> 4) & 0xF;

				break;
			}
			case bbl: {
				a.ac = rom[++p.pc];

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
				rr[value] &= 0xF;

				if (++result > 15) {
					carry = true;
				}
				else {
					carry = false;
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
			default:
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
			case jun: {
				int address = rom[++p.pc] - 1;

				p.pc = address;

				break;
			}
			case jms: {
				stack.push_back(p.pc + 1);

				int value = rom[++p.pc] - 1;

				p.pc = value;

				break;
			}
			}

			opcode = rom[++p.pc];
		}

		return 0;
	}
};