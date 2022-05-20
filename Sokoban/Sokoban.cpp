#include "Sokoban.hpp"

#include <iostream>
#include <string>
#include <vector>



std::vector<bool> State::GetPlayerCoordinates() const {
	return player_coordinates;
}


std::vector<bool> State::GetBoxesCoordinates() const {
	return boxes_coordinates;
}


Sokoban::Sokoban(int _plan_length, std::vector<std::string> table) {
	plan_length = _plan_length;
	
	int i, j;
	n = table.size();
	m = table[0].length();
	
	std::vector<bool> player_coordinates(n*m, false);
	std::vector<bool> boxes_coordinates(n*m, false);
	
	std::vector<bool> tmp1(n*m, false);
	walls = tmp1;
	
	std::vector<bool> tmp2(n*m, false);
	box_home = tmp2;
	
	for(i=0; i<n; i++) {
		for(j=0; j<m; j++) {
			switch(table[i][j]) {
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
				default: // prazan prostor
					continue;
			}
		}
	}
	
	state = new State(player_coordinates, boxes_coordinates);
	
}


Sokoban::~Sokoban() {
	delete state;
}


void Sokoban::PrintTable() const {
	
	std::vector<bool> player_coordinates = state->GetPlayerCoordinates();
	std::vector<bool> boxes_coordinates = state->GetBoxesCoordinates();
	
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
