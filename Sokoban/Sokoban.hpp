#ifndef __SOKOBAN_HPP__
#define __SOKOBAN_HPP__

#include <vector>
#include <string>

class State {
	public:
		State(std::vector<bool> _player_coordinates, std::vector<bool> _boxes_coordinates) 
			: player_coordinates(_player_coordinates), boxes_coordinates(_boxes_coordinates) { 		}

		std::vector<bool> GetPlayerCoordinates() const; 
		std::vector<bool> GetBoxesCoordinates() const;

	private:
		std::vector<bool> player_coordinates;
		std::vector<bool> boxes_coordinates;
};



class Sokoban {
	public:
		Sokoban(int _plan_length, std::vector<std::string> table);
		~Sokoban();

		void PrintTable() const;


	private:
		int plan_length;
		int n;  // napraviti klasu Table
		int m;
		std::vector<bool> walls;
		std::vector<bool> box_home;
		State *state;
};




#endif