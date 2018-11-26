calc-3.0:calc-3.0.cpp w1.o w2.o w3.o
	g++ -std=c++11 -o calc-3.0 calc-3.0.cpp w1.o w2.o w3.o
w1.o : CalculatorShell.cpp
	g++ -std=c++11 -c -o w1.o  CalculatorShell.cpp
w2.o : ExprCalculator.cpp
	g++ -std=c++11 -c -o w2.o  ExprCalculator.cpp
w3.o : Terms.cpp
	g++ -std=c++11 -c -o w3.o Terms.cpp 
clean :
	rm -rf calc-3.0 *.o
