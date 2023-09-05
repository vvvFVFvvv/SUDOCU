#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Sudocu_Data {
public:
	int field[9][9] = {0};
	map<int, vector<int>> pretendents;
	int filled_cells = 0;

	Sudocu_Data(vector<vector<int>> raw_sudocu) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				field[i][j] = raw_sudocu[i][j];
				if (raw_sudocu[i][j] != 0) filled_cells++;
			}
		}
		for (int a = 0; a < 81; a++) {
			pretendents[a].reserve(9);
		}

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				for (int num = 1; num < 10; num++) {
					pretendents[i * 9 + j].push_back(num);
				}
			}
		}
	}
	Sudocu_Data() {
		for (int a = 0; a < 81; a++) {
			pretendents[a].reserve(9);
		}

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				for (int num = 1; num < 10; num++) {
					pretendents[i * 9 + j].push_back(num);
				}
			}
		}
	}


	void Print_Field() {
		cout << endl << endl << endl;

		for (int i = 0; i < 9; i++) {
			cout << endl;
			for (int j = 0; j < 9; j++) {
				cout << " "s << field[i][j];
			}
		}
		cout << endl << endl << endl;
	}


};

class Sudocu {
public:
	Sudocu() = default;

	Sudocu(vector<vector<int>> raw_sudocu)
		:sudocu(raw_sudocu)
	{}

	void Delete_Pretendents(int i, int j) {

		sudocu.pretendents[i * 9 + j].clear();

		for (int a = 0; a < 9; a++) {
			auto it = find(sudocu.pretendents[a*9+j].begin(), sudocu.pretendents[a*9+j].end(), sudocu.field[i][j]);
			if(it != sudocu.pretendents[a * 9 + j].end())  sudocu.pretendents[a * 9 + j].erase(it);
		}

		for (int a = 0; a < 9; a++) {
			auto it = find(sudocu.pretendents[i * 9 + a].begin(), sudocu.pretendents[i * 9 + a].end(), sudocu.field[i][j]);
			if(it != sudocu.pretendents[i * 9 + a].end())	sudocu.pretendents[i * 9 + a].erase(it);
		}

		int N = i / 3, M = j / 3;

		for (int a = N*3; a < N*3+3; a++) {
			for (int b = M*3; b < M*3+3; b++) {
				auto it = find(sudocu.pretendents[a * 9 + b].begin(), sudocu.pretendents[a * 9 + b].end(), sudocu.field[i][j]);
				if(it != sudocu.pretendents[a * 9 + b].end())	sudocu.pretendents[a * 9 + b].erase(it);
			}
		}
	}

	void Fill_Cell(int L, int R, int num) {
		cout << "Put number "s << num << " at position [" << L + 1 << ", " << R + 1 << "]" << endl;
		sudocu.field[L][R] = num;
		sudocu.filled_cells++;
		Delete_Pretendents(L, R);
	}

	bool Check_Lines() {
		cout << "Start checking lines..." << endl;
		for (int line = 0; line < 9; line++) {
			bool is_found = false;
			bool need_break = false;
			int L, R;
			cout << "new line..." << endl;
			for (int num = 1; num < 10 && !need_break; num++) {
				for (int row = 0; row < 9 && !need_break; row++) {

					if (find(sudocu.pretendents[line * 9 + row].begin(), sudocu.pretendents[line * 9 + row].end(), num) != sudocu.pretendents[line * 9 + row].end())
					{
						cout << "Found needed pretendent:" << num << endl;
						if (!is_found) {
							is_found = true;
							L = line; R = row;
						}
						else need_break = true;
					}
				}
				if (is_found && !need_break) {
					Fill_Cell(L, R, num);
					
					return true;
				}
			}
		}
		cout << "Found nothing..." << endl;
		return false;
	}

	bool Check_Rows(){
		cout << "Start checking rows..." << endl;
		for (int row = 0; row < 9; row++) {
		bool is_found = false;
		bool need_break = false;
		int L, R;
		cout << "new row..." << endl;
		for (int num = 1; num < 10 && !need_break; num++) {
			for (int line = 0; line < 9 && !need_break; line++) {

				if (find(sudocu.pretendents[line * 9 + row].begin(), sudocu.pretendents[line * 9 + row].end(), num) != sudocu.pretendents[line * 9 + row].end())
				{
					cout << "Found needed pretendent:" << num << endl;
					if (!is_found) {
						is_found = true;
						L = line; R = row;
					}
					else need_break = true;
				}
			}

			if (is_found && !need_break) {
				Fill_Cell(L, R, num);
				return true;
			}
		}
	}
		cout << "Found nothing..." << endl;
	return false;
}

	bool Check_Squares() {

		cout << "Start checking Squares..." << endl;

		for (int N = 0; N < 9; N++) {
			for (int num = 1; num < 10; num++) {
				bool is_found = false;
				bool need_break = false;
				int L, R;

				for (int line = (N / 3) * 3; line < (N / 3) * 3 + 3 && !need_break; line++) {
					for (int row = (N % 3) * 3 ; row < (N % 3) * 3 + 3 && !need_break; row++) {

						if (find(sudocu.pretendents[line * 9 + row].begin(), sudocu.pretendents[line * 9 + row].end(), num) != sudocu.pretendents[line * 9 + row].end())
						{
							if (!is_found) {
								is_found = true;
								L = line; R = row;
							}
							else need_break = true;
						}

					}
				}

				if (is_found && !need_break) {
					Fill_Cell(L, R, num);
					return true;
				}
			}
		}

		cout << "Found nothing..." << endl;
		return false;
	}

	bool Obvious_Check() {
		cout << "Start obvious checking..." << endl;
		for (int i = 0; i < 81; i++) {
			if (sudocu.pretendents[i].size() == 1) {
				Fill_Cell(i/9, i%9, sudocu.pretendents[i][0]);
				return true;
			}
		}
		cout << "Found nothing..." << endl;
		return false;
	}

	bool Error() {
		for (int i = 0; i < 81; i++) {
			if (sudocu.pretendents[i].size() == 0 && sudocu.field[i % 9][i / 9] == 0) {
				return true;
			}
		}
		return false;
	}

	void Preprocess() {
		for (int i = 0; i < 81; i++) {
			if (sudocu.field[i / 9][i % 9] != 0) {
				Delete_Pretendents(i / 9, i % 9);
			}
		}
	}

	void Solver() {
		Preprocess();
		while (sudocu.filled_cells < 81) {

			cout << "Another loop" << endl;

			sudocu.Print_Field();

			if (Obvious_Check()) continue;

			if (Check_Lines()) continue;

			if (Check_Rows()) continue;

			if (Check_Squares()) continue;

			if (Error()) { cout << "Error has occured :("s << endl; 
			sudocu.filled_cells = 100;
			}

			cout << "Unable to solve :("s << endl;
			sudocu.filled_cells = 100;
		}
		if (sudocu.filled_cells == 81) {
			cout << "Solved :)"s << endl;
			sudocu.Print_Field();
		}
	}

private:
	bool changes;
	Sudocu_Data sudocu;
};

int main() {
	vector<vector<int>> raw_sudocu_template = { 
		{0,0,0 ,0,0,0 ,0,0,0},
		{0,0,0 ,0,0,0 ,0,0,0},
		{0,0,0 ,0,0,0 ,0,0,0},

		{0,0,0 ,0,0,0 ,0,0,0},
		{0,0,0 ,0,0,0 ,0,0,0},
		{0,0,0 ,0,0,0 ,0,0,0},

		{0,0,0 ,0,0,0 ,0,0,0},
		{0,0,0 ,0,0,0 ,0,0,0},
		{0,0,0 ,0,0,0 ,0,0,0}
		};

	vector<vector<int>> raw_sudocu = {
		{0,0,5 ,0,0,0 ,9,0,2},
		{7,0,6 ,0,0,0 ,0,1,0},
		{8,0,1 ,2,5,0 ,4,0,7},

		{2,0,0 ,0,0,0 ,0,0,0},
		{0,0,7 ,9,1,0 ,0,2,0},
		{0,0,3 ,0,0,0 ,5,0,0},

		{0,0,9 ,0,3,2 ,6,5,0},
		{0,0,0 ,7,0,4 ,0,9,1},
		{0,0,0 ,0,9,8 ,0,0,0}
	};

	Sudocu S(raw_sudocu);
	S.Solver();
}

