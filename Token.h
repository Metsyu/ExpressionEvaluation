#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <cctype>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <stack>
#include <algorithm>
#include <queue>
#include <deque>
#include <map>
#include <utility>

using namespace std;

enum Token_type {Identifier, Integer, Operators, EqualSign,
	OpenBrace, CloseBrace, INVALID};

class Token
{
private:
	Token_type type;
	string token;
	int priority;

public:
	Token();
	explicit Token(string s);
	void set(string s);
	int value() const;
	void display() const;
	Token_type get_type() const;
	string get_token() const;
	int get_priority() const;
};

#endif //TOKEN_H
