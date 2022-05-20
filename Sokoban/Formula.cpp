
#include "Formula.hpp"

#include <iostream>

void Atom::printFormula() const {
	std::cout << n;
}

void Not::printFormula() const {
	std::cout << "~(";
	f->printFormula();
	std::cout << ")";
}

void And::printFormula() const {
	std::cout << "(";
	l->printFormula();
	std::cout << ")";
	std::cout << " && ";
	std::cout << "(";
	r->printFormula();
	std::cout << ")";
}

void Or::printFormula() const {
	std::cout << "(";
	l->printFormula();
	std::cout << ")";
	std::cout << " || ";
	std::cout << "(";
	r->printFormula();
	std::cout << ")";
}

void Impl::printFormula() const {
	std::cout << "(";
	l->printFormula();
	std::cout << ")";
	std::cout << " => ";
	std::cout << "(";
	r->printFormula();
	std::cout << ")";
}

void Eql::printFormula() const {
	std::cout << "(";
	l->printFormula();
	std::cout << ")";
	std::cout << " <=> ";
	std::cout << "(";
	r->printFormula();
	std::cout << ")";
}

Not::~Not() {
	delete f;
}

And::~And() {
	delete l;
	delete r;
}

Or::~Or() {
	delete l;
	delete r;
}

Impl::~Impl() {
	delete l;
	delete r;
}

Eql::~Eql() {
	delete l;
	delete r;
}

