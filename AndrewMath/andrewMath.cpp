#include "andrewMath.h"

//// Basic type restrictions supercede algebra restrictions
//// If level 3 algebra is chosen, but only addition and subtraction
////		are valid types, then only addition/subtraction will be generated
//// 
////  If algebra:
////		Given firstNum + secondNum = resultNum (non-algebra)
////		Values are swapped if algebra
////			firstNum + resultNum = secondNum (resultNum to represent variable)
//// 
//// Public member functions:
//// newProblem() -- Randomly generate new problem
//// getFirst() -- Returns integer: value of first number 
//// getSecond() -- Returns integer: value of second number
//// getResult() -- Returns integer: result of operation
//// isAlg() -- Returns boolean: Is objects currently stored problem algebraic? 
//// getOperator() -- Returns char (+,-,*,/): Type of currently stored problem
//// checkAnswer(int) -- Returns boolean: Does passed integer match correct answer?
////
//// Algebra configuration:
////	- Does not decide range of integers
////	- Controls frequency of algebra problems
////	- Controls what types (=,-,*,/) can be generated as algebra
////	- Frequency is a ratio (ant:con) - Ex: 1:3 will produce algebra 1/3rd of the time
////	- Any ratio >= 1 will result in 100% algebra
////
//// Object creation:
////	Creating an object with NO parameters will produce only addition problems of minimum difficulty
////
////	[andrewMath objectName (int)] (1 through 10) 
////		- Creates a predetermined leveled game type
////
////	[andrewMath objectName (A, S, M, D, G)]  
////		- Creates a custom game type
////		- A (1 through 10) sets addition difficulty
////		- S (1 through 10) sets subtraction difficulty
////		- M (1 through 7) sets multiplication difficulty
////		- D (1 through 7) sets division difficulty
////		- G (1 through 3) sets algebra difficulty
////		- Any value set to 0 will disable that type of problem from being created
////			- (0, 1, 0, 1, 0) will produce only subtraction or division of level 1, with no algebra

using namespace std;
// add destructor(s)

andrewMath::andrewMath() // Default constructor
{
	loadDiffValues();
	srand(time(NULL));
	initDiff(1, 1, 0, 0, 0);
	initRandom();
	newProblem();
}

andrewMath::andrewMath(int level)
{
	loadDiffValues();
	srand(time(NULL));
	switch (level)
	{
	default:
		initDiff(1, 1, 0, 0, 0);
		break;
	case 2:
		initDiff(2, 2, 0, 0, 0);
		break;
	case 3:
		initDiff(3, 3, 0, 0, 0);
		break;
	case 4:
		initDiff(4, 4, 1, 1, 0);	// Add level 4, Sub level 4
		break;						// Mult level 1, Div level 1
	case 5:
		initDiff(5, 5, 2, 2, 0);	// Add level 5, Sub level 5
		break;						// Mult level 2, Div level 2
	case 6:
		initDiff(6, 6, 3, 3, 0);	// Add level 6, Sub level 6
		break;						// Mult level 3, Div level 3
	case 7:
		initDiff(7, 7, 4, 4, 0);	// Add level 7, Sub level 7
		break;						// Mult level 4, Div level 4
	case 8:
		initDiff(8, 8, 5, 5, 1);	// Add level 8, Sub level 8		 // Algebra level 1 (add)
		break;						// Mult level 5, Div level 5	 //
	case 9:															 
		initDiff(9, 9, 6, 6, 2);	// Add level 9, Sub level 9		 //	Algebra level 2 (add/sub)
		break;						// Mult level 6, Div level 6	 //
	case 10:														 
		initDiff(10, 10, 7, 7, 3);	// Add level 10, Sub level 10	 // Algebra level 3 (add/sub/mult/div)
		break;						// Mult level 7, Div level 7	 //
	}
	initRandom();
	newProblem();
}

andrewMath::andrewMath(int A, int S, int M, int D, int G)
{																// Single and Mixed
	loadDiffValues();
	srand(time(NULL));
	initDiff(A, S, M, D, G);
	initRandom();
	newProblem();
}

void andrewMath::initRandom()
{
	int result = 0;
	for (int i = 0; i < 3; i++)     // Get the random going (first few numbers seem somewhat predictable)
		result = rand() % 10000 + 1;
}

int andrewMath::getResult()
{
	return resultNum;
}

int andrewMath::getFirst()
{
	return firstNum;
}

int andrewMath::getSecond()
{
	return secondNum;
}

char andrewMath::getOperator()
{
	return oper;
}

bool andrewMath::checkAnswer(const int check)
{
	if (check == resultNum)
		return 1;
	else
		return 0;
}

bool andrewMath::isAlgebra()
{
	if (isAlg) return 1;
	else return 0;
}

bool andrewMath::isValidType(const int typeNum)
{
	if (isAlg) // Requesting algebra?
	{
		switch (typeNum)
		{
		default:
			if (addDiff != -1 && algMode_A[algDiff]) // Is valid mode and is enabled for algebra?
				return 1;
			else
				return 0;
		case 1:
			if (subDiff != -1 && algMode_S[algDiff])
				return 1;
			else
				return 0;
		case 2:
			if (multDiff != -1 && algMode_M[algDiff])
				return 1;
			else
				return 0;
		case 3:
			if (divDiff != -1 && algMode_D[algDiff])
				return 1;
			else
				return 0;
		}
	}
	else
	{
		switch (typeNum)
		{
		default:
			if (addDiff == -1)
				return 0;
			else
				return 1;
		case 1:
			if (subDiff == -1)
				return 0;
			else
				return 1;
		case 2:
			if (multDiff == -1)
				return 0;
			else
				return 1;
		case 3:
			if (divDiff == -1)
				return 0;
			else
				return 1;
		}
	}
}

bool andrewMath::determineAlg() // Apply the given ratio, and decide if
{
	int result;
	int ratio = // Convert ratio to integer value
		(static_cast<double>(diffAlg_Ant[algDiff]) / static_cast<double>(diffAlg_Con[algDiff])) * 10000;
	result = rand() % 10000 + 1;																	   // 
	if (result < ratio)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void andrewMath::newProblem()
{
	isAlg = 0;
	if (algDiff != -1)
		isAlg = determineAlg();
	do
	{
		currentProblemType = (rand() % 10000 + 1) % 4;
	} while (!isValidType(currentProblemType));
	switch (currentProblemType)
	{
	case 0: // add
	{
				addition setProblem(diffAdd_lower[addDiff], diffAdd_upper[addDiff], firstNum, secondNum, resultNum, oper, isAlg);
				break;

	}
	case 1: // sub
	{
				subtraction setProblem(diffSub_lower[subDiff], diffSub_upper[subDiff], firstNum, secondNum, resultNum, oper, isAlg);
				break;
	}
	case 2: // mult
	{
				multiplication setProblem(diffMult_lower[multDiff], diffMult_upper[multDiff], firstNum, secondNum, resultNum, oper, isAlg);
				break;
	}
	case 3: // div
	{
				division setProblem(diffDiv_lower[divDiff], diffDiv_upper[divDiff], firstNum, secondNum, resultNum, oper, isAlg);
				break;
	}
	default:
	{
			   cout << "ERROR IN PROBLEM TYPE - PROBLEM NOT SET - REFER TO newProblem()" << endl;
			   system("pause");
	}
	}

	// Implement member functions (in add, sub,...) to get values and set them based on algebra or not

}

void andrewMath::loadDiffValues()
{
	int d = 0;
	ifstream configIn;
	configIn.open("mathConfiguration.txt");
	if (!configIn)									 // Intended to create file so that it 
	{												 // does not need to be included with executable
		generateConfig();							 // 
		configIn.open("mathConfiguration.txt");	 // 
	}
	for (int i = 0; i < numDifficulties; i++)
	{
		configIn >> d;
		configIn >> diffAdd_lower[i] >> diffAdd_upper[i];
		configIn >> diffSub_lower[i] >> diffSub_upper[i];
	}
	configIn.ignore(INT_MAX, '&');
	for (int i = 0; i < numDifficulties - 3; i++)
	{
		configIn >> d;
		configIn >> diffMult_lower[i] >> diffMult_upper[i];
		configIn >> diffDiv_lower[i] >> diffDiv_upper[i];
	}
	configIn.ignore(INT_MAX, '&');
	for (int i = 0; i < numDifficulties - 7; i++)
	{
		configIn >> d;
		configIn >> diffAlg_Ant[i] >> diffAlg_Con[i]
			>> algMode_A[i] >> algMode_S[i] >> algMode_M[i] >> algMode_D[i];
	}
	configIn.close();
}

void andrewMath::initDiff(int A, int S, int M, int D, int G)
{
	// Add some conditions so that the values can't be set too high -> will overstep bounds of arrays
	// ADD/SUB -> numDifficulties, MULT/DIV -> numDifficulties - 3, ALG -> numDifficulties - 7
	// If too high, set as maximum difficulty. Too low, set as minimum. (no negatives)
	if (A <= 0 && S <= 0 && M <= 0 && D <= 0 && G <= 0)
	{
		addDiff = 0; // Can't all be 0 or invalid values. Default to level 1 addition
		subDiff = -1;
		multDiff = -1;
		divDiff = -1;
		algDiff = -1;
		return;
	}
	if (A < 0)
		addDiff = -1;
	else if (A > numDifficulties)
		addDiff = numDifficulties - 1;
	else
		addDiff = A - 1;

	if (S < 0)
		subDiff = -1;
	else if (S > numDifficulties)
		subDiff = numDifficulties - 1; //////////// Support -1//////////////
	else
		subDiff = S - 1;

	if (M < 0)
		multDiff = -1;
	else if (M > numDifficulties - 3)
		multDiff = numDifficulties - 4;
	else
		multDiff = M - 1;
	
	if (D < 0)
		divDiff = -1;
	else if (D > numDifficulties - 3)
		divDiff = numDifficulties - 4;
	else
		divDiff = D - 1;

	if (G < 0)
	{
		algDiff = -1;
	}
	else if (G > numDifficulties - 7)
		algDiff = numDifficulties - 8;
	else
		algDiff = G - 1; 
	//cout << addDiff << endl << subDiff << endl << multDiff << endl << divDiff << endl << algDiff << endl;
}

void andrewMath::generateConfig()
{
	ofstream genFile("mathConfiguration.txt");
    genFile << "1   1           10          1           10" << endl;
    genFile << "2   1           50          1           50" << endl;
    genFile << "3   1           100         1           100" << endl;
    genFile << "4   1           500         1           500" << endl;
    genFile << "5   1           1000        1           1000" << endl;
    genFile << "6   1           10000       1           10000" << endl;
    genFile << "7   1           10000       1           10000" << endl;
    genFile << "8   1           10000       1           10000" << endl;
    genFile << "9   1           10000       1           10000" << endl;
    genFile << "10  1           10000       1           10000" << endl;
    genFile << "0   Addlow      Addhi       Sublow      Subhi" << endl;
    genFile << "&" << endl;
    genFile << "1   1           10          1           50" << endl;
    genFile << "2   1           15          1           100" << endl;
    genFile << "3   1           20          1           200" << endl;
    genFile << "4   1           50          1           500" << endl;
    genFile << "5   1           100         1           1000" << endl;
    genFile << "6   1           1000        1           10000" << endl;
    genFile << "7   1           10000       1           10000" << endl;
    genFile << "0   Mullow      Mulhi       Divlow      Divhi" << endl;
    genFile << "&" << endl;
    genFile << "1   1       5       1       1       0       0" << endl;
    genFile << "2   1       5       1       1       1       1" << endl;
    genFile << "3   1       1       1       1       1       1" << endl;
    genFile << "0   AlgAnt  AlgCon  ModeA   ModeS   ModeM   ModeD" << endl << endl;
    genFile << "// For algebra config, default value of low = 1, high = 100 -> 1 in 100 chance of being an algebra problem" << endl;
    genFile << "// 1:1 would result in all algebra problems." << endl;
    genFile << "// Any ratios higher than 1:1 (like 2:1) are treated as 1:1." << endl;
	genFile.close();
}

int andrewMath::getLevel(char opType)
{
	switch (opType)
	{
	case '+':
		return addDiff;
	case '-':
		return subDiff;
	case '*':
		return multDiff;
	default:
		return divDiff;

	}
}


andrewMath::addition::addition(const int lower, const int upper, int& first, int& second, int& result, char& op, const bool isAlg) // addition constructor
{
	             // need reference things and alg?
	firstNum = rand() % (upper - lower) + lower;
	secondNum = rand() % (upper - lower) + lower;
	resultNum = firstNum + secondNum;

	if (isAlg)
	{
		first = firstNum;
		result = secondNum;
		second = resultNum;
		op = '+';
	}
	else
	{
		first = firstNum;
		second = secondNum;
		result = resultNum;
		op = '+';
	}

}

// subtraction subclass
andrewMath::subtraction::subtraction(const int lower, const int upper, int& first, int& second, int& result, char& op, const bool isAlg) // subtract constructor
{
	firstNum = rand() % (upper - lower) + lower;
	secondNum = rand() % (upper - lower) + lower;
	if (firstNum > secondNum)
		resultNum = firstNum - secondNum;
	else
	{
		int temp = secondNum;
		secondNum = firstNum;
		firstNum = temp;
		resultNum = firstNum - secondNum;
	}
	if (isAlg)
	{
		first = firstNum;
		result = secondNum;
		second = resultNum;
		op = '-';
	}
	else
	{
		first = firstNum;
		second = secondNum;
		result = resultNum;
		op = '-';
	}
}

// multiplication subclass
andrewMath::multiplication::multiplication(const int lower, const int upper, int& first, int& second, int& result, char& op, const bool isAlg) // mult constructor
{
	firstNum = rand() % (upper - lower) + lower;
	secondNum = rand() % (upper - lower) + lower;
	resultNum = firstNum * secondNum;
	if (isAlg)
	{
		first = firstNum;
		result = secondNum;
		second = resultNum;
		op = '*';
	}
	else
	{
		first = firstNum;
		second = secondNum;
		result = resultNum;
		op = '*';
	}
}

//division subclass
andrewMath::division::division(const int lower, const int upper, int& first, int& second, int& result, char& op, const bool isAlg) // division constructor
{
	do {
		firstNum = rand() % (upper - lower) + lower;
		secondNum = rand() % (upper - lower) + lower;

		if (firstNum < secondNum)
		{
			int temp = secondNum;
			secondNum = firstNum;    // Make sure larger value is firstNum
			firstNum = temp;
		}
		if ((upper - lower) > 500)
			secondNum /= 16;
		else if ((upper - lower) > 200)
			secondNum /= 7;	                                       // Provide some gap between numbers
		else if ((upper - lower) > 50)
			secondNum /= 5;
		if (secondNum == 0);
		else if ((firstNum % secondNum) != 0)
		{
			firstNum = firstNum - (firstNum % secondNum);              // Make evenly divisible
		}
	} while (secondNum == 0);
	resultNum = firstNum / secondNum;

	if (isAlg)
	{
		first = firstNum;
		result = secondNum;
		second = resultNum;
		op = '/';
	}
	else
	{
		first = firstNum;
		second = secondNum;
		result = resultNum;
		op = '/';
	}
}

