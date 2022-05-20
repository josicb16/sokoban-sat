#ifndef __FORMULA_HPP__
#define __FORMULA_HPP__

#include <vector>
#include <memory>
#include <string>
#include <set>

class BaseFormula;
using Formula = std::shared_ptr<BaseFormula>;

class BaseFormula {
	public:
		virtual ~BaseFormula()     {	}
		virtual void PrintFormula() const = 0;
		virtual std::string GetType() const = 0;
		virtual Formula ToNNF() const= 0;
		virtual Formula GetFormulaData() const {	}
		virtual Formula GetFormulaDataL() const {	}
		virtual Formula GetFormulaDataR() const {	}
};



class True: public BaseFormula {
	public:
		True() {	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula ToNNF() const;
};


class False : public BaseFormula {
	public: 
		False() {	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula ToNNF() const;
};


class Atom : public BaseFormula {
	public:
		Atom(int _n) : n(_n) {		}
		void PrintFormula() const;
		std::string GetType() const;
		Formula ToNNF() const;
	private:
		int n;
};


class Not : public BaseFormula {
	public:
		Not(Formula _f) : f(_f) 	{	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula GetFormulaData() const;
		Formula ToNNF() const;
	private:
		Formula f;
};


class Binary: public BaseFormula {
	public:
		virtual ~Binary() {		}
};


class And : public Binary {
	public:
		And(Formula _l, Formula _r) : l(_l), r(_r)   {	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula GetFormulaDataL() const;
		Formula GetFormulaDataR() const;
		Formula ToNNF() const;
	private:
		Formula l;
		Formula r;
}; 


class Or : public Binary {
	public:
		Or(Formula _l, Formula _r) : l(_l), r(_r)   {	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula GetFormulaDataL() const;
		Formula GetFormulaDataR() const;
		Formula ToNNF() const;
	private:
		Formula l;
		Formula r;
};


class Impl : public Binary {
	public:
		Impl(Formula _l, Formula _r) : l(_l), r(_r)   {	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula GetFormulaDataL() const;
		Formula GetFormulaDataR() const;
		Formula ToNNF() const;
	private:
		Formula l;
		Formula r;
};


class Eql : public Binary {
	public:
		Eql(Formula _l, Formula _r) : l(_l), r(_r)   {	}
		void PrintFormula() const;
		std::string GetType() const;
		Formula GetFormulaDataL() const;
		Formula GetFormulaDataR() const;
		Formula ToNNF() const;
	private:
		Formula l;
		Formula r;
};



#endif