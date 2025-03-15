#include <vector>
#include <cstdint>
#include <iostream>

using namespace std;

class Intel4002 {
public:
	Intel4002() {
		ram.resize(4);

		for (auto& ch : ram) {
			ch.resize(20, 0);
		}
	}

	vector<vector <uint8_t>> ram;
};