#include <iostream>
using std::cout;
using std::endl;
using std::cin;

bool used(int n, int *row, int except) {
	for (int j = 0; j < 9; ++j) {
		if (j == except) {continue;}
		if (n == row[j]) {return true;}
	}
	return false;
}

void printgrid(int *rows) {
	for (int i = 0; i < 81; ++i) {
		cout << rows[i] << " ";
		if (i % 9 == 8) {
			cout << endl;
		}
	}
}

int *getrow(int i, int *grid) {
	int *rowi = new int[9];
	for (int j = 0; j < 9; ++j) {
		rowi[j] = grid[9*(i-1)+j];
	}
	return rowi;
}

int *getcolumn(int i, int *grid) {
	int *columni = new int[9];
	for (int j = 0; j < 9; ++j) {
		columni[j] = grid[i+9*j-1];
	}
	return columni;
}

int *getgrid(int i, int *grid) {
	int *gridi = new int[9];
	--i;
	for (int j = 0; j < 9; ++j) {
		gridi[j] = grid[(i/3)*27+(i%3)*3+(j/3)*9+j%3];
	}
	return gridi;
}

int posrow(int pos) {
	return pos/9 + 1;
}

int poscol(int pos) {
	return pos%9 + 1;
}

int posgrid(int pos) {
	return 3*((pos/9)/3)+(pos%9)/3+1;
}

bool feasible(int n, int pos, int *grid) {
	if (n==0) {return false;}
	int *row = getrow(posrow(pos),grid);
	int rowpos = pos % 9;
	int *col = getcolumn(poscol(pos),grid);
	int colpos = pos / 9;
	int *sgrid = getgrid(posgrid(pos),grid);
	int gridpos = 3*((pos / 9) % 3) + (pos % 9) % 3;
	bool rowf = !(used(n,row,rowpos));
	bool colf = !(used(n,col,colpos));
	bool gridf = !(used(n,sgrid,gridpos));
	delete[] row;
	delete[] col;
	delete[] sgrid;
	return rowf && colf && gridf;
}

bool checkgrid(int *grid) {
	for (int i = 0; i < 81; ++i) {
		if(!feasible(grid[i],i,grid)) {
			return false;
		}
	}
	return true;
}

int *feasibility(int pos, int *grid) {
	int *row = getrow(posrow(pos),grid);
	int rowpos = pos % 9;
	int *col = getcolumn(poscol(pos),grid);
	int colpos = pos / 9;
	int *sgrid = getgrid(posgrid(pos),grid);
	int gridpos = 3*((pos / 9) % 3) + (pos % 9) % 3;
	bool check = false;
	int count = 0;
	int *n = new int[10];
	n[0] = count;
	for (int i = 1; i < 10; ++i) {
		check = !(used(i,row,rowpos)) && !(used(i,col,colpos)) && !(used(i,sgrid,gridpos));
		if(check) {
			count++;
			n[count] = i;
		}
	}
	n[0] = count;
	delete[] row;
	delete[] col;
	delete[] sgrid;
	return n;
}

void workfirst(int *grid) {
	for (int pos = 0; pos < 81; ++pos) {
		int *n0 = feasibility(pos,grid);
		if (n0[0]==1) {grid[pos]=n0[1];}
		delete[] n0;
	}
}

bool hasone(int *grid) {
	for (int pos = 0; pos < 81; ++pos) {
		if (grid[pos] != 0) {continue;}
		int *arr = feasibility(pos,grid);
		if (arr[0] == 1) {return true;}
		delete[] arr;
	}
	return false;
}

int *rowpositions(int pos) {
	int rownum = posrow(pos);
	int *positions = new int[21];
	for (int j = 0; j < 9; ++j) {
		positions[j] = 9*(rownum-1)+j;
	}
	return positions;
}

int *colpositions(int pos) {
	int colnum = poscol(pos);
	int *positions = new int[21];
	for (int j = 0; j < 9; ++j) {
		positions[j] = colnum+9*j-1;
	}
	return positions;
}

int *gridpositions(int pos) {
	int gridnum = posgrid(pos) - 1;
	int *positions = new int[21];
	for (int j = 0; j < 9; ++j) {
		positions[j] = (gridnum/3)*27+(gridnum%3)*3+(j/3)*9+j%3;
	}
	return positions;
}

int inrow(int n, int pos, int *grid) {
	int *all = rowpositions(pos);
	int count = 0;
	for (int i = 0; i < 9; ++i) {
		if (grid[all[i]] != 0) {continue;}
		if (feasible(n,all[i],grid)) {count++;}
	}
	delete[] all;
	return count;
}

int incol(int n, int pos, int *grid) {
	int *all = colpositions(pos);
	int count = 0;
	for (int i = 0; i < 9; ++i) {
		if (grid[all[i]] != 0) {continue;}
		if (feasible(n,all[i],grid)) {count++;}
	}
	delete[] all;
	return count;
}

int ingrid(int n, int pos, int *grid) {
	int *all = gridpositions(pos);
	int count = 0;
	for (int i = 0; i < 9; ++i) {
		if (grid[all[i]] != 0) {continue;}
		if (feasible(n,all[i],grid)) {count++;}
	}
	delete[] all;
	return count;
}

void alg1(int *grid) {
	while (!checkgrid(grid) && hasone(grid)) {
		workfirst(grid);
	}
}

int alg2(int *grid) {
	int change = 0;
	for (int i = 0; i < 81; ++i) {
		if (grid[i] != 0) {continue;}
		for (int j = 1; j < 10; ++j) {
			if (!feasible(j,i,grid)) {continue;}
			if (inrow(j,i,grid)==1) {
				grid[i]=j;
				change++;
				break;
			}
			else if(incol(j,i,grid)==1) {
				grid[i]=j;
				change++;
				break;
			}
			else if(ingrid(j,i,grid)==1) {
				grid[i]=j;
				change++;
				break;
			}
		}
	}
	return change;
}

void logicsolve(int *grid) {
	alg1(grid);
	int count = 1;
	while (!checkgrid(grid) && count != 0) {
		count = alg2(grid);
		alg1(grid);
	}
}

bool impossible(int *grid) {
	bool ans = false;
	for (int pos = 0; pos < 81; ++pos) {
		int *n0 = feasibility(pos,grid);
		if (n0[0]==0) {ans = true;}
		delete[] n0;
	}
	return ans;
}

int guess(int trial, int pos, int *grid) {
	int *exp = new int[81];
	for (int i = 0; i < 81; ++i) {
		exp[i] = grid[i];
	}
	int *info = feasibility(pos, grid);
	exp[pos] = info[trial];
	logicsolve(exp);
	if(checkgrid(exp)) {
		delete[] exp;
		delete[] info;
		return 0;
	} else if(!impossible(exp)) {
		delete[] exp;
		delete[] info;
		return 1;
	} else {
		delete[] exp;
		delete[] info;
		return 2;
	}
}

int *experiment(int n, int pos, int *grid) {
	int *exp = new int[81];
	for (int i = 0; i < 81; ++i) {
		exp[i] = grid[i];
	}
	int *info = feasibility(pos, grid);
	exp[pos] = info[n];
	logicsolve(exp);
	delete[] info;
	return exp;
}

void experimentrisky(int n, int pos, int *grid) {
	int *info = feasibility(pos, grid);
	grid[pos] = info[n];
	logicsolve(grid);
	delete[] info;
}

int guesspos(int *grid) {
	int p = -1;
	for (int pos = 0; pos < 81; ++pos) {
		if (grid[pos] != 0) {continue;}
		int *n0 = feasibility(pos,grid);
		if (n0[0] >= 2) {
			p=pos;
			delete[] n0;
			break;
		}
		delete[] n0;
	}
	return p;
}

bool guesswork(int trial, int position, int *grid) {
	bool ans = false;
	if (position == -1) {return false;}
	int g = guess(trial,position,grid);
	if (g==0) {return true;}
	else if (g==1) {
		int *exp = experiment(trial,position,grid);
		int gp = guesspos(exp);
		int *info = feasibility(gp,exp);
		for (int t = 1; t <= info[0]; ++t) {
			if (guesswork(t,gp,exp)) {
				delete[] info;
				delete[] exp;
				return true;
			}
		}
		return false;
	} else {
		return false;

	}
}

void alg3(int *grid) {
	logicsolve(grid);
	if(checkgrid(grid)) {return;}
	int *exp = new int[81];
	for (int i = 0; i < 81; ++i) {
		exp[i] = grid[i];
	}
	int gp = guesspos(exp);
	int *info = feasibility(gp,exp);
	for (int t = 1; t <= info[0]; ++t) {
		if (guesswork(t,gp,exp)) {
			cout << "guessed " << info[t] << " at " << gp << endl;
			experimentrisky(t,gp,grid);
			alg3(grid);
			break;
		}
	}
	delete[] info;
	delete[] exp;
	return;
}

/********************************************************************/
// For debugging:

void printarray(int *row) {
	for (int k = 0; k < 9; ++k) {
		cout << row[k] << " ";
	}
	cout << endl;
}

void printinfo(int pos, int *grid) {
	int *n0 = feasibility(pos,grid);
	cout << n0[0] << endl;
	for(int i = 1; i <= n0[0]; ++i) {
		cout << n0[i] << " ";
	}
	cout << endl;
	delete[] n0;
}

void printallinfo(int *grid) {
	for (int i = 0; i < 81; ++i) {
		if (grid[i]==0) {
			cout << "at position " << i << endl;
			printinfo(i,grid);
		}
	}
}
/********************************************************************/

int main() {
	int grid[81] = {0};
	for (int count = 0; count < 81; ++count) {
		cin >> grid[count];
	}

	cout << endl;
	printgrid(grid);
	cout << endl;

	alg3(grid);

	printgrid(grid);
	cout << endl;

	return 0;
}
