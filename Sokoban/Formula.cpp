
#include "Formula.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <cstdlib>
#include <algorithm>

std::string True::GetType() const { return "true"; }
std::string False::GetType() const { return "false"; }
std::string Atom::GetType() const { return "atom"; }
std::string Not::GetType() const { return "not"; }
std::string And::GetType() const { return "and"; }
std::string Or::GetType() const { return "or"; }
std::string Impl::GetType() const { return "impl"; }
std::string Eql::GetType() const { return "eql"; }

Formula Not::GetFormulaData() const { return f; }

Formula And::GetFormulaDataL() const { return l; }
Formula Or::GetFormulaDataL() const { return l; }
Formula Impl::GetFormulaDataL() const { return l; }
Formula Eql::GetFormulaDataL() const { return l; }

Formula And::GetFormulaDataR() const { return r; }
Formula Or::GetFormulaDataR() const { return r; }
Formula Impl::GetFormulaDataR() const { return r; }
Formula Eql::GetFormulaDataR() const { return r; }


void Atom::GetAtoms(std::set<int>& atoms) const {
	atoms.insert(n);
	return;
}
void Not::GetAtoms(std::set<int>& atoms) const {
	f->GetAtoms(atoms);
	return;
}
void And::GetAtoms(std::set<int>& atoms) const {
	l->GetAtoms(atoms);
	r->GetAtoms(atoms);
	return;
}
void Or::GetAtoms(std::set<int>& atoms) const {
	l->GetAtoms(atoms);
	r->GetAtoms(atoms);
	return;
}
void Impl::GetAtoms(std::set<int>& atoms) const {
	l->GetAtoms(atoms);
	r->GetAtoms(atoms);
	return;
}
void Eql::GetAtoms(std::set<int>& atoms) const {
	l->GetAtoms(atoms);
	r->GetAtoms(atoms);
	return;
}


void True::PrintFormula() const {
	std::cout << "T";
}
void False::PrintFormula() const {
	std::cout << "F";
}
void Atom::PrintFormula() const {
	std::cout << n;
}
void Not::PrintFormula() const {
	std::cout << "~(";
	f->PrintFormula();
	std::cout << ")";
}
void And::PrintFormula() const {
	std::cout << "(";
	l->PrintFormula();
	std::cout << ")";
	std::cout << " && ";
	std::cout << "(";
	r->PrintFormula();
	std::cout << ")";
}
void Or::PrintFormula() const {
	std::cout << "(";
	l->PrintFormula();
	std::cout << ")";
	std::cout << " || ";
	std::cout << "(";
	r->PrintFormula();
	std::cout << ")";
}
void Impl::PrintFormula() const {
	std::cout << "(";
	l->PrintFormula();
	std::cout << ")";
	std::cout << " => ";
	std::cout << "(";
	r->PrintFormula();
	std::cout << ")";
}
void Eql::PrintFormula() const {
	std::cout << "(";
	l->PrintFormula();
	std::cout << ")";
	std::cout << " <=> ";
	std::cout << "(";
	r->PrintFormula();
	std::cout << ")";
}


Formula True::ToNNF() const {
	return std::make_shared<True>();
}
Formula False::ToNNF() const {
	return std::make_shared<False>();
}
Formula Atom::ToNNF() const {
	return std::make_shared<Atom>(n);
}
Formula Not::ToNNF() const {
	std::string type = f->GetType();
	if(type=="true")
		return std::make_shared<False>();
	else if(type=="false")
		return std::make_shared<True>();
	else if(type=="atom")
		return std::make_shared<Not>(f);
	else if(type=="not")
		return f->GetFormulaData();
	else if(type=="and") {
		Formula l = f->GetFormulaDataL();
		Formula r = f->GetFormulaDataR();
		Formula l1 = std::make_shared<Not>(l);
		Formula r1 = std::make_shared<Not>(r);
		Formula l1_tmp = l1->ToNNF();
		Formula r1_tmp = r1->ToNNF();
		return std::make_shared<Or>(l1_tmp, r1_tmp);
	}
	else if(type=="or") {
		Formula l = f->GetFormulaDataL();
		Formula r = f->GetFormulaDataR();
		Formula l1 = std::make_shared<Not>(l);
		Formula r1 = std::make_shared<Not>(r);
		Formula l1_tmp = l1->ToNNF();
		Formula r1_tmp = r1->ToNNF();
		return std::make_shared<And>(l1_tmp, r1_tmp);
	}
	else if(type=="impl") {
		Formula l = f->GetFormulaDataL();
		Formula r = f->GetFormulaDataR();
		Formula l1_tmp = l->ToNNF();
		Formula r1 = std::make_shared<Not>(r);
		Formula r1_tmp = r1->ToNNF();
		return std::make_shared<And>(l1_tmp, r1_tmp);
	}
	else if(type=="eql") {
		Formula l = f->GetFormulaDataL();
		Formula r = f->GetFormulaDataR();
		Formula l1 = std::make_shared<Impl>(l, r);
		Formula r1 = std::make_shared<Impl>(r, l);
		Formula tmp = std::make_shared<And>(l1, r1);
		return tmp->ToNNF();
	}
}
Formula And::ToNNF() const {
	Formula l1 = l->ToNNF();
	Formula r1 = r->ToNNF();
	return std::make_shared<And>(l1, r1);
}
Formula Or::ToNNF() const {
	Formula l1 = l->ToNNF();
	Formula r1 = r->ToNNF();
	return std::make_shared<Or>(l1, r1);
}
Formula Impl::ToNNF() const {
	Formula l1 = std::make_shared<Not>(l);
	Formula l1_tmp = l1->ToNNF();
	Formula r1_tmp = r->ToNNF();
	return std::make_shared<Or>(l1_tmp, r1_tmp);
}
Formula Eql::ToNNF() const {
	Formula l1 = std::make_shared<Impl>(l, r);
	Formula r1 = std::make_shared<Impl>(r, l);
	Formula tmp = std::make_shared<And>(l1, r1);
	return tmp->ToNNF();
}



int True::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	cnf.push_back({atom});
	return atom++;
}

int False::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	cnf.push_back({-atom});
	return atom++;
}

int Atom::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	return n;
}

int Not::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	int subformula = f->ToTseitinCNF(cnf, atom);
	std::vector<std::vector<int>> tseitin_not = {{-subformula, -atom}, {subformula, atom}};
	std::copy(begin(tseitin_not), end(tseitin_not), std::back_inserter(cnf));
	return atom++;
}

int And::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	int l_atom = l->ToTseitinCNF(cnf, atom);
	int r_atom = r->ToTseitinCNF(cnf, atom);
	
	std::vector<std::vector<int>> tseitin_and = {{-atom, l_atom}, {-atom, r_atom}, {atom, -l_atom, -r_atom}};
	
	std::copy(begin(tseitin_and), end(tseitin_and), std::back_inserter(cnf));
	return atom++;
}

int Or::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
    int l_atom = l->ToTseitinCNF(cnf, atom);
	int r_atom = r->ToTseitinCNF(cnf, atom);
	
	std::vector<std::vector<int>> tseitin_or = {{-atom, l_atom, r_atom}, {atom, -l_atom}, {atom, -r_atom}};
	
	std::copy(begin(tseitin_or), end(tseitin_or), std::back_inserter(cnf));
	return atom++;
}

int Impl::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	int l_atom = l->ToTseitinCNF(cnf, atom);
	int r_atom = r->ToTseitinCNF(cnf, atom);
	
	std::vector<std::vector<int>> tseitin_impl = {{-atom, -l_atom, r_atom}, {atom, l_atom}, {atom, -r_atom}};
	
	std::copy(begin(tseitin_impl), end(tseitin_impl), std::back_inserter(cnf));
	return atom++;
}

int Eql::ToTseitinCNF(std::vector<std::vector<int>>& cnf, int& atom) const {
	int l_atom = l->ToTseitinCNF(cnf, atom);
	int r_atom = r->ToTseitinCNF(cnf, atom);
	
	std::vector<std::vector<int>> tseitin_eql = {{-atom, -l_atom, r_atom}, {-atom, l_atom, -r_atom}, {atom, l_atom, r_atom}, {atom, -l_atom, -r_atom}};
	
	std::copy(begin(tseitin_eql), end(tseitin_eql), std::back_inserter(cnf));
	return atom++;
}


std::string BaseFormula::Dimacs() const {
	Formula f = this->ToNNF();
	
	std::set<int> atoms;
	f->GetAtoms(atoms);
	int atom = *atoms.rbegin() + 1;
	
	std::vector<std::vector<int>> cnf;
	int s = f->ToTseitinCNF(cnf, atom);
	cnf.push_back({s});
	
	if(cnf.empty())
		return "p cnf 0 0";
	
	int number_of_clauses = cnf.size();
	
	std::set<unsigned> variables;
	int i;
	for(i=0; i<number_of_clauses; i++) {
		auto it = cnf[i].cbegin();
		auto it_end = cnf[i].cend();
		while(it != it_end) {
			variables.insert(abs(*it));
			it++;
		}
	}
	
	int number_of_variables = variables.size();
	
	std::string rez = "p cnf " + std::to_string(number_of_variables) + " " + std::to_string(number_of_clauses) + "\n";
	
	for(i=0; i<number_of_clauses; i++) {
		auto it = cnf[i].cbegin();
		auto it_end = cnf[i].cend();
		while(it != it_end) {
			rez += std::to_string(*it) + " ";
			it++;
		}
		rez += "0\n";
	}
	
	return rez;
}

