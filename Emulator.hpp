#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

unsigned count_commands_real = 0;
unsigned count_commands = 0;
map<unsigned, unsigned> real_commands;
map<string, unsigned> commands;

vector<uint16_t> rom;
bool is_halted = false;

enum Assembler {
	jcn = 0x01,
	fim = 0x02,
	fin = 0x03,
	jin = 0x03,
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
	clb = 0xF0,
	clc = 0xF1,
	hlt = 0xF2,

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
		uint16_t pc : 4;
	};

	std::vector<uint8_t> assemble(const std::string& asm_code) {
		std::vector<uint8_t> instructions;
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

	void load_program(std::vector<uint8_t> instructions) {
		rom.resize(10);
		for (size_t i = 0; i < instructions.size(); i++) {
			rom[i] = instructions[i];
		}

		for (size_t i = instructions.size(); i < rom.size(); i++) {
			rom[i] = 0;
		}
	}

	int run() {
		Registers r;
		Accumulator a;
		PointerCounter p;

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

		while (is_halted != true) {
			uint8_t opcode;
			if (p.pc > 0) {
				opcode = rom[p.pc++];
			}
			else if (p.pc == 0) {
				opcode = rom[p.pc];
			}
			else {
				return -1;
			}

			switch (opcode) {
			case hlt:
				is_halted = true;;
			}
		}

		return 0;
	}
};