#ifndef __SOKOBAN_HPP__
#define __SOKOBAN_HPP__

#include "Formula.hpp"

#include <vector>
#include <string>


class State {
	public:
		State(std::vector<bool> _player_coordinates, std::vector<bool> _boxes_coordinates) 
			: player_coordinates(_player_coordinates), boxes_coordinates(_boxes_coordinates) { 		}
		
		std::vector<bool> GetPlayerCoordinates() const; 
		std::vector<bool> GetBoxesCoordinates() const; 
		
	private:
		std::vector<bool> player_coordinates; // [1, n*m]
		std::vector<bool> boxes_coordinates; //  [1+n*m, 2*n*m]
};


class Table {
	public:
		Table(int _n, int _m, std::vector<bool> _walls, std::vector<bool> _box_home)
			: n(_n), m(_m), walls(_walls), box_home(_box_home) {	}
			
		std::vector<bool> GetWalls() const;
		std::vector<bool> GetBoxHome() const;
		int GetN() const;
		int GetM() const;
		
	private:
		int n;
		int m;
		std::vector<bool> walls;
		std::vector<bool> box_home;
};


class Move {
	public:
		Move(short _type, Table *_table, State *_state) : type(_type), table(_table), state(_state)  {	}  
		~Move();
		std::string GetType() const;
		Formula MovePrecondition(int k) const;
		Formula MoveEffect(int k) const;
	private:
		short type;
		Table *table;
		State *state;
};


class Sokoban {
	public:
		Sokoban(int _plan_length, std::vector<std::string> table);
		~Sokoban();
		Table *GetTable() const;
		State *GetState() const;
		void PrintTable() const;
		
	private:
		int plan_length;
		Table *table;
		State *current_state;
};



#endif
