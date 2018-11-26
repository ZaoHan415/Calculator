#include "Terms.h"

Variable::Variable(string s, double x)
{
	name = s;
	val = x;
}
Function::Function(string s,int i, double(*_calc)(vector<double> x))
{
	name = s;
	calc = _calc;
	variable_count = i;
}

Operator::Operator(string s,Function _f,int p,bool _ass)
{
	name = s;
	f = _f;
	priority = p;
	associtivity = _ass;
}