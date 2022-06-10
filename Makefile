All:
	g++ -std=c++2b -Iinclude/ -Idoc/ src/*.cpp -o bin/ISG++
	clang++ -std=c++2b -Wno-parentheses -Iinclude/ -Idoc/ src/*.cpp -o bin/ISCl++