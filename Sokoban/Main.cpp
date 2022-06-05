#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Sokoban.hpp"


int main() {
	
	int plan_length;
	std::vector<std::string> table;
	
	std::string tmp;
	
	
	std::cout << "Zadati pocetni izgled table (za kraj unosa upisati - KRAJ):\n";
	while(true) {
		std::getline(std::cin, tmp);
		if(tmp=="KRAJ")
			break;
		table.push_back(tmp);
	}
	
	std::cout << "Upisati duzinu plana: "; // za ovu tablu - 50
	std::cin >> plan_length;

	Sokoban s(plan_length, table);	
	
	
	std::ofstream dimacs;
	dimacs.open("dimacs.txt");

	dimacs << s.GeneratePlanFormula()->Dimacs();
	
	dimacs.close();

	return 0;
}
