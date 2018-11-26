#pragma once
#include "ExprCalculator.h"

//This shell should support structures as follows
// In[1]:= help
//
// Out[1]= "..."
//
// In[2]:= let x = [Expr]
//
// Out[2]= [Val]
//
// In[3]:= sin(pi/2)+5
//
// Out[3]= 6.00
//
// In[4]:= x=[newExpr]
//
// Out[4]= [newVal]
class CalculatorShell
{
public:
	CalculatorShell();
	~CalculatorShell();
private:
	unsigned int shellCount = 0;
	void main_procedure();
	ExprCalculator* cal;
	string letCommand(string s);
};