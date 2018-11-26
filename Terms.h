#pragma once
#include <string>
#include <vector>
#include <cstdarg>
#include <iostream>
using namespace std;
enum class term_type
{
	Function,
	Operator,
	Operand,
	Bracket,
	Variable,
	Comma,
	others
};
class term
{
public:
	term_type t;
	string val;
	term(term_type _t, string _val)
	{
		t = _t;
		val = _val;
	}
};
class Variable
{
public:
	Variable(string, double);
	Variable()
	{
	}
	string name;
	double val;
	void changeVal(double newVal)
	{
		val = newVal;
	}
};
class Function
{
public:
	Function(string,int, double(*_calc)(vector<double> x));
	Function() {};
	string name;
	int variable_count;
	double(*calc)(vector<double> x);
};
class Operator
{
public:
	Operator(string s, Function, int, bool ass = 0);
	Operator() {};
	string name;
	Function f;
	int priority;

	// 0 for right to left
	// 1 for left to right
	bool associtivity;
};