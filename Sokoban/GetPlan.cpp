#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>


bool IsPositive(int x);


int main() {	
  
	std::ifstream read;
	std::ifstream info;
	
	int n, m, plan_length;
	
	info.open("info.txt");  
	
	info >> n;
	info >> m;
	info >> plan_length;
	
	info.close();
	
	read.open("result.txt");
	
	std::string tmp;
	read >> tmp;
	
	int i;
	
	std::vector<int> sokoban_variable;
	int variable;
	
	for(i=0; i<2*n*m*(plan_length+1); i++) {
	  read >> variable;
	  sokoban_variable.push_back(variable);
	}
	
	read.close();
	
	std::vector<int>::iterator it = std::find_if(sokoban_variable.begin(), sokoban_variable.end(), IsPositive);
	std::vector<int>::iterator it_next;
	int index = it - sokoban_variable.begin();
	int index_next;
	
	int k=1;
	while(true) {
	  it_next = std::find_if(++it, sokoban_variable.end(), IsPositive);
	  if(it_next == sokoban_variable.end())
	    break;
	  
	  index_next = it_next - sokoban_variable.begin();
	  
	  if(index_next <= 2*k*n*m && index_next>=(n*m*(2*k-1)+1)) {
	    it = it_next;
	    continue;
	  }
	  
	  if(index_next-index == m + 2*n*m) 
	    std::cout << "dole ";
	  else if(index_next-index == -m + 2*n*m)
	    std::cout << "gore ";
	  else if(index_next-index == 1 + 2*n*m)
	    std::cout << "desno ";
	  else if(index_next-index == -1 + 2*n*m)
	    std::cout << "levo ";
	  
	  it = it_next;
	  index = index_next;
	  
	  k++;
	  
	  
	}
	
	  
}


bool IsPositive(int x) {
  return x>0;
}	






