#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Pos { int r,c; };

class Sudoku {
	vector<vector<int>> grid;
	vector<vector<bool>> grid_color;
	int empty;
	bool show;
public:
    int numguesses = 0;
	Sudoku(string filename, bool show = true);
	Sudoku(const Sudoku &other);
	Sudoku &operator=(const Sudoku &other);
	vector<int> getRow(int r) const;
	vector<int> getCol(int c) const;
	vector<int> getGrid(int g) const;
	static int rownum(int r, int c);
	static int colnum(int r, int c);
	static int gridnum(int r, int c);
	static int gridpos(int r, int c);
	bool usedRow(int n, int r, int skippos = -1) const;
	bool usedCol(int n, int c, int skippos = -1) const;
	bool usedGrid(int n, int g, int skippos = -1) const;
	bool feasible(int n, int r, int c) const;
	bool checkgrid() const;
	vector<int> feasibility(int r, int c) const;
	bool hasone() const;
	void alg1(ostream &out = cout);
	int timesFeasibleInRow(int n, int r) const;
	int timesFeasibleInCol(int n, int c) const;
	int timesFeasibleInGrid(int n, int g) const;
	void alg2(ostream &out = cout);
	bool impossible() const;
	int guess(int trial, int r, int c) const;
	Sudoku experiment(int trial, int r, int c) const;
	Pos guesspos() const;
	bool guesswork(int trial, int r, int c) const;
	void alg3(ostream &out = cout);
	friend ostream &operator<<(ostream &out, const Sudoku &s);
};

ostream &operator<<(ostream &out, const Sudoku &s);

#endif
