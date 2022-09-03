main : main.cpp
	g++-10 -std=c++2a -O2 main.cpp grid.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system; ./main

clean : 
	rm main