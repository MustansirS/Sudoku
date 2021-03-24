#include "sudoku.h"

int main(int argc, char* argv[]) {
	if (argc < 2) return 1;
	Sudoku s{argv[1]};
	cout << s << endl;
	s.alg3();
	cout << s << endl;
	if (s.checkgrid()) cout << "Success!" << endl;
}
