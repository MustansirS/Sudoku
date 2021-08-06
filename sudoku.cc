#include "sudoku.h"

Sudoku::Sudoku(string filename, bool show) : empty{0}, show{show} {
	ifstream f{filename};
	int x;
	for (int i = 0; i < 9; ++i) {
		vector<int> v;
        vector<bool> color;
		for (int j = 0; j < 9; ++j) {
			f >> x;
			v.push_back(x);
			if (x == 0) {
                empty++;
                color.push_back(true);
            } else {
                color.push_back(false);
            }
		}
		grid.push_back(v);
        grid_color.push_back(color);
	}
}

Sudoku::Sudoku(const Sudoku &other) : empty{other.empty}, grid{other.grid}, show{false} {}

Sudoku &Sudoku::operator=(const Sudoku &other) {
	empty = other.empty;
	grid = other.grid;
	show = false;
	return *this;
}

vector<int> Sudoku::getRow(int r) const {
	return grid[r];
}

vector<int> Sudoku::getCol(int c) const {
	vector<int> result;
	for (int i = 0; i < 9; ++i) result.push_back(grid[i][c]);
	return result;
}

vector<int> Sudoku::getGrid(int g) const {
	vector<int> result;
	for (int i = 0; i < 9; ++i) result.push_back(grid[(i/3)+(g/3)*3][(g%3)*3+i%3]);
	return result;
}

int Sudoku::rownum(int r, int c) {
	return r;
}

int Sudoku::colnum(int r, int c) {
	return c;
}

int Sudoku::gridnum(int r, int c) {
	return (c/3)+3*(r/3);
}

int Sudoku::gridpos(int r, int c) {
	return 3*(r%3)+(c%3);
}

bool Sudoku::usedRow(int n, int r, int skippos) const {
	auto v = getRow(r);
	for (int i = 0; i < 9; ++i) {
		if (i == skippos) continue;
		if (n == v[i]) return true;
	}
	return false;
}

bool Sudoku::usedCol(int n, int c, int skippos) const {
	auto v = getCol(c);
	for (int i = 0; i < 9; ++i) {
		if (i == skippos) continue;
		if (n == v[i]) return true;
	}
	return false;
}

bool Sudoku::usedGrid(int n, int g, int skippos) const {
	auto v = getGrid(g);
	for (int i = 0; i < 9; ++i) {
		if (i == skippos) continue;
		if (n == v[i]) return true;
	}
	return false;
}

bool Sudoku::feasible(int n, int r, int c) const {
	if (n == 0) return false;
	bool x = !(usedRow(n,r,c));
	bool y = !(usedCol(n,c,r));
	bool z = !(usedGrid(n,Sudoku::gridnum(r,c),Sudoku::gridpos(r,c)));
	return x && y && z;
}

bool Sudoku::checkgrid() const {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (!feasible(grid[i][j],i,j)) return false;
		}
	}
	return true;
}

vector<int> Sudoku::feasibility(int r, int c) const {
	vector<int> result;
	for (int i = 1; i < 10; ++i) {
		if (feasible(i,r,c)) result.push_back(i);
	}
	return result;
}

bool Sudoku::hasone() const {
	for (int i = 0; i < 9; ++i) {
		for (int  j = 0; j < 9; ++j) {
			if (grid[i][j] != 0) continue;
			if (feasibility(i,j).size() == 1) return true;
		}
	}
	return false;
}

void Sudoku::alg1(ostream &out) {
	while (hasone() && empty > 0) {
		vector<int> a;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (grid[i][j] != 0) continue;
				a = feasibility(i,j);
				if (a.size() == 1) {
					grid[i][j] = a[0];
					if (show) {
                        out << "Placing " << a[0] << " at " << i+1 << "," << j+1 << " because nothing else can go here!" << endl;
                        out << *this << endl;
                    }
					empty--;
				}
			}
		}
	}
}

int Sudoku::timesFeasibleInRow(int n, int r) const {
	int count = 0;
	for (int i = 0; i < 9; ++i) {
		if (grid[r][i] != 0) continue;
		if (feasible(n,r,i)) count++;
	}
	return count;
}

int Sudoku::timesFeasibleInCol(int n, int c) const {
	int count = 0;
	for (int i = 0; i < 9; ++i) {
		if (grid[i][c] != 0) continue;
		if (feasible(n,i,c)) count++;
	}
	return count;
}

int Sudoku::timesFeasibleInGrid(int n, int g) const {
	int count = 0;
	for (int i = 0; i < 9; ++i) {
		int r = (g/3)*3+i/3;
		int c = (g%3)*3+i%3;
		if (grid[r][c] != 0) continue;
		if (feasible(n,r,c)) count++;
	}
	return count;
}

void Sudoku::alg2(ostream &out) {
	alg1(out);
	if (empty == 0) return;
	int change = 1;
	while (change > 0) {
		change = 0;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (grid[i][j] != 0) continue;
				for (int k = 1; k < 10; ++k) {
					if (!feasible(k,i,j)) continue;
					if (timesFeasibleInRow(k,i) == 1) {
						grid[i][j] = k;
						if (show) {
                            out << "Placing " << k << " at " << i+1 << "," << j+1 << " because it can go nowhere else in the row!" << endl;
                            out << *this << endl;
                        }
						empty--;
						change++;
						break;
					} else if (timesFeasibleInCol(k,j) == 1) {
						grid[i][j] = k;
						if (show) {
                            out << "Placing " << k << " at " << i+1 << "," << j+1 << " because it can go nowhere else in the column!" << endl;
                            out << *this << endl;
                        }
						empty--;
						change++;
						break;
					} else if (timesFeasibleInGrid(k,Sudoku::gridnum(i,j)) == 1) {
						grid[i][j] = k;
						if (show) {
                            out << "Placing " << k << " at " << i+1 << "," << j+1 << " because it can go nowhere else in the subgrid!" << endl;
                            out << *this << endl;
                        }
						empty--;
						change++;
						break;
					}
				}
			}
		}
		alg1(out);
		if (empty == 0) return;
	}
}

bool Sudoku::impossible() const {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (feasibility(i,j).size() == 0) return true;
		}
	}
	return false;
}

int Sudoku::guess(int trial, int r, int c) const {
	Sudoku exp{*this};
	vector<int> info = feasibility(r,c);
	exp.grid[r][c] = info[trial];
	exp.alg2();
	if (exp.checkgrid()) return 0;
	else if (!exp.impossible()) return 1;
	else return 2;
}

Sudoku Sudoku::experiment(int trial, int r, int c) const {
	Sudoku exp{*this};
	vector<int> info = feasibility(r,c);
	exp.grid[r][c] = info[trial];
	exp.alg2();
	return exp;
}

Pos Sudoku::guesspos() const {
	Pos p{-1,-1};
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (grid[i][j] != 0) continue;
			vector<int> f = feasibility(i,j);
			if (f.size() >= 2) {
				p.r = i;
				p.c = j;
				break;
			}
		}
	}
	return p;
}

bool Sudoku::guesswork(int trial, int r, int c) const {
	if (r == -1 || c == -1) return false;
	int g = guess(trial,r,c);
	if (g == 0) return true;
	else if (g == 2) return false;
	else {
		Sudoku exp = experiment(trial,r,c);
		Pos co = exp.guesspos();
		vector<int> info = exp.feasibility(co.r,co.c);
		for (int t = 0; t < (int)info.size(); ++t) if (exp.guesswork(t,co.r,co.c)) return true;
		return false;
	}
}

void Sudoku::alg3(ostream &out) {
	alg2(out);
	if(empty == 0) return;
	Sudoku exp{*this};
	Pos gp = exp.guesspos();
	vector<int> info = exp.feasibility(gp.r,gp.c);
	for (int t = 0; t < (int)info.size(); ++t) {
		if (exp.guesswork(t,gp.r,gp.c)) {
            numguesses++;
			grid[gp.r][gp.c] = info[t];
			if (show) { 
                out << "Guessed " << info[t] << " at " << gp.r << "," << gp.c << endl;
                out << *this << endl;
            }
			empty--;
			alg3(out);
			break;
		}
	}
}

ostream &operator<<(ostream &out, const Sudoku &s) {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
            if (s.grid_color[i][j]) out << "\033[41m";
			if (s.grid[i][j] != 0) out << s.grid[i][j];
            else out << " ";
            if (s.grid_color[i][j]) out << "\033[0m";
			if (j != 8) out << " ";
		}
		out << endl;
	}
	return out;
}
