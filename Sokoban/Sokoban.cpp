#include "Sokoban.hpp"

#include <vector>
#include <string>

#include <iostream>

Sokoban::Sokoban() {
	std::vector<std::string> _table;
	table = _table;
}

void Sokoban::PrintTable() const {
	auto it = table.cbegin();
	auto it_end = table.cend();

	while(it != it_end) {
		std::cout << *it << std::endl;
		it++;
	}
}

