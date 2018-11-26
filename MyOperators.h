#pragma once
#include <cstdarg>
#include <cmath>
#include <vector>
#include <stdexcept>
using namespace std;
//constants
const double pi = 3.14159265358979;
const double e = 2.718281828459;

//operators
double _add(vector<double> x)
{
	return x.at(1) + x.at(0);
}
double _sub(vector<double> x)
{
	return x.at(1) - x.at(0);
}
double _mul(vector<double> x)
{
	return x.at(1) * x.at(0);
}
double _div(vector<double> x)
{
	return x.at(1) / x.at(0);
}
double _pow(vector<double> x)
{
	return pow(x.at(1), x.at(0));
}
int fac(int temp)
{
	if (temp < 0)
		throw runtime_error("illegal use of n!");
	if (temp == 0)
		return 1;
	else
		return temp * fac(temp - 1);
}
double _fact(vector<double> x)
{
	int temp = int(x.at(0));
	return fac(temp);
}

double _fmod(vector<double> x)
{
	return fmod(x.at(1), x.at(0));
}

//functions
double _sin(vector<double> x)
{
	return ::sin(x.at(0));
}

double _cos(vector<double> x)
{
	return ::cos(x.at(0));
}

double _sqrt(vector<double> x)
{
	if(x.at(0) < 0)
	{
		throw runtime_error("sqrt must apply to positive numbers");
	}
	return sqrt(x.at(0));
}

double _log(vector<double> x)
{
	return log(x.at(0));
}
