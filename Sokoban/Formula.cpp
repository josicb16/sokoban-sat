
#include "Formula.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>

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


