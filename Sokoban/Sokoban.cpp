#include "Sokoban.hpp"
#include "Formula.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <set>

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



Formula Move::MovePrecondition(int k) const {
	int n = table->GetN();
	int m = table->GetM();
	int i;
	
	std::vector<bool> S = state->GetPlayerCoordinates(); // [1, n*m] 
	std::vector<bool> B = state->GetBoxesCoordinates(); //  [1+n*m, 2*n*m]
	std::vector<bool> W = table->GetWalls();
	
	
	if(type==0) {   // up
		Formula tmp_atom;
		Formula tmp_box;
		Formula tmp_not;
		Formula tmp_and;
		Formula tmp_or;
		std::vector<Formula> tmp_vector;
		
		Formula first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=0; i<m; i++) {       
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 

		int vector_size = tmp_vector.size();
		for(i=m; i<n*m; i++) {
			if(!W[i-m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1+2*(k-1)*n*m+n*m-m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		} 
		
		Formula first_condition = std::make_shared<And>(tmp_vector[m], tmp_vector[vector_size-1]);
		
		tmp_vector.clear();
		
		first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=0; i<2*m; i++) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		int first_size = tmp_vector.size();
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 
		
		vector_size = tmp_vector.size();
		for(i=2*m; i<n*m; i++) {
			if(!W[i-2*m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1+2*(k-1)*n*m+n*m-2*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}
		
		int second = vector_size;
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first);
		vector_size++;
		
		for(i=m; i<n*m; i++) {
			if(!W[i-m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_atom);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}

		tmp_and = std::make_shared<And>(tmp_vector[first_size-1], tmp_vector[second-1]);
		Formula second_condition = std::make_shared<And>(tmp_and, tmp_vector[vector_size-1]);
		
		return std::make_shared<Or>(first_condition, second_condition);
		
	}
	
	if(type==1) { // down
		Formula tmp_atom;
		Formula tmp_box;
		Formula tmp_not;
		Formula tmp_and;
		Formula tmp_or;
		std::vector<Formula> tmp_vector;
		
		Formula first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=n*m-m; i<n*m; i++) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 

		int vector_size = tmp_vector.size();
		for(i=0; i<n*m-m; i++) {
			if(!W[i+m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1+2*(k-1)*n*m+n*m+m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		} 
		
		Formula first_condition = std::make_shared<And>(tmp_vector[m], tmp_vector[vector_size-1]);
		
		tmp_vector.clear();
		
		first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=n*m-2*m; i<n*m; i++) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		int first_size = tmp_vector.size();
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 
		
		vector_size = tmp_vector.size();
		for(i=0; i<n*m-2*m; i++) {
			if(!W[i+2*m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1+2*(k-1)*n*m+n*m+2*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}
		
		int second = vector_size;
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first);
		vector_size++;
		
		for(i=0; i<n*m-m; i++) {
			if(!W[i+m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_atom);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}

		tmp_and = std::make_shared<And>(tmp_vector[first_size-1], tmp_vector[second-1]);
		Formula second_condition = std::make_shared<And>(tmp_and, tmp_vector[vector_size-1]);
		
		return std::make_shared<Or>(first_condition, second_condition);
	}
	
	if(type==2) { // left
	
	Formula tmp_atom;
		Formula tmp_box;
		Formula tmp_not;
		Formula tmp_and;
		Formula tmp_or;
		std::vector<Formula> tmp_vector;
		
		Formula first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=0; i<n; i++) {
			tmp_atom = std::make_shared<Atom>(1+2*(k-1)*n*m+i*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 

		int vector_size = tmp_vector.size();
		for(i=0; i<n*m; i++) {
			if(i%m==0)
				continue;
			if(!W[i-1]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+2*(k-1)*n*m+n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		} 
		
		Formula first_condition = std::make_shared<And>(tmp_vector[n], tmp_vector[vector_size-1]);
		
		tmp_vector.clear();
		
		first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=0; i<n; i++) { 
			tmp_atom = std::make_shared<Atom>(1+2*(k-1)*n*m+i*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
			tmp_atom = std::make_shared<Atom>(2+2*(k-1)*n*m+i*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		int first_size = tmp_vector.size();
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 
		
		vector_size = tmp_vector.size();
		for(i=0; i<n*m; i++) {   
			if(i%m==0 || i%m==1)
				continue;
			if(!W[i-2]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i-1+2*(k-1)*n*m+n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}
		
		int second = vector_size;
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first);
		vector_size++;
		
		for(i=0; i<n*m; i++) {
			if(i%m==0)
				continue;
			if(!W[i-1]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_atom);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}

		tmp_and = std::make_shared<And>(tmp_vector[first_size-1], tmp_vector[second-1]);
		Formula second_condition = std::make_shared<And>(tmp_and, tmp_vector[vector_size-1]);
		
		return std::make_shared<Or>(first_condition, second_condition);
	
	}
	if(type==3) { // right
	
	Formula tmp_atom;
		Formula tmp_box;
		Formula tmp_not;
		Formula tmp_and;
		Formula tmp_or;
		std::vector<Formula> tmp_vector;
	
		Formula first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=1; i<=n; i++) {
			tmp_atom = std::make_shared<Atom>(2*(k-1)*n*m + i*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 

		int vector_size = tmp_vector.size();
		for(i=0; i<n*m; i++) { 
			if((i+1)%m==0)
				continue;
			if(!W[i+1]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+2+2*(k-1)*n*m+n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		} 
		
		Formula first_condition = std::make_shared<And>(tmp_vector[n], tmp_vector[vector_size-1]);
		
		tmp_vector.clear();
		
		first = std::make_shared<True>();
		tmp_vector.push_back(first);
		for(i=1; i<=n; i++) {
			tmp_atom = std::make_shared<Atom>(2*(k-1)*n*m+i*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
			tmp_atom = std::make_shared<Atom>(2*(k-1)*n*m+i*m-1);
			tmp_not = std::make_shared<Not>(tmp_atom);
			tmp_and = std::make_shared<And>(tmp_vector[i], tmp_not);
			tmp_vector.push_back(tmp_and);
		}
		
		int first_size = tmp_vector.size();
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first); 
		
		vector_size = tmp_vector.size();
		for(i=0; i<n*m; i++) { 
			if((i+1)%m==0 || (i+2)%m==0)
				continue;
			if(!W[i+2]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m );
				tmp_box = std::make_shared<Atom>(i+3+2*(k-1)*n*m+n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_not);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_and);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}
		
		int second = vector_size;
		
		first = std::make_shared<False>();
		tmp_vector.push_back(first);
		vector_size++;
		
		for(i=0; i<n*m; i++) {
			if((i+1)%m==0)
				continue;
			if(!W[i+1]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_or = std::make_shared<Or>(tmp_vector[vector_size-1], tmp_atom);
				vector_size++;
				tmp_vector.push_back(tmp_or);
			}
		}

		tmp_and = std::make_shared<And>(tmp_vector[first_size-1], tmp_vector[second-1]);
		Formula second_condition = std::make_shared<And>(tmp_and, tmp_vector[vector_size-1]);
		
		return std::make_shared<Or>(first_condition, second_condition);
	}
	
}





Move::~Move() {
	delete table;
	delete state;
}


Table *Sokoban::GetTable() const {
	return table;
}
		
State *Sokoban::GetState() const {
	return current_state;
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
					walls[j*m+i] = true;
					break;
				case 'o':  // box_home
					box_home[j*m+i] = true;
					break;
				case 's':  // Sokoban
					player_coordinates[j*m+i] = true;
					break;
				case 'S':  // Sokoban; box_home
					player_coordinates[j*m+i] = true;
					box_home[j*m+i] = true;
					break;
				case 'b':  // box
					boxes_coordinates[j*m+i] = true;
					break;
				case 'B':  // box; box_home
					boxes_coordinates[j*m+i] = true;
					box_home[j*m+i] = true;
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
	
	std::vector<bool> player_coordinates = current_state->GetPlayerCoordinates();
	std::vector<bool> boxes_coordinates = current_state->GetBoxesCoordinates();
	
	int n = table->GetN();
	int m = table->GetM();
	
	int i, j;
	for(i=0; i<n; i++) {
		for(j=0; j<m; j++) {
			if(walls[j*m+i]) {
				std::cout << "#";
				continue;
			}
			else if(box_home[j*m+i]) {
				if(boxes_coordinates[j*m+i]) 
					std::cout << "B";
				else if(player_coordinates[j*m+i])
					std::cout << "S";
				else
					std::cout << "o";
				continue;
			}
			else if(player_coordinates[j*m+i]) {
				std::cout << "s";
				continue;
			}
			else if(boxes_coordinates[j*m+i]) {
				std::cout << "b";
				continue;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


