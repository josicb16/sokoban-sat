#ifndef __SOKOBAN_HPP__
#define __SOKOBAN_HPP__

#include "Formula.hpp"

#include <vector>
#include <string>

class Table {
	public:
		Table(int _n, int _m, std::vector<bool> _walls, std::vector<bool> _box_home, std::vector<bool> _player_coordinates, std::vector<bool> _boxes_coordinates)
			: n(_n), m(_m), walls(_walls), box_home(_box_home), player_coordinates(_player_coordinates), boxes_coordinates(_boxes_coordinates) {	}
			
		const std::vector<bool> & GetWalls() const;
		const std::vector<bool> & GetBoxHome() const;
		const std::vector<bool> & GetPlayerCoordinates() const;
		const std::vector<bool> & GetBoxesCoordinates() const;
		int GetN() const;
		int GetM() const;
		
	private:
		int n;
		int m;
		std::vector<bool> walls;
		std::vector<bool> box_home;
		std::vector<bool> player_coordinates;
		std::vector<bool> boxes_coordinates;
};


class Sokoban {
	public:
		Sokoban(int _plan_length, std::vector<std::string> table);
		~Sokoban();
		void GeneratePlanFormula(std::ofstream &dimacs) const;
		Table *GetTable() const;
		void PrintTable() const;
		
	private:
		int plan_length;
		Table *table;
};

#endif
