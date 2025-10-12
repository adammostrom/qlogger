.PHONY: run, runqt

run:
	@clang++ tutorial.cpp -o tutorial -lncurses
	@./tutorial
	
runqt:
	g++ testQt.cpp -o testQt `pkg-config --cflags --libs Qt6Widgets`
