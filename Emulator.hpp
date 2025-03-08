#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

//

unsigned count_commands_real = 0;
unsigned count_commands = 0;
map<unsigned, unsigned> real_commands;
map<string, unsigned> commands;

//

enum Assembler : uint16_t {
	jcn = 1000,
	fim,
	fin,
	jin,
	jun,
	jms,
	inc,
	isz,
	add,
	sub,
	ld,
	xch,
	bbl,
	ldm,
	clb,
	clc,
	hlt,
	stc,
	call,
	ret,
	iac,
	dac,

	rr0,
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
	ac,
	c2,
	c3,
	c4
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
	{"call", call},
	{"ret", ret},
	{"stc", stc},
	{"inc", inc},
	{"iac", iac},
	{"dac", dac},
	{"jcn", jcn},
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

	struct Registers {
		uint8_t rr0 : 4;
		uint8_t rr1 : 4;
		uint8_t rr2 : 4;
		uint8_t rr3 : 4;
		uint8_t rr4 : 4;
		uint8_t rr5 : 4;
		uint8_t rr6 : 4;
		uint8_t rr7 : 4;
		uint8_t rr8 : 4;
		uint8_t rr9 : 4;
		uint8_t rr10 : 4;
		uint8_t rr11 : 4;
		uint8_t rr12 : 4;
		uint8_t rr13 : 4;
		uint8_t rr14 : 4;
		uint8_t rr15 : 4;
		uint8_t rr16 : 4;
		uint8_t rr17 : 4;
		uint8_t rr18 : 4;
		uint8_t rr19 : 4;
		uint8_t rr20 : 4;
		uint8_t rr21 : 4;
		uint8_t rr22 : 4;
		uint8_t rr23 : 4;
	};

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
			try {
				if (opcode_map.find(token) != opcode_map.end()) {
					instructions.push_back(opcode_map[token]);
				}
				else {
					instructions.push_back(static_cast<uint8_t>(std::stoi(token)));
				}
			}
			catch (...) {
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

	Registers r;
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
		return static_cast<int>(r.rr0);
	}

	int get_rr1() {
		return static_cast<int>(r.rr1);
	}

	int get_rr2() {
		return static_cast<int>(r.rr2);
	}

	int get_rr3() {
		return static_cast<int>(r.rr3);
	}

	int get_rr4() {
		return static_cast<int>(r.rr4);
	}

	int get_rr5() {
		return static_cast<int>(r.rr5);
	}

	int get_rr6() {
		return static_cast<int>(r.rr6);
	}

	int get_rr7() {
		return static_cast<int>(r.rr7);
	}

	int get_rr8() {
		return static_cast<int>(r.rr8);
	}

	int get_rr9() {
		return static_cast<int>(r.rr9);
	}

	int get_rr10() {
		return static_cast<int>(r.rr10);
	}

	int get_rr11() {
		return static_cast<int>(r.rr11);
	}

	int get_rr12() {
		return static_cast<int>(r.rr12);
	}

	int get_rr13() {
		return static_cast<int>(r.rr13);
	}

	int get_rr14() {
		return static_cast<int>(r.rr14);
	}

	int get_rr15() {
		return static_cast<int>(r.rr15);
	}

	int get_rr16() {
		return static_cast<int>(r.rr16);
	}

	int get_rr17() {
		return static_cast<int>(r.rr17);
	}

	int get_rr18() {
		return static_cast<int>(r.rr18);
	}

	int get_rr19() {
		return static_cast<int>(r.rr19);
	}

	int get_rr20() {
		return static_cast<int>(r.rr20);
	}

	int get_rr21() {
		return static_cast<int>(r.rr21);
	}

	int get_rr22() {
		return static_cast<int>(r.rr22);
	}

	int get_rr23() {
		return static_cast<int>(r.rr23);
	}

	unsigned get_counter() {
		return count_commands;
	}

	unsigned get_really_counter() {
		return count_commands_real;
	}

	Emulator() {
		a.ac = 0;

		r.rr0 = 0;
		r.rr1 = 0;
		r.rr2 = 0;
		r.rr3 = 0;
		r.rr4 = 0;
		r.rr5 = 0;
		r.rr6 = 0;
		r.rr7 = 0;
		r.rr8 = 0;
		r.rr9 = 0;
		r.rr10 = 0;
		r.rr11 = 0;
		r.rr12 = 0;
		r.rr13 = 0;
		r.rr14 = 0;
		r.rr15 = 0;

		p.pc = 0;
	}

	int run() {
		uint16_t opcode = rom[p.pc];
		while (is_halted != true && p.pc < instructions.size()) {
			if (stack.size() > 7) {
				for (size_t i = 0; i < 5; i++) {
					stack[i] = stack[i + 1];
				}
				stack.pop_back();
			}
			switch (opcode) {
			case hlt: {
				is_halted = true;

				break;
			}
			case ldm: {
				if (rom[++p.pc] > 15) {
					carry = true;
				}
				a.ac = rom[p.pc];

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
				int address = real_commands[rom[++p.pc]] - 1;

				p.pc = address;

				break;
			}
			case jcn: {
				int condition = rom[++p.pc];
				int address = real_commands[rom[++p.pc]] - 1;

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
			case call: {
				stack.push_back(p.pc + 1);

				int value = real_commands[rom[++p.pc]] - 1;

				p.pc = value;

				break;
			}
			case ret: {
				p.pc = stack.back();

				stack.pop_back();

				break;
			}
			case add: {
				int value = rom[++p.pc];
				int ac = a.ac;

				switch (value) {
				case rr0: {
					value = r.rr0;
					a.ac += r.rr0;
					break;
				}
				case rr1: {
					value = r.rr1;
					a.ac += r.rr1;
					break;
				}
				case rr2: {
					value = r.rr2;
					a.ac += r.rr2;
					break;
				}
				case rr3: {
					value = r.rr3;
					a.ac += r.rr3;
					break;
				}
				case rr4: {
					value = r.rr4;
					a.ac += r.rr4;
					break;
				}
				case rr5: {
					value = r.rr5;
					a.ac += r.rr5;
					break;
				}
				case rr6: {
					value = r.rr6;
					a.ac += r.rr6;
					break;
				}
				case rr7: {
					value = r.rr7;
					a.ac += r.rr7;
					break;
				}
				case rr8: {
					value = r.rr8;
					a.ac += r.rr8;
					break;
				}
				case rr9: {
					value = r.rr9;
					a.ac += r.rr9;
					break;
				}
				case rr10: {
					value = r.rr10;
					a.ac += r.rr10;
					break;
				}
				case rr11: {
					value = r.rr11;
					a.ac += r.rr11;
					break;
				}
				case rr12: {
					value = r.rr12;
					a.ac += r.rr12;
					break;
				}
				case rr13: {
					value = r.rr13;
					a.ac += r.rr13;
					break;
				}
				case rr14: {
					value = r.rr14;
					a.ac += r.rr14;
					break;
				}
				case rr15: {
					value = r.rr15;
					a.ac += r.rr15;
					break;
				}
				case rr16: {
					value = r.rr16;
					a.ac += r.rr16;
					break;
				}
				case rr17: {
					value = r.rr17;
					a.ac += r.rr17;
					break;
				}
				case rr18: {
					value = r.rr18;
					a.ac += r.rr18;
					break;
				}
				case rr19: {
					value = r.rr19;
					a.ac += r.rr19;
					break;
				}
				case rr20: {
					value = r.rr20;
					a.ac += r.rr20;
					break;
				}
				case rr21: {
					value = r.rr21;
					a.ac += r.rr21;
					break;
				}
				case rr22: {
					value = r.rr22;
					a.ac += r.rr22;
					break;
				}
				case rr23: {
					value = r.rr23;
					a.ac += r.rr23;
					break;
				}
				}

				if (ac + value > 15) {
					carry = true;
				}
				else {
					carry = false;
				}

				break;
			}
			case inc: {
				int value = rom[++p.pc];
				int result;

				switch (value) {
				case rr0: {
					result = r.rr0;
					++r.rr0;
					break;
				}
				case rr1: {
					result = r.rr1;
					++r.rr1;
					break;
				}
				case rr2: {
					result = r.rr2;
					++r.rr2;
					break;
				}
				case rr3: {
					result = r.rr3;
					++r.rr4;
					break;
				}
				case rr4: {
					result = r.rr4;
					++r.rr4;
					break;
				}
				case rr5: {
					result = r.rr5;
					++r.rr5;
					break;
				}
				case rr6: {
					result = r.rr6;
					++r.rr6;
					break;
				}
				case rr7: {
					result = r.rr7;
					++r.rr7;
					break;
				}
				case rr8: {
					result = r.rr8;
					++r.rr8;
					break;
				}
				case rr9: {
					result = r.rr9;
					++r.rr9;
					break;
				}
				case rr10: {
					result = r.rr10;
					++r.rr10;
					break;
				}
				case rr11: {
					result = r.rr11;
					++r.rr11;
					break;
				}
				case rr12: {
					result = r.rr12;
					++r.rr12;
					break;
				}
				case rr13: {
					result = r.rr13;
					++r.rr13;
					break;
				}
				case rr14: {
					result = r.rr14;
					++r.rr14;
					break;
				}
				case rr15: {
					result = r.rr15;
					++r.rr15;
					break;
				}
				case rr16: {
					result = r.rr16;
					++r.rr16;
					break;
				}
				case rr17: {
					result = r.rr17;
					++r.rr17;
					break;
				}
				case rr18: {
					result = r.rr18;
					++r.rr18;
					break;
				}
				case rr19: {
					result = r.rr19;
					++r.rr19;
					break;
				}
				case rr20: {
					result = r.rr20;
					++r.rr20;
					break;
				}
				case rr21: {
					result = r.rr21;
					++r.rr21;
					break;
				}
				case rr22: {
					result = r.rr22;
					++r.rr22;
					break;
				}
				case rr23: {
					result = r.rr23;
					++r.rr23;
					break;
				}

				}

				if (++result > 15) {
					carry = true;
				}
				else {
					carry = false;
				}

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
			case xch: {
				int value = rom[++p.pc];

				switch (value) {
				case rr0: {
					uint8_t temp = a.ac;
					a.ac = r.rr0;
					r.rr0 = temp;
					break;
				}
				case rr1: {
					uint8_t temp = a.ac;
					a.ac = r.rr1;
					r.rr1 = temp;
					break;
				}
				case rr2: {
					uint8_t temp = a.ac;
					a.ac = r.rr2;
					r.rr2 = temp;
					break;
				}
				case rr3: {
					uint8_t temp = a.ac;
					a.ac = r.rr3;
					r.rr3 = temp;
					break;
				}
				case rr4: {
					uint8_t temp = a.ac;
					a.ac = r.rr4;
					r.rr4 = temp;
					break;
				}
				case rr5: {
					uint8_t temp = a.ac;
					a.ac = r.rr5;
					r.rr5 = temp;
					break;
				}
				case rr6: {
					uint8_t temp = a.ac;
					a.ac = r.rr6;
					r.rr6 = temp;
					break;
				}
				case rr7: {
					uint8_t temp = a.ac;
					a.ac = r.rr7;
					r.rr7 = temp;
					break;
				}
				case rr8: {
					uint8_t temp = a.ac;
					a.ac = r.rr8;
					r.rr8 = temp;
					break;
				}
				case rr9: {
					uint8_t temp = a.ac;
					a.ac = r.rr9;
					r.rr9 = temp;
					break;
				}
				case rr10: {
					uint8_t temp = a.ac;
					a.ac = r.rr10;
					r.rr10 = temp;
					break;
				}
				case rr11: {
					uint8_t temp = a.ac;
					a.ac = r.rr11;
					r.rr11 = temp;
					break;
				}
				case rr12: {
					uint8_t temp = a.ac;
					a.ac = r.rr12;
					r.rr12 = temp;
					break;
				}
				case rr13: {
					uint8_t temp = a.ac;
					a.ac = r.rr13;
					r.rr13 = temp;
					break;
				}
				case rr14: {
					uint8_t temp = a.ac;
					a.ac = r.rr14;
					r.rr14 = temp;
					break;
				}
				case rr15: {
					uint8_t temp = a.ac;
					a.ac = r.rr15;
					r.rr15 = temp;
					break;
				}
				case rr16: {
					uint8_t temp = a.ac;
					a.ac = r.rr16;
					r.rr16 = temp;
					break;
				}
				case rr17: {
					uint8_t temp = a.ac;
					a.ac = r.rr17;
					r.rr17 = temp;
					break;
				}
				case rr18: {
					uint8_t temp = a.ac;
					a.ac = r.rr18;
					r.rr18 = temp;
					break;
				}
				case rr19: {
					uint8_t temp = a.ac;
					a.ac = r.rr19;
					r.rr19 = temp;
					break;
				}
				case rr20: {
					uint8_t temp = a.ac;
					a.ac = r.rr20;
					r.rr20 = temp;
					break;
				}
				case rr21: {
					uint8_t temp = a.ac;
					a.ac = r.rr21;
					r.rr21 = temp;
					break;
				}
				case rr22: {
					uint8_t temp = a.ac;
					a.ac = r.rr22;
					r.rr22 = temp;
					break;
				}
				case rr23: {
					uint8_t temp = a.ac;
					a.ac = r.rr23;
					r.rr23 = temp;
					break;
				}

				}

				break;
			}
			case sub: {
				int value = rom[++p.pc];
				int8_t result = a.ac;

				switch (value) {
				case rr0: {
					a.ac -= r.rr0;
					value = r.rr0;
					break;
				}
				case rr1: {
					a.ac -= r.rr1;
					value = r.rr1;
					break;
				}
				case rr2: {
					a.ac -= r.rr2;
					value = r.rr2;
					break;
				}
				case rr3: {
					a.ac -= r.rr3;
					value = r.rr3;
					break;
				}
				case rr4: {
					a.ac -= r.rr4;
					value = r.rr4;
					break;
				}
				case rr5: {
					a.ac -= r.rr5;
					value = r.rr5;
					break;
				}
				case rr6: {
					a.ac -= r.rr6;
					value = r.rr6;
					break;
				}
				case rr7: {
					a.ac -= r.rr7;
					value = r.rr7;
					break;
				}
				case rr8: {
					a.ac -= r.rr8;
					value = r.rr8;
					break;
				}
				case rr9: {
					a.ac -= r.rr9;
					value = r.rr9;
					break;
				}
				case rr10: {
					a.ac -= r.rr10;
					value = r.rr10;
					break;
				}
				case rr11: {
					a.ac -= r.rr11;
					value = r.rr11;
					break;
				}
				case rr12: {
					a.ac -= r.rr12;
					value = r.rr12;
					break;
				}
				case rr13: {
					a.ac -= r.rr13;
					value = r.rr13;
					break;
				}
				case rr14: {
					a.ac -= r.rr14;
					value = r.rr14;
					break;
				}
				case rr15: {
					a.ac -= r.rr15;
					value = r.rr15;
					break;
				}
				case rr16: {
					a.ac -= r.rr16;
					value = r.rr16;
					break;
				}
				case rr17: {
					a.ac -= r.rr17;
					value = r.rr17;
					break;
				}
				case rr18: {
					a.ac -= r.rr18;
					value = r.rr18;
					break;
				}
				case rr19: {
					a.ac -= r.rr19;
					value = r.rr19;
					break;
				}
				case rr20: {
					a.ac -= r.rr20;
					value = r.rr20;
					break;
				}
				case rr21: {
					a.ac -= r.rr21;
					value = r.rr21;
					break;
				}
				case rr22: {
					a.ac -= r.rr22;
					value = r.rr22;
					break;
				}
				case rr23: {
					a.ac -= r.rr23;
					value = r.rr23;
					break;
				}
				}

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

			opcode = rom[++p.pc];
		}

		return 0;
	}
};