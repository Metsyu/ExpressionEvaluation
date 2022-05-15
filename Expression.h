#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Token.h"

enum Exp_type {Assignment, Arithmetic, Bad};

class Expression
{
private:
	string original;
	vector<Token> tokenized;
	vector<Token> postfix;
	vector<Token> prefix;
	bool valid;
	Exp_type type;

public:
	Expression();
	Expression(const string& s);
	void set(const string& s);
	void set_postfix();
	void set_prefix();
	void parenthesize();
	void set_valid_type();
	void display() const;
	void display_postfix() const;
	void display_prefix() const;
	void evaluate(const map<string, int>& IDs) const;
	string get_original() const;
	vector<Token> get_tokenized() const;
	bool get_valid() const;
	Exp_type get_type() const;
};

#endif //EXPRESSION_H
