#include <cstdint>
#include <iostream>

#include "Emulator.hpp"

class Display : public Emulator {
public:
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
};