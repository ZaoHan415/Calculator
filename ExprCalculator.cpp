#include "ExprCalculator.h"
#include <cmath>
#include <sstream>
#include <stack>
#include "MyOperators.h"
string dtos(double x)
{
	stringstream ss;
	ss << x;
	return ss.str();
}
string dtos(char x)
{
	stringstream ss;
	ss << x;
	return ss.str();
}
void ExprCalculator::initList()
{
	addOperator(Operator("+", Function("add", 2, &_add), 4));
	addOperator(Operator("-", Function("sub", 2, &_sub), 4));
	addOperator(Operator("/", Function("div", 2, &_div), 3));
	addOperator(Operator("*", Function("mul", 2, &_mul), 3));
	addOperator(Operator("^", Function("pow", 2, &_pow), 3,true));
	addOperator(Operator("!", Function("fac", 1, &_fact), 2));
	addOperator(Operator("%", Function("mod", 2, &_fmod), 3));
	addFunction(Function("sin", 1, &_sin));
	addFunction(Function("cos", 1, &_cos));
	addFunction(Function("sqrt", 1, &_sqrt));
	addFunction(Function("log", 1, &_log));
	addVariable("pi", pi);
	addVariable("e", e);
}

ExprCalculator::ExprCalculator(string *s)
{
	initList();
	mainStr = s;
}

void ExprCalculator::format()
{
	for (unsigned int i = 0; i < mainStr->length(); i++)
	{
		char c = mainStr->at(i);
		bool temp_flag = c == '-' &&
			(expr.empty() || expr[expr.size() - 1].t != term_type::Operand
				|| expr[expr.size() - 1].t != term_type::Operator);
		if (isblank(c) )
		{
			continue;
		}
		else if (c == ';')
		{
			break;
		}
		else if (isalpha(c) || c == '_')
		{
			string temp = match(mainStr->substr(i), term_type::Variable);
			term_type m_type = determine(temp);
			expr.push_back(term(m_type, temp));
			i = i + temp.length() - 1;
		}
		else if (isdigit(c) || c == '.' || temp_flag)
		{
			string temp = match(mainStr->substr(i), term_type::Operand);
			expr.push_back(term(term_type::Operand, temp));
			i = i + temp.length() - 1;
		}
		else
		{
			string c_str = dtos(c);
			switch (c)
			{
			case '(':
			case ')':
				expr.push_back(term(term_type::Bracket, c_str));
				break;
			case ',':
				expr.push_back(term(term_type::Comma, c_str));
				break;
			default:
				if (operator_list.count(c_str))
					expr.push_back(term(term_type::Operator, c_str));
				else
					throw runtime_error("Unknown symbol");
					
				break;
			}
		}
	}
}

term_type ExprCalculator::determine(string s)
{
	bool is_function = function_list.count(s);
	bool is_variable = variable_list.count(s);
	if (is_function&&is_variable)
	{
		throw runtime_error("conflict names");
	}
	else if (is_function)
	{
		return term_type::Function;
	}
	else if (is_variable)
	{
		return term_type::Variable;
	}
	else
	{
		throw runtime_error("Unknown Symbol");
	}
}

string ExprCalculator::match(string s,term_type t)
{
	if (t == term_type::Variable)
	{
		regex r_name("^[a-za-z_][a-za-z0-9_]*");
		smatch m;
		regex_search(s, m, r_name);
		return m.begin()->str();
	}
	else if (t == term_type::Operand)
	{
		regex r_double("^[-+]?[0-9]*\\.?[0-9]+");
		smatch m;
		regex_search(s, m, r_double);
		return m.begin()->str();
	}
	else
	{
		throw runtime_error("type not defined");
	}
}

void ExprCalculator::addFunction(Function f)
{
	if (function_list.count(f.name) > 0)
	{
		throw runtime_error("defination conflict :" + f.name);
	}
	function_list[f.name] = f;
}

void ExprCalculator::addOperator(Operator op)
{
	if (operator_list.count(op.name) > 0)
	{
		throw runtime_error("defination conflict :" + op.name);
	}
	operator_list[op.name] = op;
}

void ExprCalculator::Polish()
{
	stack<term> sta;
	for (unsigned int i = 0; i < expr.size(); i++)
	{
		term te = expr[i];
		switch (te.t)
		{
		case term_type::Variable:
		{
			Variable v(variable_list[te.val]);
			te = term(term_type::Variable, dtos(v.val));
		}
		case term_type::Operand:
			polish_expr.push_back(te);
			break;
		case term_type::Function:
			sta.push(te);
			break;
		case term_type::Comma:
			if (sta.empty())
			{
				throw runtime_error("illegal expression");
			}
			while (sta.top().val != "(")
			{
				polish_expr.push_back(sta.top());
				sta.pop();
				if (sta.empty())
				{
					throw runtime_error("illegal expression");
				}
			}
			break;
		case term_type::Operator:
			while (!sta.empty() && sta.top().t == term_type::Operator)
			{
				Operator op1 = operator_list[te.val];
				Operator op2 = operator_list[sta.top().val];
				bool flag = (op1.associtivity == false &&
					op1.priority >= op2.priority) || (op1.associtivity == true
						&& op1.priority > op2.priority);
				if (flag)
				{
					polish_expr.push_back(sta.top());
					sta.pop();
				}
				else
					break;
			}
			sta.push(te);
			break;
		case term_type::Bracket:
			if (te.val == "(")
			{
				sta.push(te);
			}
			else if (te.val == ")")
			{
				bool left_bra_exist = false;
				while (!sta.empty())
				{
					term top_term = sta.top();
					if (top_term.t == term_type::Operator)
					{
						polish_expr.push_back(top_term);
						sta.pop();
					}
					else if (top_term.val == "(")
					{
						left_bra_exist = true;
						break;
					}
					else
					{
						throw runtime_error("illegal expression");
					}
				}
				if (left_bra_exist == false)
				{
					throw runtime_error("brackets mismatch");
				}
				else
				{
					sta.pop();
					if (!sta.empty() && sta.top().t == term_type::Function)
					{
						polish_expr.push_back(sta.top());
						sta.pop();
					}
				}
			}
			break;
		default:
			throw("unkown error");
			break;
		}
	}
	while (!sta.empty())
	{
		if (sta.top().t == term_type::Bracket)
		{
			throw runtime_error("brackets mismatch");
		}
		else
		{
			polish_expr.push_back(sta.top());
			sta.pop();
		}
	}
}

double ExprCalculator::postFixCalculate(vector<term> post_expr)
{
	stack<term> temp_stack;
	for (unsigned int i = 0; i < post_expr.size(); i++)
	{
		term now_term = post_expr[i];
		if (now_term.t == term_type::Operand || now_term.t == term_type::Variable)
		{
			temp_stack.push(now_term);
		}
		else 
		{
			double ans;
			if (now_term.t == term_type::Operator)
			{
				vector<double> x;
				Operator now_op = operator_list[now_term.val];
				for (int i = 0; i < now_op.f.variable_count; i++)
				{
					if (temp_stack.empty())
					{
						throw runtime_error("Grammar error");
					}
					x.push_back(stod(temp_stack.top().val));
					temp_stack.pop();
				}
				ans = now_op.f.calc(x);
			}
			else if (now_term.t == term_type::Function)
			{
				vector<double> x;
				Function f = function_list[now_term.val];
				for (int i = 0; i < f.variable_count; i++)
				{
					if (temp_stack.empty())
					{
						throw runtime_error("Grammar error");
					}
					x.push_back(stod(temp_stack.top().val));
					temp_stack.pop();
				}
				ans = f.calc(x);
			}
			term now_t(term_type::Operand,dtos(ans));
			temp_stack.push(now_t);
		}
	}
	if (temp_stack.size() == 1)
	{
		if (temp_stack.top().t == term_type::Operand || temp_stack.top().t == term_type::Variable)
		{
			return stod(temp_stack.top().val);
		}
	}
	else
	{
		throw runtime_error("illegal expression");
	}
}

double ExprCalculator::postFixCalculate()
{
	return postFixCalculate(polish_expr);
}

string ExprCalculator::answer()
{
	format();
	Polish();
	return dtos(postFixCalculate());
}

void ExprCalculator::addVariable(string name, double val)
{
	if (variable_list.count(name))
	{
		throw runtime_error("used variable name :"+name);
	}
	variable_list[name] = Variable(name, val);
}

void ExprCalculator::restart(string *s)
{
	expr.clear();
	polish_expr.clear();
	mainStr = s;
}

Variable* ExprCalculator::getVariable(string name)
{
	if (variable_list.count(name))
		return &variable_list[name];
	return nullptr;
}