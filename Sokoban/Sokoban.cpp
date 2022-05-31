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


static Formula MovePrecondition(int type, int k, int n, int m, std::vector<bool> &S, std::vector<bool> &B, std::vector<bool> &W) {
	int i;
	
	Formula res;
	
	Formula tmp_atom;
	Formula tmp_and;
	Formula tmp_box;
	Formula tmp_not;
	Formula tmp_or;
	Formula tmp_impl;
	
	if(type==0) {   // gore
		
		// Sokoban se ne nalazi u nultom redu 
		tmp_atom = std::make_shared<Atom>(1+2*(k-1)*n*m);
		
		res = std::make_shared<Not>(tmp_atom);
		
		for(i=1; i<m; i++) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			
			res = std::make_shared<And>(res, tmp_not);
		}
		
		// Iznad Sokobana se ne nalazi zid
		for(i=m; i<n*m; i++) {
			if(W[i-m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_atom);
				
				res = std::make_shared<And>(res, tmp_not);
			}
		}
		
		// Ako je iznad Sokobana kutija, onda iznad kutije ne smeju biti ni zid ni kutija
		for(i=2*m; i<n*m; i++) {
			if(W[i-2*m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				tmp_not = std::make_shared<Not>(tmp_and);
				
				res = std::make_shared<And>(res, tmp_not);
			}
			else {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				
				tmp_box = std::make_shared<Atom>(i+1-2*m+n*m+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				
				tmp_impl = std::make_shared<Impl>(tmp_and, tmp_not);
				
				res = std::make_shared<And>(res, tmp_impl);
			}
		}
		
		return res;
		
	}
	
	if(type==1) { // dole
		
		// Sokoban se ne nalazi u poslednjem redu 
		tmp_atom = std::make_shared<Atom>(n*m-m+1+2*(k-1)*n*m);
		
		res = std::make_shared<Not>(tmp_atom);
		
		for(i=n*m-m+1; i<n*m; i++) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			
			res = std::make_shared<And>(res, tmp_not);
		}
		
		// Ispod Sokobana se ne nalazi zid
		for(i=0; i<n*m-m; i++) {
			if(W[i+m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_atom);
				
				res = std::make_shared<And>(res, tmp_not);
			}
		}
		
		// Ako je ispod Sokobana kutija, onda ispod kutije ne smeju biti ni zid ni kutija
		for(i=0; i<n*m-2*m; i++) {
			if(W[i+2*m]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				tmp_not = std::make_shared<Not>(tmp_and);
				
				res = std::make_shared<And>(res, tmp_not);
			}
			else {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				
				tmp_box = std::make_shared<Atom>(i+1+2*m+n*m+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				
				tmp_impl = std::make_shared<Impl>(tmp_and, tmp_not);
				
				res = std::make_shared<And>(res, tmp_impl);
			}
		}
		
		return res;
		
	}
	
	if(type==2) { // levo
	
		// Sokoban se ne nalazi u nultoj koloni 
		tmp_atom = std::make_shared<Atom>(1+2*(k-1)*n*m);
		
		res = std::make_shared<Not>(tmp_atom);
		
		for(i=m; i<n*m-m+1; i+=m) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			
			res = std::make_shared<And>(res, tmp_not);
		}
		
		// Levo od Sokobana se ne nalazi zid
		for(i=0; i<n*m; i++) {
			if(i%m==0)
				continue;
			if(W[i-1]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_atom);
				
				res = std::make_shared<And>(res, tmp_not);
			}
		}
		
		// Ako je levo od Sokobana kutija, onda levo od kutije ne smeju biti ni zid ni kutija
		for(i=0; i<n*m; i++) {
			if(i%m==0 || (i-1)%m==0)
				continue;
			if(W[i-2]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				tmp_not = std::make_shared<Not>(tmp_and);
				
				res = std::make_shared<And>(res, tmp_not);
			}
			else {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				
				tmp_box = std::make_shared<Atom>(i-1+n*m+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				
				tmp_impl = std::make_shared<Impl>(tmp_and, tmp_not);
				
				res = std::make_shared<And>(res, tmp_impl);
			}
		}
		
		return res;
	
	}
	if(type==3) { // desno
	
		// Sokoban se ne nalazi u poslednjoj koloni 
		tmp_atom = std::make_shared<Atom>(m+2*(k-1)*n*m);
		
		res = std::make_shared<Not>(tmp_atom);
		
		for(i=2*m-1; i<n*m; i+=m) {
			tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom);
			
			res = std::make_shared<And>(res, tmp_not);
		}
		
		// Desno od Sokobana se ne nalazi zid
		for(i=0; i<n*m; i++) {
			if((i+1)%m==0)
				continue;
			if(W[i+1]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_atom);
				
				res = std::make_shared<And>(res, tmp_not);
			}
		}
		
		
		// Ako je desno od Sokobana kutija, onda desno od kutije ne smeju biti ni zid ni kutija
		for(i=0; i<n*m; i++) {
			if((i+1)%m==0 || (i+2)%m==0)
				continue;
			if(W[i+2]) {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				tmp_not = std::make_shared<Not>(tmp_and);
				
				res = std::make_shared<And>(res, tmp_not);
			}
			else {
				tmp_atom = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
				tmp_box = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
				tmp_and = std::make_shared<And>(tmp_atom, tmp_box);
				
				tmp_box = std::make_shared<Atom>(i+3+n*m+2*(k-1)*n*m);
				tmp_not = std::make_shared<Not>(tmp_box);
				
				tmp_impl = std::make_shared<Impl>(tmp_and, tmp_not);
				
				res = std::make_shared<And>(res, tmp_impl);
			}
		}
		
		return res;
		
	}
	
}


static Formula MoveEffect(int type, int k, int n, int m, std::vector<bool> &S, std::vector<bool> &B) {
	
	int i;
	
	Formula res;
	
	Formula tmp_atom1;
	Formula tmp_atom2;
	Formula tmp_box;
	Formula tmp_and;
	Formula tmp_eql;
	Formula tmp_impl;
	Formula tmp_not;
	Formula tmp_or;
	
	if(type==0) {  // gore
		
		tmp_atom1 = std::make_shared<Atom>(m+1+2*(k-1)*n*m);
		tmp_atom2 = std::make_shared<Atom>(1+2*k*n*m);
		res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
		
		// Sokoban se pomera za jedno mesto gore
		for(i=m+1; i<n*m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		for(i=n*m-m; i<n*m; i++) {
			tmp_atom2 = std::make_shared<Atom>(i+1+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			res = std::make_shared<And>(res, tmp_not);
		}


		// Ako iznad Sokobana nije bila kutija, raspored kutija ostaje nepromenjen,
		// a ako je iznad Sokobana bila kutija, ona se pomera za jedno mesto gore

		for(i=2*m; i<n*m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			tmp_box = std::make_shared<Atom>(i+1-m+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom1 = std::make_shared<Atom>(i+1-2*m+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-2*m+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			tmp_or = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			tmp_box = std::make_shared<Atom>(i+1-m+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom2 = std::make_shared<Atom>(i+1-2*m+n*m+2*k*n*m);
			tmp_and = std::make_shared<And>(tmp_and, tmp_atom2);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom1);
			tmp_atom1 = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-m+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_not, tmp_eql);
			tmp_atom1 = std::make_shared<Atom>(i+1-2*m+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-2*m+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			res = std::make_shared<And>(res, tmp_or);
		}
		// Za sve redove osim za poslednji:
		//   (S[i] && ~B[i-m] && ~B_k[i-m]      && (B_k[i-2*m] <=> B[i-2*m])) || 
		//   (S[i] && B[i-m] && ~B_k[i-m]    && B_k[i-2*m])  ||
		//  (~S[i] && (B[i-m] <=> B_k[i-m]) && (B[i-2*m] <=> B_k[i-2*m]))
		
		
		// poslednji red 
		for(i=n*m-m; i<n*m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		
		return res;
		
	}
	else if(type==1) {  // dole
		
		tmp_atom1 = std::make_shared<Atom>(1+2*(k-1)*n*m);
		tmp_atom2 = std::make_shared<Atom>(1+m+2*k*n*m);
		res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
		
		for(i=0; i<n*m-m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		for(i=0; i<m; i++) {
			tmp_atom2 = std::make_shared<Atom>(i+1+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			res = std::make_shared<And>(res, tmp_not);
		}

		for(i=0; i<n*m-2*m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			tmp_box = std::make_shared<Atom>(i+1+m+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom1 = std::make_shared<Atom>(i+1+2*m+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+2*m+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			tmp_or = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			tmp_box = std::make_shared<Atom>(i+1+m+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom2 = std::make_shared<Atom>(i+1+2*m+n*m+2*k*n*m);
			tmp_and = std::make_shared<And>(tmp_and, tmp_atom2);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom1);
			tmp_atom1 = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+m+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_not, tmp_eql);
			tmp_atom1 = std::make_shared<Atom>(i+1+2*m+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+2*m+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			res = std::make_shared<And>(res, tmp_or);
		}
		
		for(i=0; i<m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		
		return res;
	
		
	}
	else if(type == 2) { // levo
	
		
		tmp_atom1 = std::make_shared<Atom>(2+2*(k-1)*n*m);
		tmp_atom2 = std::make_shared<Atom>(1+2*k*n*m);
		res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
		
		for(i=2; i<n*m; i++) {
			if(i%m==0)
				continue;
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		for(i=m-1; i<n*m; i+=m) {
			tmp_atom2 = std::make_shared<Atom>(i+1+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			res = std::make_shared<And>(res, tmp_not);
		}


		for(i=0; i<n*m; i++) {
			if(i%m==0 || (i-1)%m==0)
				continue;
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			tmp_box = std::make_shared<Atom>(i+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom1 = std::make_shared<Atom>(i-1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i-1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			tmp_or = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			tmp_box = std::make_shared<Atom>(i+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom2 = std::make_shared<Atom>(i-1+n*m+2*k*n*m);
			tmp_and = std::make_shared<And>(tmp_and, tmp_atom2);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom1);
			tmp_atom1 = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_not, tmp_eql);
			tmp_atom1 = std::make_shared<Atom>(i-1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i-1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			res = std::make_shared<And>(res, tmp_or);
		}

		for(i=m-1; i<n*m; i+=m) {
			tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		
		return res;
		
	}
	else if(type==3) { // desno
		
		tmp_atom1 = std::make_shared<Atom>(1+2*(k-1)*n*m);
		tmp_atom2 = std::make_shared<Atom>(2+2*k*n*m);
		res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
		
		for(i=1; i<n*m; i++) {
			if((i+1)%m==0)
				continue;
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		for(i=0; i<n*m; i+=m) {
			tmp_atom2 = std::make_shared<Atom>(i+1+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			res = std::make_shared<And>(res, tmp_not);
		}


		for(i=0; i<n*m; i++) {
			if((i+1)%m==0 || (i+2)%m==0)
				continue;
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			tmp_box = std::make_shared<Atom>(i+2+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom1 = std::make_shared<Atom>(i+3+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+3+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			tmp_or = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			tmp_box = std::make_shared<Atom>(i+2+n*m+2*k*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			tmp_and = std::make_shared<And>(tmp_and, tmp_not);
			tmp_atom2 = std::make_shared<Atom>(i+3+n*m+2*k*n*m);
			tmp_and = std::make_shared<And>(tmp_and, tmp_atom2);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom1);
			tmp_atom1 = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_not, tmp_eql);
			tmp_atom1 = std::make_shared<Atom>(i+3+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+3+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_and, tmp_eql);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
			
			res = std::make_shared<And>(res, tmp_or);
		}

		for(i=0; i<n*m; i+=m) {
			tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			res = std::make_shared<And>(res, tmp_eql);
		}
		
		return res;
	
	}
	
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
					walls[i*m+j] = true;
					break;
				case 'o':  // box_home
					box_home[i*m+j] = true;
					break;
				case 's':  // Sokoban
					player_coordinates[i*m+j] = true;
					break;
				case 'S':  // Sokoban; box_home
					player_coordinates[i*m+j] = true;
					box_home[i*m+j] = true;
					break;
				case 'b':  // box
					boxes_coordinates[i*m+j] = true;
					break;
				case 'B':  // box; box_home
					boxes_coordinates[i*m+j] = true;
					box_home[i*m+j] = true;
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
			if(walls[i*m+j]) {
				std::cout << "#";
				continue;
			}
			else if(box_home[i*m+j]) {
				if(boxes_coordinates[i*m+j]) 
					std::cout << "B";
				else if(player_coordinates[i*m+j])
					std::cout << "S";
				else
					std::cout << "o";
				continue;
			}
			else if(player_coordinates[i*m+j]) {
				std::cout << "s";
				continue;
			}
			else if(boxes_coordinates[i*m+j]) {
				std::cout << "b";
				continue;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


Formula Sokoban::GeneratePlanFormula() const {
	
	int i;
	
	Formula plan_formula;
	
	Formula tmp_not;
	Formula tmp_box;
	Formula tmp_atom;
	Formula tmp_or;
	Formula tmp;
	Formula tmp_operator;

	
	std::vector<bool> S = current_state->GetPlayerCoordinates(); // [1, n*m]
	std::vector<bool> B = current_state->GetBoxesCoordinates();  // [1+n*m, 2*n*m]
	std::vector<bool> W = table->GetWalls();
	std::vector<bool> BH = table->GetBoxHome();
	
	int n = table->GetN();
	int m = table->GetM();
	
	if(S[0]) {
		plan_formula = std::make_shared<Atom>(1); // i+1+2*(k-1)*n*m
	}
	else {
		tmp_not = std::make_shared<Atom>(1);
		plan_formula = std::make_shared<Not>(tmp_not);
	}
	if(B[0]) {
		tmp_box = std::make_shared<Atom>(1+n*m);
		plan_formula = std::make_shared<And>(plan_formula, tmp_box);
	}
	else {
		tmp_box = std::make_shared<Atom>(1+n*m);
		tmp_not = std::make_shared<Not>(tmp_box);
		plan_formula = std::make_shared<And>(plan_formula, tmp_not);
	}
	for(i=1; i<n*m; i++) {
		if(S[i]) {
			tmp_atom = std::make_shared<Atom>(i+1);
			plan_formula = std::make_shared<And>(plan_formula, tmp_atom);
		}
		else {
			tmp_atom = std::make_shared<Atom>(i+1);
			tmp_not = std::make_shared<Not>(tmp_atom);
			plan_formula = std::make_shared<And>(plan_formula, tmp_not);
		}
		if(B[0]) {
			tmp_box = std::make_shared<Atom>(i+1+n*m);
			plan_formula = std::make_shared<And>(plan_formula, tmp_box);
		}
		else {
			tmp_box = std::make_shared<Atom>(i+1+n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			plan_formula = std::make_shared<And>(plan_formula, tmp_not);
		}
	}
	
	for(i=1; i<=plan_length; i++) {
		tmp_operator = MovePrecondition(0, i, n, m, S, B, W); 
		tmp = MoveEffect(0, i, n, m, S, B);
		tmp_or = std::make_shared<And>(tmp_operator, tmp);
		
		tmp_operator = MovePrecondition(1, i, n, m, S, B, W); 
		tmp = MoveEffect(1, i, n, m, S, B);
		tmp_operator = std::make_shared<And>(tmp_operator, tmp);
		tmp_or = std::make_shared<Or>(tmp_or, tmp_operator);
		
		tmp_operator = MovePrecondition(2, i, n, m, S, B, W); 
		tmp = MoveEffect(2, i, n, m, S, B);
		tmp_operator = std::make_shared<And>(tmp_operator, tmp);
		tmp_or = std::make_shared<Or>(tmp_or, tmp_operator);
		
		tmp_operator = MovePrecondition(3, i, n, m, S, B, W); 
		tmp = MoveEffect(3, i, n, m, S, B);
		tmp_operator = std::make_shared<And>(tmp_operator, tmp);
		tmp_or = std::make_shared<Or>(tmp_or, tmp_operator);
		
		plan_formula = std::make_shared<And>(plan_formula, tmp_or);
		
	}
	
	if(BH[0]) {
		tmp_box = std::make_shared<Atom>(1+n*m+2*plan_length*n*m); 
		plan_formula = std::make_shared<And>(plan_formula, tmp_box);
	}
	else {
		tmp_box = std::make_shared<Atom>(1+n*m+2*plan_length*n*m);
		tmp_not = std::make_shared<Not>(tmp_box);
		plan_formula = std::make_shared<And>(plan_formula, tmp_not);
	}
	
	
	for(i=1; i<n*m; i++) {
			if(BH[i]) {
			tmp_box = std::make_shared<Atom>(i+1+n*m+2*plan_length*n*m); 
			plan_formula = std::make_shared<And>(plan_formula, tmp_box);
		}
		else {
			tmp_box = std::make_shared<Atom>(i+1+n*m+2*plan_length*n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			plan_formula = std::make_shared<And>(plan_formula, tmp_not);
		}
	}
	
	
	return plan_formula;
}


