#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <algorithm>

using namespace std;

unsigned count_commands_real = 0;
unsigned count_commands = 0;
map<unsigned, unsigned> real_commands;
map<string, unsigned> commands;

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
	ac
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
	{"rr23", rr23}
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
		rom.resize(60);
		this->instructions = instructions;

		for (size_t i = 0; i < instructions.size(); i++) {
			rom[i] = instructions[i];
		}

		for (size_t i = instructions.size(); i < rom.size(); i++) {
			rom[i] = 0;
		}
	}

	uint8_t get_acc() {
		std::cout << static_cast<int>(a.ac) << std::endl;
		return a.ac;
	}

	bool get_carry() {
		std::cout << carry << std::endl;
		return carry;
	}

	uint8_t get_rr0() {
		std::cout << static_cast<int>(r.rr0) << std::endl;
		return r.rr0;
	}

	uint8_t get_rr1() {
		std::cout << static_cast<int>(r.rr1) << std::endl;
		return r.rr1;
	}

	uint8_t get_rr2() {
		std::cout << static_cast<int>(r.rr2) << std::endl;
		return r.rr2;
	}

	uint8_t get_rr3() {
		std::cout << static_cast<int>(r.rr3) << std::endl;
		return r.rr3;
	}

	uint8_t get_rr4() {
		std::cout << static_cast<int>(r.rr4) << std::endl;
		return r.rr4;
	}

	uint8_t get_rr5() {
		std::cout << static_cast<int>(r.rr5) << std::endl;
		return r.rr5;
	}

	uint8_t get_rr6() {
		std::cout << static_cast<int>(r.rr6) << std::endl;
		return r.rr6;
	}

	uint8_t get_rr7() {
		std::cout << static_cast<int>(r.rr7) << std::endl;
		return r.rr7;
	}

	uint8_t get_rr8() {
		std::cout << static_cast<int>(r.rr8) << std::endl;
		return r.rr8;
	}

	uint8_t get_rr9() {
		std::cout << static_cast<int>(r.rr9) << std::endl;
		return r.rr9;
	}

	uint8_t get_rr10() {
		std::cout << static_cast<int>(r.rr10) << std::endl;
		return r.rr10;
	}

	uint8_t get_rr11() {
		std::cout << static_cast<int>(r.rr11) << std::endl;
		return r.rr11;
	}

	uint8_t get_rr12() {
		std::cout << static_cast<int>(r.rr12) << std::endl;
		return r.rr12;
	}

	uint8_t get_rr13() {
		std::cout << static_cast<int>(r.rr13) << std::endl;
		return r.rr13;
	}

	uint8_t get_rr14() {
		std::cout << static_cast<int>(r.rr14) << std::endl;
		return r.rr14;
	}

	uint8_t get_rr15() {
		std::cout << static_cast<int>(r.rr15) << std::endl;
		return r.rr15;
	}

	uint8_t get_rr16() {
		std::cout << static_cast<int>(r.rr16) << std::endl;
		return r.rr16;
	}

	uint8_t get_rr17() {
		std::cout << static_cast<int>(r.rr17) << std::endl;
		return r.rr17;
	}

	uint8_t get_rr18() {
		std::cout << static_cast<int>(r.rr18) << std::endl;
		return r.rr18;
	}

	uint8_t get_rr19() {
		std::cout << static_cast<int>(r.rr19) << std::endl;
		return r.rr19;
	}

	uint8_t get_rr20() {
		std::cout << static_cast<int>(r.rr20) << std::endl;
		return r.rr20;
	}

	uint8_t get_rr21() {
		std::cout << static_cast<int>(r.rr21) << std::endl;
		return r.rr21;
	}

	uint8_t get_rr22() {
		std::cout << static_cast<int>(r.rr22) << std::endl;
		return r.rr22;
	}

	uint8_t get_rr23() {
		std::cout << static_cast<int>(r.rr23) << std::endl;
		return r.rr23;
	}

	unsigned get_counter() {
		return count_commands;
	}

	unsigned get_really_counter() {
		return count_commands_real;
	}

	Registers r;
	Accumulator a;
	PointerCounter p;

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
				int value = real_commands[rom[++p.pc]] - 1;

				p.pc = value;

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
				else if (ac + value <= 15) {
					carry = false;
				}

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

				switch (value) {
				case rr0: {
					a.ac -= r.rr0;
					break;
				}
				case rr1: {
					a.ac -= r.rr1;
					break;
				}
				case rr2: {
					a.ac -= r.rr2;
					break;
				}
				case rr3: {
					a.ac -= r.rr3;
					break;
				}
				case rr4: {
					a.ac -= r.rr4;
					break;
				}
				case rr5: {
					a.ac -= r.rr5;
					break;
				}
				case rr6: {
					a.ac -= r.rr6;
					break;
				}
				case rr7: {
					a.ac -= r.rr7;
					break;
				}
				case rr8: {
					a.ac -= r.rr8;
					break;
				}
				case rr9: {
					a.ac -= r.rr9;
					break;
				}
				case rr10: {
					a.ac -= r.rr10;
					break;
				}
				case rr11: {
					a.ac -= r.rr11;
					break;
				}
				case rr12: {
					a.ac -= r.rr12;
					break;
				}
				case rr13: {
					a.ac -= r.rr13;
					break;
				}
				case rr14: {
					a.ac -= r.rr14;
					break;
				}
				case rr15: {
					a.ac -= r.rr15;
					break;
				}
				case rr16: {
					a.ac -= r.rr16;
					break;
				}
				case rr17: {
					a.ac -= r.rr17;
					break;
				}
				case rr18: {
					a.ac -= r.rr18;
					break;
				}
				case rr19: {
					a.ac -= r.rr19;
					break;
				}
				case rr20: {
					a.ac -= r.rr20;
					break;
				}
				case rr21: {
					a.ac -= r.rr21;
					break;
				}
				case rr22: {
					a.ac -= r.rr22;
					break;
				}
				case rr23: {
					a.ac -= r.rr23;
					break;
				}
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