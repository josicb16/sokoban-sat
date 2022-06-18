#include "Sokoban.hpp"
#include "Formula.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


const std::vector<bool> & Table::GetWalls() const { return walls; }
const std::vector<bool> & Table::GetBoxHome() const { return box_home;}
const std::vector<bool> & Table::GetPlayerCoordinates() const { return player_coordinates; }
const std::vector<bool> & Table::GetBoxesCoordinates() const { return boxes_coordinates; }
int Table::GetN() const { return n; }
int Table::GetM() const { return m; }


static Formula MovePrecondition(int type, int k, int n, int m, const std::vector<bool> &W) {
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

static Formula SetBoxes(int type, int k, int n, int m, int b=-1) {
	int i;
	
	Formula res;
	Formula tmp_atom1;
	Formula tmp_atom2;
	Formula tmp_eql;
	Formula tmp_not;
	
	
	if(b==-1) {  // raspored kutija ostaje nepromenjen
		tmp_atom1 = std::make_shared<Atom>(1+n*m+2*(k-1)*n*m);
		tmp_atom2 = std::make_shared<Atom>(1+n*m+2*k*n*m);
		res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);

		for(i=1; i<n*m; i++) {
			tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
			tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			res = std::make_shared<And>(res, tmp_eql);
		}
		
		return res;
	}
	else {
		// kutija na poziciji b se pomera, u zavisnosti od tipa operatora;
		// pozicije ostalih kutija se ne menjaju.
		if(type == 0) { // gore
			tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(b+1-m+n*m+2*k*n*m);
			
			res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			for(i=0; i<n*m; i++) {
				if(i==b) {
					tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*k*n*m);
					tmp_not = std::make_shared<Not>(tmp_atom1);
					res = std::make_shared<And>(res, tmp_not);
					continue;
				}
				if(i==b-m)
					continue;
				tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
				tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
				tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
				
				res = std::make_shared<And>(res, tmp_eql);
			}
			
			return res;
			
		}
		else if(type == 1) { // dole
			tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(b+1+m+n*m+2*k*n*m);
			
			res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			for(i=0; i<n*m; i++) {
				if(i==b) {
					tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*k*n*m);
					tmp_not = std::make_shared<Not>(tmp_atom1);
					res = std::make_shared<And>(res, tmp_not);
					continue;
				}
				if(i==b+m)
					continue;
				tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
				tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
				tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
				
				res = std::make_shared<And>(res, tmp_eql);
			}
			
			return res;
		}
		else if(type == 2) { // levo
		
			tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(b+n*m+2*k*n*m);
			
			res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			for(i=0; i<n*m; i++) {
				if(i==b) {
					tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*k*n*m);
					tmp_not = std::make_shared<Not>(tmp_atom1);
					res = std::make_shared<And>(res, tmp_not);
					continue;
				}
				if(i==b-1)
					continue;
				tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
				tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
				tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
				
				res = std::make_shared<And>(res, tmp_eql);
			}
			
			return res;
			
		}
		else if(type == 3) { // desno
		
			tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(b+2+n*m+2*k*n*m);
			
			res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
			
			for(i=0; i<n*m; i++) {
				if(i==b) {
					tmp_atom1 = std::make_shared<Atom>(b+1+n*m+2*k*n*m);
					tmp_not = std::make_shared<Not>(tmp_atom1);
					res = std::make_shared<And>(res, tmp_not);
					continue;
				}
				if(i==b+1)
					continue;
				tmp_atom1 = std::make_shared<Atom>(i+1+n*m+2*(k-1)*n*m);
				tmp_atom2 = std::make_shared<Atom>(i+1+n*m+2*k*n*m);
				tmp_eql = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
				
				res = std::make_shared<And>(res, tmp_eql);
			}
			
			return res;
			
		}
		
	}
	
}

static Formula MoveEffect(int type, int k, int n, int m) {
	
	int i;
	
	Formula res;
	
	Formula tmp_atom1;
	Formula tmp_atom2;
	Formula tmp_box;
	Formula tmp_and;
	Formula tmp_eql;
	Formula tmp_impl;
	Formula tmp_not;
	Formula tmp;
	Formula tmp_or;
	
	Formula set_boxes;
	
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
		tmp_or = std::make_shared<False>();
		for(i=m; i<n*m; i++) {
			// iznad sokobana nije kutija
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			
			set_boxes = SetBoxes(0, k, n, m);  // raspored kutija se ne menja
		
			tmp = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp);
			
		}
		
		for(i=2*m; i<n*m; i++) {
			// iznad sokobana je kutija. 
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1-m+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			
			set_boxes = SetBoxes(0, k, n, m, i-m);
			
			tmp_and = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
		}
		
		res = std::make_shared<And>(res, tmp_or);
		
		return res;
		
	}
	else if(type==1) {  // dole
		
		tmp_atom1 = std::make_shared<Atom>(1+2*(k-1)*n*m);
		tmp_atom2 = std::make_shared<Atom>(1+m+2*k*n*m);
		res = std::make_shared<Eql>(tmp_atom1, tmp_atom2);
		
		for(i=1; i<n*m-m; i++) {
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


		// Ako ispod Sokobana nije bila kutija, raspored kutija ostaje nepromenjen,
		// a ako je ispod Sokobana bila kutija, ona se pomera za jedno mesto dole
		tmp_or = std::make_shared<False>();
		for(i=0; i<n*m-m; i++) {
			// ispod sokobana nije kutija
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			
			set_boxes = SetBoxes(1, k, n, m);  // raspored kutija se ne menja
		
			tmp = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp);
			
		}
		
		for(i=0; i<n*m-2*m; i++) {
			// ispod sokobana je kutija. 
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+1+m+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			
			set_boxes = SetBoxes(1, k, n, m, i+m);
			
			tmp_and = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
		}
		
		res = std::make_shared<And>(res, tmp_or);
		
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
		
		
		// Ako levo od Sokobana nije bila kutija, raspored kutija ostaje nepromenjen,
		// a ako je levo od Sokobana bila kutija, ona se pomera za jedno mesto ulevo
		tmp_or = std::make_shared<False>();
		for(i=0; i<n*m; i++) {
			if(i%m==0)
				continue;
			// levo od sokobana nije kutija
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			
			set_boxes = SetBoxes(2, k, n, m);  // raspored kutija se ne menja
		
			tmp = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp);
			
		}
		
		for(i=0; i<n*m; i++) {
			if(i%m==0 || (i-1)%m==0)
				continue;
			// levo od sokobana je kutija. 
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			
			set_boxes = SetBoxes(2, k, n, m, i-1);
			
			tmp_and = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
		}
		
		res = std::make_shared<And>(res, tmp_or);
		
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

		
		// Ako levo od Sokobana nije bila kutija, raspored kutija ostaje nepromenjen,
		// a ako je levo od Sokobana bila kutija, ona se pomera za jedno mesto ulevo
		tmp_or = std::make_shared<False>();
		for(i=0; i<n*m; i++) {
			if((i+1)%m==0)
				continue;
			// levo od sokobana nije kutija
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
			tmp_not = std::make_shared<Not>(tmp_atom2);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_not);
			
			set_boxes = SetBoxes(3, k, n, m);  // raspored kutija se ne menja
		
			tmp = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp);
			
		}
		
		for(i=0; i<n*m; i++) {
			if((i+1)%m==0 || (i+2)%m==0)
				continue;
			// levo od sokobana je kutija. 
			tmp_atom1 = std::make_shared<Atom>(i+1+2*(k-1)*n*m);
			tmp_atom2 = std::make_shared<Atom>(i+2+n*m+2*(k-1)*n*m);
			tmp_and = std::make_shared<And>(tmp_atom1, tmp_atom2);
			
			set_boxes = SetBoxes(3, k, n, m, i+1);
			
			tmp_and = std::make_shared<And>(tmp_and, set_boxes);
			
			tmp_or = std::make_shared<Or>(tmp_or, tmp_and);
		}
		
		res = std::make_shared<And>(res, tmp_or);
		
		return res;
	
	}
	
}


Table *Sokoban::GetTable() const {
	return table;
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
	table = new Table(n, m, walls, box_home, player_coordinates, boxes_coordinates);
}

Sokoban::~Sokoban() {
	delete table;
}

void Sokoban::PrintTable() const {
	const std::vector<bool> & walls = table->GetWalls();
	const std::vector<bool> & box_home = table->GetBoxHome();
	const std::vector<bool> & player_coordinates = table->GetPlayerCoordinates();
	const std::vector<bool> & boxes_coordinates = table->GetBoxesCoordinates();
	
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


void Sokoban::GeneratePlanFormula(std::ofstream &dimacs) const {
	
	int i;
	
	Formula plan_formula;
	
	Formula tmp_not;
	Formula tmp_box;
	Formula tmp_atom;
	Formula tmp_or;
	Formula tmp;
	Formula tmp_operator;

	
	const std::vector<bool> & S = table->GetPlayerCoordinates();
	const std::vector<bool> & B = table->GetBoxesCoordinates();
	const std::vector<bool> & W = table->GetWalls();
	const std::vector<bool> & BH = table->GetBoxHome();
	
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
		if(B[i]) {
			tmp_box = std::make_shared<Atom>(i+1+n*m);
			plan_formula = std::make_shared<And>(plan_formula, tmp_box);
		}
		else {
			tmp_box = std::make_shared<Atom>(i+1+n*m);
			tmp_not = std::make_shared<Not>(tmp_box);
			plan_formula = std::make_shared<And>(plan_formula, tmp_not);
		}
	}
	
	int atom = 2*n*m*(plan_length+1)+1;
	
	int number_of_clauses = 0;
	
 	std::string dimacs_str = plan_formula->Dimacs(atom, number_of_clauses);
	
	
	for(i=1; i<=plan_length; i++) {
		tmp_operator = MovePrecondition(0, i, n, m, W); 
		tmp = MoveEffect(0, i, n, m);
		tmp_or = std::make_shared<And>(tmp_operator, tmp);
		
		
		tmp_operator = MovePrecondition(1, i, n, m, W); 
		tmp = MoveEffect(1, i, n, m);
		tmp_operator = std::make_shared<And>(tmp_operator, tmp);
		tmp_or = std::make_shared<Or>(tmp_or, tmp_operator);
		
		tmp_operator = MovePrecondition(2, i, n, m, W); 
		tmp = MoveEffect(2, i, n, m);
		tmp_operator = std::make_shared<And>(tmp_operator, tmp);
		tmp_or = std::make_shared<Or>(tmp_or, tmp_operator);
		
		tmp_operator = MovePrecondition(3, i, n, m, W); 
		tmp = MoveEffect(3, i, n, m);
		tmp_operator = std::make_shared<And>(tmp_operator, tmp);
		plan_formula = std::make_shared<Or>(tmp_or, tmp_operator);
		
		dimacs_str = dimacs_str + plan_formula->Dimacs(atom, number_of_clauses);
		
	}
	
	if(BH[0]) {
		plan_formula = std::make_shared<Atom>(1+n*m+2*plan_length*n*m); 
	}
	else {
		tmp_box = std::make_shared<Atom>(1+n*m+2*plan_length*n*m);
		plan_formula = std::make_shared<Not>(tmp_box);
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
	
	dimacs_str = dimacs_str + plan_formula->Dimacs(atom, number_of_clauses);
	
	int number_of_variables = --atom;
	
	dimacs << "p cnf " + std::to_string(number_of_variables) + " " + std::to_string(number_of_clauses) + "\n";
	dimacs << dimacs_str;
	
	
}

