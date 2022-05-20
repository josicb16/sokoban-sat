#include "Sokoban.hpp"
#include "Formula.hpp"

#include <iostream>
#include <string>
#include <vector>

std::vector<bool> State::GetPlayerCoordinates() const { return player_coordinates; }
std::vector<bool> State::GetBoxesCoordinates() const { return boxes_coordinates; }

std::vector<bool> Table::GetWalls() const { return walls; }
std::vector<bool> Table::GetBoxHome() const { return box_home;}
int Table::GetN() const { return n; }
int Table::GetM() const { return m; }

std::string Move::GetType() const {
	switch(type) {
		case(0): return "up";
		case(1): return "down";
		case(2): return "left";
		case(3): return "right";
	}
}

Move::~Move() {
	delete table;
	delete state;
}


Sokoban::Sokoban(int _plan_length, std::vector<std::string> table_str) {
	plan_length = _plan_length;
	
	int i, j;
	int n = table_str.size();
	int m = table_str[0].length();
	
	std::vector<bool> player_coordinates(n*m, false);
	std::vector<bool> boxes_coordinates(n*m, false);
	
	std::vector<bool> walls(n*m, false);
	std::vector<bool> box_home(n*m, false);
	
	for(i=0; i<n; i++) {
		for(j=0; j<m; j++) {
			switch(table_str[i][j]) {
				case '#':  // wall
					walls[j*n+i] = true;
					break;
				case 'o':  // box_home
					box_home[j*n+i] = true;
					break;
				case 's':  // Sokoban
					player_coordinates[j*n+i] = true;
					break;
				case 'S':  // Sokoban; box_home
					player_coordinates[j*n+i] = true;
					box_home[j*n+i] = true;
					break;
				case 'b':  // box
					boxes_coordinates[j*n+i] = true;
					break;
				case 'B':  // box; box_home
					boxes_coordinates[j*n+i] = true;
					box_home[j*n+i] = true;
					break;
				default: // empty space
					continue;
			}
		}
	}
	table = new Table(n, m, walls, box_home);
	current_state = new State(player_coordinates, boxes_coordinates);
}

Sokoban::~Sokoban() {
	delete table;
	delete current_state;
}

void Sokoban::PrintTable() const {
	std::vector<bool> walls = table->GetWalls();
	std::vector<bool> box_home = table->GetBoxHome();
	
	std::vector<bool> player_coordinates = state->GetPlayerCoordinates();
	std::vector<bool> boxes_coordinates = state->GetBoxesCoordinates();
	
	int n = table->GetN();
	int m = table->GetM();
	
	int i, j;
	for(i=0; i<n; i++) {
		for(j=0; j<m; j++) {
			if(walls[j*n+i]) {
				std::cout << "#";
				continue;
			}
			else if(box_home[j*n+i]) {
				if(boxes_coordinates[j*n+i]) 
					std::cout << "B";
				else if(player_coordinates[j*n+i])
					std::cout << "S";
				else
					std::cout << "o";
				continue;
			}
			else if(player_coordinates[j*n+i]) {
				std::cout << "s";
				continue;
			}
			else if(boxes_coordinates[j*n+i]) {
				std::cout << "b";
				continue;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


