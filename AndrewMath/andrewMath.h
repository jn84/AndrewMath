#ifndef ANDREW_CLASS
#define ANDREW_CLASS

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>


class andrewMath
{
public:
	andrewMath();													   // Defaults to level 1 
	andrewMath(int);												   // Leveled game type
	andrewMath(int, int, int, int, int);							   // Custom game type

	void newProblem();

	///Temporary for debug
	int getResult();
	/// 


	int getFirst();				
	int getSecond();		
	char getOperator();
	bool checkAnswer(int);		
	bool isAlgebra();
	int getLevel(char);
	
private:
	int firstNum, secondNum, resultNum;
	bool isAlg;
	char oper;
	int currentProblemType;
	
	void initRandom();
	void setValues(int&, int&, int&, char&);
	bool determineAlg();
	bool isValidType(int);
	void generateConfig();

	int addDiff, subDiff, multDiff, divDiff, algDiff; // If zero, not used (stored value will be -1)
	bool isAdd, isSub, isMult, isDiv;

	static const int numDifficulties = 10;
	int		diffAdd_lower[numDifficulties],			diffAdd_upper[numDifficulties];
	int		diffSub_lower[numDifficulties],			diffSub_upper[numDifficulties];
	int		diffMult_lower[numDifficulties - 3],	diffMult_upper[numDifficulties - 3];
	int		diffDiv_lower[numDifficulties - 3],		diffDiv_upper[numDifficulties - 3];
	int		diffAlg_Ant[numDifficulties - 7],		diffAlg_Con[numDifficulties - 7];
	bool	algMode_A[numDifficulties - 7],			algMode_S[numDifficulties - 7],
			algMode_M[numDifficulties - 7],			algMode_D[numDifficulties - 7];
	// Algebra configuration
	
	void loadDiffValues();
	void initDiff(int, int, int, int, int);

	class addition
	{
		int firstNum;
		int secondNum;
		int resultNum;

	public:
		addition(const int, const int, int&, int&, int&, char&, const bool);
	};
	class subtraction
	{
		int firstNum;
		int secondNum;
		int resultNum;

	public:
		subtraction(const int, const int, int&, int&, int&, char&, const bool);

	};
	class division // Negatives? Handle secondNum = 0
	{
		int firstNum;
		int secondNum;
		int resultNum;

	public:
		division(const int, const int, int&, int&, int&, char&, const bool);

	};
	class multiplication
	{
		int firstNum;
		int secondNum;
		int resultNum;

	public:
		multiplication(const int, const int, int&, int&, int&, char&, const bool);

	};
};

#endif