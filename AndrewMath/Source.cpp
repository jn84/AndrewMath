#include <iomanip>
#include "andrewMath.h"
#include "highScoreEntry.h"

using namespace std;




// Drawing constants
const char  VERTICAL = 179,				HORIZONTAL = 196,			CORNER_TOP_LEFT = 218,
			CORNER_TOP_RIGHT = 191,		CORNER_BOTTOM_LEFT = 192,	CORNER_BOTTOM_RIGHT = 217,	
			CROSS = 197,				HORIZONTAL_BOTTOM = 194,	HORIZONTAL_TOP = 193, 
			VERTICAL_RIGHT = 195,		VERTICAL_LEFT = 180;

// Scoring constants
const double	ADD_SCORE = 10.0, GUESS_1 = 1.0,
				SUB_SCORE = 10.0, GUESS_2 = 0.66,
				MUL_SCORE = 20.0, GUESS_3 = 0.30,
				DIV_SCORE = 25.0, GUESS_4 = 0.15,
				ALG_MULTIPLIER = 2.2,   LEVEL_1 = 1.0, LEVEL_2 = 1.33, LEVEL_3 = 1.66, LEVEL_4 = 2.0,
				HCORE_TIME_LIMIT = 10,  LEVEL_5 = 2.5, LEVEL_6 = 3.0,  LEVEL_7 = 3.33, LEVEL_8 = 3.66,
				HCORE_BONUS = 2.0, LEVEL_9 = 4.0, LEVEL_10 = 5.0;

double levelValue(int level)
{
	switch (level)
	{
	case 1:
		return LEVEL_2;
	case 2:
		return LEVEL_3;
	case 3:
		return LEVEL_4;
	case 4:
		return LEVEL_5;
	case 5:
		return LEVEL_6;
	case 6:
		return LEVEL_7;
	case 7:
		return LEVEL_8;
	case 8:
		return LEVEL_9;
	case 9:
		return LEVEL_10;
	default:
		return LEVEL_1;
	}
}

int scoreCalc(double &score, char type, int level, int guessNum, time_t startTime = 0, bool isAlg = 0, bool isHCore = 0) // Should be initialized when problem made available
{
	int timeElapsed = 0; 
	int workingScore = 0;
	time_t final;
	time(&final);
	timeElapsed = final - startTime;
		if (isHCore && timeElapsed > HCORE_TIME_LIMIT)
		{
			score = 0;
			return timeElapsed;
		}
	if (type == '+')
		workingScore = ADD_SCORE;
	else if (type == '-')
		workingScore = SUB_SCORE;
	else if (type == '*')
		workingScore = MUL_SCORE;
	else if (type == '/')
		workingScore = DIV_SCORE;
	workingScore *= levelValue(level);
	if (isAlg)
		workingScore *= ALG_MULTIPLIER;
	if (isHCore)
		workingScore *= HCORE_BONUS;
	if (guessNum == 0)
	{
		score = workingScore * GUESS_1;
		return timeElapsed;
	}
	else if (guessNum == 1)
	{
		score = workingScore * GUESS_2;
		return timeElapsed;
	}
	else if (guessNum == 2)
	{
		score = workingScore * GUESS_3;
		return timeElapsed;
	}
	else if (guessNum == 3)
	{
		score = workingScore * GUESS_4;
		return timeElapsed;
	}
	else if (guessNum >= 4)
	{
		score = 0.0;
		return timeElapsed;
	}
}

void intInput(int& num, const int lower, const int upper, const string invMessage)
{
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << endl << invMessage;
		}
		cin >> num;
		while ((num < lower && !cin.fail()) || ((num > upper && !cin.fail())))
		{
			cout << endl << invMessage;
			cin >> num; // integer _space_ <anything> will mess up the stream
		}
	} while (cin.fail());
}

void mainMenuSelection(int& selection) // Seems to be correctly validated
{
	cout << "Selection:" << endl;
	cout << "        1: Start a New Game" << endl;
	cout << "        2: View the high scores" << endl;
	cout << "        3: Quit" << endl << endl;
	cout << "Enter your selection: ";

	intInput(selection, 1, 3, "Invalid Selection. Please enter another: ");
}

void gameMainMenu(int& selection)
{
	system("cls");
	cout << "Selection:" << endl;
	cout << "        1: Start a leveled game" << endl;
	cout << "        2: Start a custom game" << endl;
	cout << "        3: Start a hardcore game" << endl;
	cout << "        4: Go back to the main menu" << endl << endl;
	cout << "Enter your selection: ";
	
	intInput(selection, 1, 4, "Invalid Selection. Please enter another: ");

}

void printScores(int marker = -1)
{
	system("cls");
	highScoreEntry readScores;
	cout << setfill(' ');
	cout << "    " << left << setw(35) << "Name"
		<< right << setw(11) << "Score"
		<< "   Date" 
		<< endl;
	for (int i = 0; i < 20; i++)
	{
		if (i == 0)
		{
			cout << setfill(HORIZONTAL);
			cout << CORNER_TOP_LEFT << HORIZONTAL << HORIZONTAL << HORIZONTAL_BOTTOM
				<< setw(35) << HORIZONTAL_BOTTOM
				<< setw(14) << HORIZONTAL_BOTTOM
				<< setw(22) << CORNER_TOP_RIGHT << endl;
		}
		cout << VERTICAL << setfill(' ') << setw(2) << (i + 1) 
			<< VERTICAL << ' ' << readScores.nameInFile(i) << ' ' << VERTICAL 
			<< right << setw(12) << readScores.scoreInFile(i) << ' '
			<< VERTICAL << ' ' << readScores.dateInFile(i) << VERTICAL 
			<< (marker == i ? '*' : ' ') << endl;

		if (((i + 1) % 5) == 0 && i != 19)
		{
			cout << setfill(HORIZONTAL);
			cout << VERTICAL_RIGHT << HORIZONTAL << HORIZONTAL << CROSS
				<< setw(35) << CROSS
				<< setw(14) << CROSS
				<< setw(22) << VERTICAL_LEFT << endl;
		}
		if (i == 19)
		{
			cout << setfill(HORIZONTAL);
			cout << CORNER_BOTTOM_LEFT << HORIZONTAL << HORIZONTAL << HORIZONTAL_TOP
				<< setw(35) << HORIZONTAL_TOP
				<< setw(14) << HORIZONTAL_TOP
				<< setw(22) << CORNER_BOTTOM_RIGHT << endl;
		}
	}
}

void printTitle()
{
	system("cls");
	cout << "MMM   MMM    AAA  TTTTTTTT HH   HH" << endl;
	cout << "MMMM MMMM   AA AA    TT    HH   HH" << endl;
	cout << "MM MMM MM  AA   AA   TT    HHHHHHH" << endl;
	cout << "MM     MM AAAAAAAAA  TT    HH   HH" << endl;
	cout << "MM     MM AA     AA  TT    HH   HH" << endl << endl << endl;
}

void answerGet(andrewMath &game, int &answerGuess, int &numGuesses, int &problemScore_i, int &scoreTotal, bool hCore)
{
	int t = 0;
	bool x = 0;
	time_t start;
	time(&start);
	double problemScore_d = 0.0;
	if (game.isAlgebra()) // For fun. My son keeps wanting to be funny and answering 'x' for algebra problems.
	{
		while (!game.checkAnswer(answerGuess))
		{
			intInput(answerGuess, INT_MIN, INT_MAX, "Invalid guess. Enter again. = ");
			if (game.checkAnswer(answerGuess))
			{
				if (!hCore)
					t = scoreCalc(problemScore_d, game.getOperator(), game.getLevel(game.getOperator()), numGuesses, start, game.isAlgebra());
				else
					t = scoreCalc(problemScore_d, game.getOperator(), game.getLevel(game.getOperator()), numGuesses, start, game.isAlgebra(), 1);
				problemScore_i = static_cast<int>(problemScore_d);
				scoreTotal += problemScore_i;

				if (hCore && t > HCORE_TIME_LIMIT)
				{
					cout << "Correct, but you took too long. Zero points awarded." << endl <<
						"Points: " << problemScore_i << "  Total: " << scoreTotal
						<< "  Time: " << t << " Seconds" << endl;
					return;
				}
				cout << "Correct!" << endl <<
					"Points: " << problemScore_i << "  Total: " << scoreTotal
					<< "  Time: " << t << " Seconds" << endl;

			}
			else
			{
				if (hCore)
				{
					time_t final;
					time(&final);
					t = final - start;
					cout << "Incorrect. Zero points awarded." << endl <<
						"Points: 0  Total: " << scoreTotal
						<< "  Time: " << t << " Seconds" << endl;
					return;
				}
				if (numGuesses == 3)
				{
					cout << "Incorrrect.\nToo many incorrect answers. Zero points awarded." << endl
						<< "Total: " << scoreTotal << endl << endl;
					return;
				}
				cout << "Incorrect. Try again. = ";
				numGuesses++;
				// End problem and award zero points.
			}
		}
	}
	else
	{
		while (!game.checkAnswer(answerGuess))
		{
			intInput(answerGuess, INT_MIN, INT_MAX, "Invalid guess. Enter again. = ");
			if (game.checkAnswer(answerGuess))
			{
				if (!hCore)
					t = scoreCalc(problemScore_d, game.getOperator(), game.getLevel(game.getOperator()), numGuesses, start, game.isAlgebra());
				else
					t = scoreCalc(problemScore_d, game.getOperator(), game.getLevel(game.getOperator()), numGuesses, start, game.isAlgebra(), 1);
				problemScore_i = static_cast<int>(problemScore_d);
				scoreTotal += problemScore_i;

				if (hCore && t > HCORE_TIME_LIMIT)
				{
					cout << "Correct, but you took too long. Zero points awarded." << endl <<
						"Points: " << problemScore_i << "  Total: " << scoreTotal
						<< "  Time: " << t << " Seconds" << endl;
					return;
				}
				cout << "Correct!" << endl <<
					"Points: " << problemScore_i << "  Total: " << scoreTotal
					<< "  Time: " << t << " Seconds" << endl;

			}
			else
			{
				if (hCore)
				{
					time_t final;
					time(&final);
					t = final - start;
					cout << "Incorrect. Zero points awarded." << endl <<
						"Points: 0  Total: " << scoreTotal
						<< "  Time: " << t << " Seconds" << endl;
					return;
				}
				if (numGuesses == 3)
				{
					cout << "Incorrrect.\nToo many incorrect answers. Zero points awarded." << endl
						<< "Total: " << scoreTotal << endl << endl;
					return;
				}
				cout << "Incorrect. Try again. = ";
				numGuesses++;
				// End problem and award zero points.
			}
		}
	}
}

void gameLoop(string name, andrewMath &game, const int numRounds, bool hCore = 0)
{
	system("cls");
	int answerGuess = 0, numGuesses, scoreTotal = 0, problemScore;

	for (int i = 0; i < numRounds; i++)
	{
		game.newProblem();
		numGuesses = 0;
		problemScore = 0;

		if (game.checkAnswer(answerGuess))
			answerGuess += 1;						 // Make sure the value stored in answerGuess is different from correct answer
		if (!game.isAlgebra())
		{
			if (!hCore)
			{
				cout << endl << "Problem " << (i + 1) << ':' << endl
					<< game.getFirst() << ' '
					<< game.getOperator() << ' '
					<< game.getSecond() << " = ";
				answerGet(game, answerGuess, numGuesses, problemScore, scoreTotal, hCore);
			}
			else
			{
				cout << endl << "Problem " << (i + 1) << ':' << endl
					<< game.getFirst() << ' '
					<< game.getOperator() << ' '
					<< game.getSecond() << " = ";
				answerGet(game, answerGuess, numGuesses, problemScore, scoreTotal, hCore);
			}
		}
		else
		{
			if (!hCore)
			{
				cout << endl << "Problem " << (i + 1) << ':' << endl
					<< game.getFirst() << ' '
					<< game.getOperator() << " x "
					<< " = " << game.getSecond() << endl;
				cout << "What is x? ";
				answerGet(game, answerGuess, numGuesses, problemScore, scoreTotal, hCore);
			}
			else
			{
				cout << endl << "Problem " << (i + 1) << ':' << endl
					<< game.getFirst() << ' '
					<< game.getOperator() << " x "
					<< " = " << game.getSecond() << endl;
				cout << "What is x? ";
				answerGet(game, answerGuess, numGuesses, problemScore, scoreTotal, hCore);
			}
		}
	}
	system("cls");
	cout << "The game has ended!" << endl << endl;
	cout << "You have accumulated a total of " << scoreTotal << " points!" << endl << endl;
	highScoreEntry submit(name, scoreTotal);
	if (submit.isHighscore())															  // Program could easily be broken here....
	{																					  // 
		string subYN;																	  // 
		cout << "You have qualified for the highscore list!" << endl;					  // 
		cout << "Would you like to submit your score to the highscore list? (Y/N) ";	  // 
		cin >> subYN;																	  // 
		if (tolower(subYN.at(0) == 'y'))										  // 
		{																				  // 
			submit.writeScore();														  // 
			cout << endl << "You score has been added to the highscore list!" << endl;	  // 
			cout << "Would you like to view the highscores? (Y/N) ";
			cin >> subYN;
			if (tolower(subYN.at(0) == 'y'))
			{
				printScores(submit.getScorePos());
				system("pause");
			}
		}

	}
	else
	{
		cout << "Unfortunately, you did not qualify for the highscore list." << endl << endl;
		system("pause");
	}
	// End of game message									   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Submit for scoring									   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Check if high score and output message:				   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// If high score, ask if would like to submit to table ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// If not, notify, and wait to quit					   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}		

void main()
{
	cout << "POO!" << endl;
	system("pause");
	cout << '\f'; // form feed
	system("pause");
	string name = "";
	int menuSelection = 0;
	while (true) // Escape method by choosing quit from the menu
	{
		printTitle();
		mainMenuSelection(menuSelection);
		if (menuSelection == 1)
		{
			gameMainMenu(menuSelection);
			if (menuSelection == 1)
			{
				// Run leveled game functions
				int numRounds = 0, levelSelection = 0;
				system("cls");
				cout << "Leveled game!" << endl << endl;/////////////////////////
				cout << "Enter your name: ";
				cin.ignore(INT_MAX, '\n');
				getline(cin,name);
				cout << "Enter your desired difficulty (1-10): ";
				intInput(levelSelection, 1, 10, "Invalid difficulty. Enter again: ");
				cout << "Enter the number of rounds (maximum of 100): ";
				intInput(numRounds, 1, 100, "Invalid number of rounds. Enter again: ");
				cout << endl << "You've chosen to play " << numRounds 
					<< (numRounds == 1 ? " round" : " rounds") << " at a difficulty of " << levelSelection << endl;
				system("pause");

				andrewMath leveledGame(levelSelection);
				gameLoop(name, leveledGame, numRounds);
			}
			else if (menuSelection == 2)
			{
				// Run custom game functions
				int numRounds = 0, add = 0, sub = 0, div = 0, mul = 0, alg = 0;
				system("cls");
				cout << "Cutsom game!" << endl << endl;
				cout << "Enter your name: ";
				cin.ignore(INT_MAX, '\n');
				getline(cin, name);
				cout << "Please refer to mathConfiguration.txt to view/modify the difficulty values. For all types, 0 disables." << endl;
				cout << "Enter the addition difficulty (1-10): ";
				intInput(add, 0, 10, "Invalid value. Enter again: ");
				cout << "Enter the subtraction difficulty (1-10): ";
				intInput(sub, 0, 10, "Invalid value. Enter again: ");
				cout << "Enter the multiplication difficulty (1-7): ";
				intInput(mul, 0, 7, "Invalid value. Enter again: ");
				cout << "Enter the division difficulty (1-7): ";
				intInput(div, 0, 7, "Invalid value. Enter again: ");
				cout << "Enter the algebra difficulty (1-3): ";
				intInput(alg, 0, 3, "Invalid value. Enter again: ");
				cout << "Enter the number of rounds (maximum of 100): ";
				intInput(numRounds, 1, 100, "Invalid number of rounds. Enter again: ");

				cout << endl << "Your custom game has been configured as: " << endl
					<< "	Addition: Level " << add << endl
					<< "	Subtraction: Level " << sub << endl
					<< "	Multiplication: Level " << mul << endl
					<< "	Division: Level " << div << endl
					<< "	Algebra: Level " << alg << endl
					<< "	Rounds: " << numRounds << endl << endl;
				system("pause");

				andrewMath customGame(add, sub, mul, div, alg);
				gameLoop(name, customGame, numRounds);
			}
			if (menuSelection == 3)
			{
				int numRounds = 0, add = 0, sub = 0, div = 0, mul = 0, alg = 0;
				system("cls");
				cout << "Hardcore game!" << endl << endl;
				cout << "Enter your name: ";
				cin.ignore(INT_MAX, '\n');
				getline(cin, name);
				cout << "Please refer to mathConfiguration.txt to view/modify the difficulty values. \nFor all types, 0 disables.\n" << endl;
				cout << "Enter the addition difficulty (1-10): ";
				intInput(add, 0, 10, "Invalid value. Enter again: ");
				cout << "Enter the subtraction difficulty (1-10): ";
				intInput(sub, 0, 10, "Invalid value. Enter again: ");
				cout << "Enter the multiplication difficulty (1-7): ";
				intInput(mul, 0, 7, "Invalid value. Enter again: ");
				cout << "Enter the division difficulty (1-7): ";
				intInput(div, 0, 7, "Invalid value. Enter again: ");
				cout << "Enter the algebra difficulty (1-3): ";
				intInput(alg, 0, 3, "Invalid value. Enter again: ");
				cout << "Enter the number of rounds (maximum of 100): ";
				intInput(numRounds, 1, 100, "Invalid number of rounds. Enter again: ");

				cout << endl << "Your custom game has been configured as: " << endl
					<< "	Addition: Level " << add << endl
					<< "	Subtraction: Level " << sub << endl
					<< "	Multiplication: Level " << mul << endl
					<< "	Division: Level " << div << endl
					<< "	Algebra: Level " << alg << endl
					<< "	Rounds: " << numRounds << endl << endl;
				cout << "This game is in hardcore mode.\nYou have " << HCORE_TIME_LIMIT
					<< " seconds to answer each question. \n\tIf you take longer, you get nothing." << endl
					<< "You must answer correctly or you will receive ZERO points." << endl 
					<< "Questions are worth " << HCORE_BONUS << " times as many points." << endl
					<< "Good luck." << endl << endl;
				system("pause");

				andrewMath customGame(add, sub, mul, div, alg);
				gameLoop(name, customGame, numRounds, 1);
			}
		}
		else if (menuSelection == 2)
		{
			printScores();
			cout << endl << "Press enter to return to the main menu" << endl;
			system("pause");
		}
		else if (menuSelection == 3)
		{
			return;
		}
	}
}