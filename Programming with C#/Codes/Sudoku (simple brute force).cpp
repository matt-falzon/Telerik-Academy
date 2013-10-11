/**
*	http://trekto.info/asd
*	Версия за мобилни телефони: http://trekto.info/f/SudokuSolver.jar
*/

/*
Отиваме на първото празно квадратче.
(1) Поставяме в текущото, празно квадратче цифрата 0. Преминаваме към стъпка (2).
(2) За всяка цифра в интервала текущата + 1 до 9 проверяваме дали вече я има в реда, стълба или квадрата, в който се намира текущото квадратче.
Ако установим, че я има вземаме следващата цифра и проверяваме за нея.
Ако установим, че я няма я поставяме в квадратчето, преминаваме на следващото празно квадратче и повтаряме стъпка (1).
Ако не можем да преминем на следващо квадратче, защото сме на последното значи сме решили играта - Край.
Ако не можем да поставим никоя от 9-те цифри в текущото квадратче, значи някоя от предишните цифри е грешна. Правим връщане назад към предишното празно квадратче и повтаряме стъпка (2).
Ако не можем да направим връщане, защото сме на първото квадратче, значи задачата няма решение - Край.
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

// Под думата 'поле' в коментарите се има предвид игралното поле.
// Под 'квадрат' се има предвид големите квадрати с размер sqrt(n), от които е изградено поле.
// Под 'квадратче' се има предвид клетката, в която се поставя цифра.

using namespace std;

class Sudoku {
private:
	static const int n = 9;					// размер на полето
	static const int littleN = 3;				// размер на квадратите = sqrt(n)
	int input[n][n];					// входното поле
	int current[n][n];					// полето в момента
	int currI;						// текущ ред
	int currJ;						// текущ стълб
	bool isPosible(int number, int i, int j);		// проверява дали е възможно цифрата number
								// да бъде поставена на позиция [i][j]
	bool moveForward();					// преминава към следващото празно квадратче
	bool moveBackward();					// връща се назад към предишното квадратче

public:
	Sudoku();						// конструктор
	void readFile(const char* fileName);			// прочита входното поле от файл
	bool solve();						// решава играта
	void print();						// отпечатва полето
};

Sudoku:: Sudoku() {						// конструктор
	currI = currJ = 0;
}

bool Sudoku:: solve() {						// решава играта

	if(current[0][0])					// ако първата клетка е зададена по условие
		moveForward();					// преминаване към първата празна клетка

	do {
		int i;						// i е цифрата, която ще поставим
		do {						// в текущото квадратче
			i = current[currI][currJ] + 1;		// първоначално тя става с 1 по-голяма от
								// цифрата, която в момента е в квадратчето.
								// Ако квадратчето е празно то съдържа 0,
								// или цифра 1..n ако сме направили връщане

			while(i <= n) // докато стойността на i е по малка от n
			{				
				if(isPosible(i, currI, currJ)) // ако е възможно в текущото квадратче да
				{
					current[currI][currJ] = i; // поставим цифрата i го правим
					break;			// и прекратяваме цикъла
				}

				i++;// вземаме следващата цифра
			}

			if(i > n)				// ако сме изчерпали всички цифри и
								// никоя от тях не може да се постави
								// в текущото квадратче
				if(!moveBackward())		// опитваме да направим връщане назад
					return false;		// ако не успеем значи няма решение

		} while(i > n);					// ако i е по-голямо от n значи
								// сме направили връщане и трябва да
								// повторим горните редове код

	} while(moveForward());					// ако сме стигнали до тук значи, че
								// успешно сме поставили цифрата i
								// в текущото квадратче, затова преминаваме
								// към следващото, докато стигнем до края

	return true;						// връщаме true
}

bool Sudoku:: isPosible(int number, int row, int col) {		// проверява дали е възможно цифрата number
								// да бъде поставена на позиция [row][col]

	for(int j = 0; j < n; j++)				// проверяваме дали цифрата в някое от
		if(current[row][j] == number) return false;	// квадратчетата в ред row е равна на number

	for(int i = 0; i < n; i++)				// проверяваме дали цифрата в някое от
		if(current[i][col] == number) return false;	// квадратчет. в стълб col е равна на number

	int shiftRow = littleN * (row / littleN);		// намираме отместването на квадрата
	int shiftCol = littleN * (col / littleN);		// в който искаме да поставим number

	for(int i = shiftRow; i < shiftRow + littleN; i++)	// за всеки ред
		for(int j = shiftCol; j < shiftCol + littleN; j++)	// и стълб в квадрата
			if(current[i][j] == number) return false;	// проверяваме дали number я има

	return true;						// ако цифрата number я няма в реда, стълба
								// и квадрата, в който се намира
								// квадратчето [row][col], значи можем
								// да я поставим там
}

bool Sudoku:: moveForward() {					// преминава към следващото празно квадратче
	do {
		if(currI == n-1 && currJ == n-1)		// ако сме стигнали до края на полето
			return false;				// връщаме false

		if(currJ < n-1)					// ако не сме на последното квадратче на
			currJ++;				// реда преминаваме на следващото
		else {						// в противен случай
			currJ = 0;				// преминаваме на първото квадратче
			currI++;				// на следващия ред
		}
	} while(input[currI][currJ]);				// повтаряме горните действия докато
								// стигнем до празно квадратче

	return true;						// тук сме преминали към следващото
								// празно квадратче и връщаме true
}

bool Sudoku:: moveBackward() {					// връща се назад към предишното квадратче
	current[currI][currJ] = 0;				// премахваме текущата цифра
	do {
		if(currI == 0 && currJ == 0)			// ако се опитваме да се върнем
			return false;				// преди началото връщаме false

		if(currJ > 0)					// ако не сме в началото на някой ред
			 currJ--;				// се връщаме едно квадратче назад
		else {						// в противен случай
			currJ = n-1;				// преминаваме към последното квадратче
			currI--;				// на предишния ред
		}
	} while(input[currI][currJ]);				// повтаряме горните действия докато
								// стигнем до празно квадратче

	return true;						// върнали сме се към предишното
								// квадратче и връщаме true
}

void Sudoku:: readFile(const char* fileName) {			// прочита входното поле от файл
	ifstream inputFile(fileName);

	inputFile >> input[0][0];				// първото число във файла е размера
								// полето, затова го прескачаме

	for(int i = 0; i < n; i++)				// прочитаме входното поле
		for(int j = 0; j < n; j++) {
			inputFile >> input[i][j];
			current[i][j] = input[i][j];
		}

	inputFile.close();
}

void Sudoku:: print() {						// отпечатва полето
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			cout << current[i][j] << ' ';
		cout << '\n';
	}
}

int main(int argc, char **argv) {				// приема като параметър името на файла
								// с входните данни

	if (argc < 2) {						// ако не е зададен файл
		cerr<<"\nТрябва да зададете входен файл: programname <filename>\n";
		exit(1);
	}

	Sudoku sudoku;						// създаваме обект от тип Sudoku
	sudoku.readFile(argv[1]);				// прочитаме входните данни
	cout << "\nВходното поле:\n";
	sudoku.print();						// отпечатваме ги
	cout << "\n";

	if(sudoku.solve()) {					// опитваме се да решим играта
		cout << "Решението:\n";
		sudoku.print();					// ако намерим решение го отпечатваме
	} else cout<<"\nИграта няма решение!\nПроверете коректността на входните данни!\n";

	return 0;
}