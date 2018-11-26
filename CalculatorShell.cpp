#include "CalculatorShell.h"

CalculatorShell::CalculatorShell()
{
	cal = new ExprCalculator(nullptr);
	main_procedure();
}


CalculatorShell::~CalculatorShell()
{
	if (cal != NULL)
		delete cal;
}

void CalculatorShell::main_procedure()
{
	cout << "Calc-3.0 by HanZeyao\n \
type \"help\" for more information\n";
	try
	{
		while (true)
		{
			cout << "In[" << shellCount <<"]"<<":= ";
			string after,head;
			cin >> head;
			getline(cin, after);
			cout << endl;
			string output;
			if (head == "let")
			{
				output = letCommand(after);
			}
			else if (head == "quit" || head == "q")
			{
				cout << "Goodbye!" << endl;
				break;
			}
			else if (head == "help")
			{
				output = "Try something like:\n"\
					"sin(pi/4)*cos(pi/4)*sqrt(2^1^2)\n"\
					"let x = sin(pi/2)*5\n"\
					"x = 1+sin(x)\n"\
					"x = log(e^2)";
			}
			else
			{
				string s = head + after;
				regex r_name("^[A-Za-z_][A-Za-z0-9_]*[\\s]*=");
				smatch m;
				regex_search(s, m, r_name);
				if (!m.empty())
				{
					string va_name = m.begin()->str();
					int len = va_name.size();
					for (int i = 0; i < va_name.size(); i++)
					{
						if (isblank(va_name.at(i))||va_name.at(i) == '=')
							va_name.erase(i--, 1);
					}
					Variable * pToVa = cal->getVariable(va_name);
					if (pToVa == nullptr)
					{
						cout << "Undefined variable " << va_name << endl;
						break;
					}
					else
					{
						string expr = s.substr(len);
						cal->restart(&expr);
						string ans = cal->answer();
						pToVa->changeVal(stod(ans));
						output = ans;
					}
				}
				else
				{
					cal->restart(&s);
					output = cal->answer();
				}
			}
			cout << "Out[" << shellCount++ << "]" << "= " << output << endl << endl;
		}
	}
	catch (runtime_error &e)
	{
		cout << e.what() << endl;
	}
	
}

string CalculatorShell::letCommand(string s)
{
	regex r_name("^\\s*[A-Za-z_][A-Za-z0-9_]*\\s*=");
	smatch m;
	string warn{ "Invalid use of 'Let',Try something like\n"\
	"let x = sin(pi/2)" };
	regex_search(s, m, r_name);
	if (!m.empty())
	{
		string va_name = m.begin()->str();
		int len = va_name.size();
		for (int i = 0; i < va_name.size(); i++)
		{
			if (isblank(va_name.at(i)) || va_name.at(i) == '=')
				va_name.erase(i--, 1);
		}
		Variable * pToVa = cal->getVariable(va_name);
		if (pToVa != nullptr)
		{
			return "This variable has been defined.";
		}
		else
		{
			string expr = s.substr(len);
			cal->restart(&expr);
			string ans = cal->answer();
			cal->addVariable(va_name, stod(ans));
			return ans;
		}
	}
	else
	{
		return warn;
	}
}
