#ifndef __FORMULA_HPP__
#define __FORMULA_HPP__

class Formula {
	public:
		virtual ~Formula()     {	}
		virtual void printFormula() const = 0;
};


class Atom : public Formula {
	public:
		Atom(unsigned _n) : n(_n) {		}
		void printFormula() const;
	private:
		unsigned n;
};


class Not : public Formula {
	public:
		Not(Formula *_f) : f(_f) 	{	}
		~Not();
		void printFormula() const;
	private:
		Formula *f;
};


class Binary: public Formula {
	public:
		virtual ~Binary() {		}
};


class And : public Binary {
	public:
		And(Formula *_l, Formula *_r) : l(_l), r(_r)   {	}
		~And();
		void printFormula() const;
	
	private:
		Formula *l;
		Formula *r;
}; 


class Or : public Binary {
	public:
		Or(Formula *_l, Formula *_r) : l(_l), r(_r)   {	}
		~Or();
		void printFormula() const;
	
	private:
		Formula *l;
		Formula *r;
};


class Impl : public Binary {
	public:
		Impl(Formula *_l, Formula *_r) : l(_l), r(_r)   {	}
		~Impl();
		void printFormula() const;
	
	private:
		Formula *l;
		Formula *r;
};


class Eql : public Binary {
	public:
		Eql(Formula *_l, Formula *_r) : l(_l), r(_r)   {	}
		~Eql();
		void printFormula() const;
	
	private:
		Formula *l;
		Formula *r;
};



#endif