#ifndef __SOKOBAN_HPP__
#define __SOKOBAN_HPP__

#include <vector>
#include <string>


class Sokoban {
	public:
		Sokoban();
		Sokoban(std::vector<std::string> _table) : table(_table) {    }

		void PrintTable() const;

	private:
		std::vector<std::string> table;
};

#endif