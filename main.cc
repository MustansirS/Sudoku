#include "sudoku.h"
#include <chrono>
#include <map>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

struct PuzzleTime {
    int puzzle;
    double time;
};

int main(int argc, char* argv[]) {
    string all = "./sudokuall";
    string single = "./sudoku";
    if (argv[0] == single) {
        if (argc < 2) return 1;
        string flag = "-s";
        bool show = (argv[1] == flag);
        string puzzle_num = show ? argv[2] : argv[1];
        Sudoku s{"puzzle"+puzzle_num+".txt", show};
        cout << s << endl;
        auto t1 = high_resolution_clock::now();
        s.alg3();
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        cout << "\033[32mSolved Puzzle:\033[0m" << endl;
        cout << s << endl;
        cout << "It took " << s.numguesses << " guesses" << endl;
        if (s.checkgrid()) cout << "Success!" << endl;
        cout << "It took "<< ms_double.count() << " ms" << endl;
    } else if (argv[0] == all) {
        int numpuzzles = 0;
        string puz = "puzzle";
        for (auto& dir_entry: std::filesystem::directory_iterator("./")) {
            if (dir_entry.path().stem().string().substr(0,6) == puz) numpuzzles++;
        }
        map<int,vector<int>> guess_map;
        vector<PuzzleTime> time_rankings;
        for (int i = 1; i <= numpuzzles; ++i) {
            string puzzle = "puzzle" + to_string(i) + ".txt";
            Sudoku s{puzzle,false};
            auto t1 = high_resolution_clock::now();
            s.alg3();
            auto t2 = high_resolution_clock::now();
            duration<double, std::milli> ms_double = t2 - t1;
            cout << "\033[32mPuzzle " << i << "\033[0m" << endl;
            int guesses = s.numguesses;
            double time = ms_double.count();
            PuzzleTime pt{i,time};
            guess_map[guesses].push_back(i);
            bool inserted = false;
            for (int j = 0; j < time_rankings.size(); ++j) {
                if (time < time_rankings[j].time) {
                    time_rankings.insert(time_rankings.begin()+j,pt);
                    inserted = true;
                    break;
                }
            }
            if (!inserted) time_rankings.push_back(pt);
            cout << "It took " << guesses << " guesses" << endl;
            if (s.checkgrid()) cout << "Success!" << endl;
            cout << "It took "<< time << " ms" << endl << endl;
        }
        cout << "\033[34mGuesses categorization:\033[0m" << endl << endl;
        for (auto &[guesses,puzzles] : guess_map) {
            cout << "\033[36m" << guesses << " Guesses:\033[0m" << endl;
            for (auto i : puzzles) cout << "Puzzle " << i << endl;
        }
        cout << endl;
        cout << "\033[35mTime rankings:\033[0m" << endl << endl;
        for (int j = 0; j < time_rankings.size(); ++j) {
            cout << j + 1 << ". Puzzle " << time_rankings[j].puzzle << " took " << time_rankings[j].time << " ms" << endl;
        }
    }
}
