#pragma once
#include "Terms.h"
#include <map>
#include <regex>

//This class is initialized by an expression in the form of a string 
//functions and operators can be added into this class
//the most commonly used functions and operators are defined in "MyOperators.h"
//They are added during the constructing process of an object
class ExprCalculator
{
private:
	//variables, functions or operators are distincted by a string 

	map<string, Function> function_list;
	map<string, Operator> operator_list;
	map<string, Variable> variable_list;

	term_type determine(string s);

	//store the pre-treatment expression(simply split it by word)
	vector<term> expr;

	//split the whole expression to words
	void format();

	//store the post-fix form
	vector<term> polish_expr;

	//add the most commonly used functions to my calculator
	void initList();

	string match(string s, term_type t);

	string *mainStr = nullptr;

	//express expr in polish notation(prefix notation)
	//we can build a expression binary-tree after this
	void Polish();

	double postFixCalculate(vector<term> post_expr);
	double postFixCalculate();

public:
	Variable * getVariable(string name);
	ExprCalculator(string *s);
	void restart(string *s);
	void addVariable(string name, double val);
	void addFunction(Function);
	void addOperator(Operator);
	string answer();
};

