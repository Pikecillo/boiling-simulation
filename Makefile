boiling-2d: HeatField.o main.o
	g++ HeatField.o main.o -o boiling-2d -lglut -lGL -lGLU
HeatField.o: HeatField.h HeatField.cpp Matrix.h
	g++ HeatField.cpp -c
main.o: main.cpp HeatField.h Matrix.h
	g++ main.cpp -c
