#ifndef __SOKOBAN_HPP__
#define __SOKOBAN_HPP__

#include <vector>


class Sokoban {
	public:
		Sokoban();
		Sokoban(std::vector<std::vector<char>> t) : table(t) {    }
	private:
		std::vector<std::vector<char>> table;
};



#endif