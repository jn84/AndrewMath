#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

// TODO: Rewrite all these constants to be defined by the system 
//			sizeof(int), sizeof(char), etc.
//			variable and block sizes and relationships may change between systems
// 
//		 Possibly rewrite the system to allow for changing encryption state, key, filename from outside the class
//		 Add the ability to modify the number of scores stored in the file

class highScoreEntry
{
public:
	// add destructor(s)


	// Default constructor - used to only read entries from file
	highScoreEntry() // Default constructor generates hardcoded score info. Intention is for reading high score file
	{
		inFile_HS.open(fileName_HS, ios::in | ios::binary);
		if (!inFile_HS)
		{
			createFile();
			inFile_HS.open(fileName_HS, ios::in | ios::binary);
		}
		populateVars("Unknown Winner", 0, "01/01/2000 12:00 AM");
		isHighScore = 0;
	}

	// New entry constructor - executes functions to determine if provided data made the list - can also be used to read from file
	highScoreEntry(const string nameIn, const int scoreIn) // Let class handle date/time
	{
		inFile_HS.open(fileName_HS, ios::in | ios::binary);
		if (!inFile_HS)
		{
			createFile();
			inFile_HS.open(fileName_HS, ios::in | ios::binary);
		}
		populateVars(nameIn, scoreIn, "get");
		isWritten = 0;
		highScorePosition = compareScore(score); // All data needed to write new score file now exists
	}

	// Writes the score to the file - does nothing if entry did not make the list
	void writeScore()
	{
		if (!isWritten)
		{
			isWritten = 1;
			if (!isHighScore || highScorePosition == -1)
			{
				return;
			}

			char tempBlock_1[56];
			char tempArr[1064];
			char tempFlag;
			inFile_HS.seekg(0);
			inFile_HS.read(&tempFlag, 1);

			ofstream outFile_HS_New("new.hs", ios::out | ios::binary);
			outFile_HS_New.write(&ENCRYPTION_ENABLE, 1);
			if (highScorePosition == 19)
			{
				for (int i = 0; i < 19; i++)
				{
					inFile_HS.seekg(1 + (56 * i));
					inFile_HS.read(tempBlock_1, 56);
					decryptData(tempBlock_1, tempFlag);
					encryptData(tempBlock_1);
					outFile_HS_New.write(tempBlock_1, 56);
				}
				encryptData(scoreBlock);
				outFile_HS_New.write(scoreBlock, 56);
			}
			else {
				for (int i = 0; i < 19; i++)
				{
					inFile_HS.seekg(1 + (56 * i));
					inFile_HS.read(tempBlock_1, 56);
					decryptData(tempBlock_1, tempFlag);
					if (highScorePosition == i)
					{
						encryptData(scoreBlock);
						outFile_HS_New.write(scoreBlock, 56);
					}
					if (highScorePosition != 19)
					{
						encryptData(tempBlock_1);
						outFile_HS_New.write(tempBlock_1, 56);
					}
				}
			}
			outFile_HS_New.close();
			inFile_HS.close();
			remove(fileName_HS.c_str());
			if (rename("new.hs", fileName_HS.c_str()))
			{
				inFile_HS.open(fileName_HS, ios::in | ios::binary);
			}
			else
			{
				// Unhandled exception
			}
		}
	}

	// Tell whether or not the entry made the list
	bool isHighscore()
	{
		return isHighScore;
	}

	// Get name in file at pos
	string nameInFile(int pos)
	{
		if (pos < 0 || pos > 19)
			return "OUT OF RANGE";
		if (blockPos != pos)
			parseBlock(pos);
		return readName;
	}

	// Get score ''
	int scoreInFile(int pos)
	{
		if (pos < 0 || pos > 19)
			return 0;
		if (blockPos != pos)
			parseBlock(pos);
		return readScore;
	}

	// Get date ''
	string dateInFile(int pos)
	{
		if (pos < 0 || pos > 19)
			return "OUT OF RANGE";
		if (blockPos != pos)
			parseBlock(pos);
		return readDate;
	}

	// Get position that score is/will hold
	int getScorePos()
	{
		return highScorePosition;
	}

private:
	// Block format [encypted flag - 1 byte] + 20([name - 32 bytes][score - 4 bytes][date - 20 bytes]) --> flag + 56 bytes per entry * 20 entries = 1121 bytes in file
	const char ENCRYPTION_ENABLE = '0'; // Same memory size as boolean (one byte), and more convenient to work with
	const string KEY = "pg0CB73lfGn82e9zbb3HUoJHC1vGVcJbMatIbyVj500k31VaqcOI3RZo"	   // 
		"LmjWJbu0ks0piOXfSqYld6PDKsRkBpm37Ry4eBvEkhEOwrQ7w9SotCsO"	   // Preschool cryptography
		"axsOt5qCkHSBZxQxpnvaZNOshrGuxvieTkeKSCvPAgKZtPL4P4B6Sm4B"	   // 
		"i0RV9LmEDh9DPM3nYZou4rFQUrSTtMIngTtv1CgKafVMvx5D41nLmIiU";	   // 
	const string fileName_HS = "list.hs"; // 
	// Might be better off handled outside of class
	ifstream inFile_HS;					  // 
	
	int blockPos;    // Position of currently stored block from file
	string readName; // Name in currently stored block
	int readScore;   // Score in currently stored block
	string readDate; // Date ''

	string name;     // Name for possible new entry
	int score;		 // Score ''
	string date;	 // Date ''

	bool isHighScore;      // Did the new entry make the cut?
	int highScorePosition; // Position the new entry will hold if written to file (-1 if isHighScore = 0)
	bool isWritten;

	char nameChar[32]; 	 // Arrays for use with fstream.read() / fstream.write()
	char scoreChar[4];	 // Probably not needed, but here they are. Might rewrite and remove later
	char dateChar[20];	 // 
	char scoreBlock[56]; // Combination of the above

	// Fills the given array with the current date and time
	void getTime(char timeDateArr[])
	{
		time_t rawtime;      // Integer type variable holding time elapsed since whenever..
		struct tm* timeinfo; // Pointer to structure named timeinfo

		time(&rawtime);		 // Populate rawtime
		timeinfo = localtime(&rawtime);	// Populate timeinfo structure with localtime

		strftime(timeDateArr, 25, "%m/%d/%Y %I:%M %p", timeinfo);	// DO IT
	}

	// Encypts the data in blockArr using XOR against KEY (ENCRYPTION_ENABLE != 0)
	void encryptData(char blockArr[])
	{
		if (ENCRYPTION_ENABLE != '0')
		{
			for (int i = 0; i < 56; i++)
				blockArr[i] ^= KEY[i];
		}
	}

	// Decrypts the data in provided array provided (firstByte != 0) - firstByte is pulled from high score file
	void decryptData(char blockArr[], char firstByte) // Array to work on; if the file is encrypted : if firstByte is 0, does nothing
	{
		if (firstByte != '0')
		{
			for (int i = 0; i < 56; i++)
				blockArr[i] ^= KEY[i];
		}
	}

	// Creates and populates a new high score file using fileName_HS
	void createFile()
	{
		char defaultBlock[56];
		char defaultName[32] = "Unknown Winner";
		char defaultDate[20] = "01/01/2000 12:00 AM";
		char defaultScore[4] = "   ";
		int defaultScoreInt = 0;
		for (int i = 0; i < 4; i++)
			defaultScore[i] = *(reinterpret_cast<char*>(&defaultScoreInt) + i);
		for (int i = 0; i < 32; i++)
			defaultBlock[i] = defaultName[i];
		for (int i = 0; i < 4; i++)
			defaultBlock[i + 32] = defaultScore[i];
		for (int i = 0; i < 20; i++)
			defaultBlock[i + 36] = defaultDate[i];

		ofstream buildFile(fileName_HS, ios::out | ios::binary);
		buildFile.write(&ENCRYPTION_ENABLE, 1);
		encryptData(defaultBlock);
		for (int i = 0; i < 20; i++)					// Twenty entries!								
			buildFile.write(defaultBlock, 56);			//							
		buildFile.close();
	}

	// Defines (most of) the variables (TODO: blockPos, readName, readDate, readScore)
	void populateVars(const string nameIn, int scoreIn, const string dateIn)
	{
		name = nameIn;
		score = scoreIn;
		date = dateIn;

		for (int i = 0; i < 32; i++) // Ensure array is not full of garbage
			nameChar[i] = '\0';
		for (int i = 0; i < nameIn.length() && i < 32; i++)
			nameChar[i] = nameIn[i];

		if (!dateIn.compare("get"))
		{
			getTime(dateChar);
		}
		else
		{
			for (int i = 0; i < 20; i++) // Ensure array is not full of garbage
				dateChar[i] = '\0';
			for (int i = 0; i < dateIn.length() && i < 20; i++)
				dateChar[i] = dateIn[i];
		}
		for (int i = 0; i < 4; i++)
			scoreChar[i] = *(reinterpret_cast<char*>(&scoreIn) + i);

		for (int i = 0; i < 32; i++)
			scoreBlock[i] = nameChar[i];
		for (int i = 0; i < 4; i++)
			scoreBlock[i + 32] = scoreChar[i];
		for (int i = 0; i < 20; i++)
			scoreBlock[i + 36] = dateChar[i];
		blockPos = -1;
	}

	// Further defines variables and determines if the score made the cut. Returns the position the score will hold (-1 if not made)
	int compareScore(const int inScore) // File assumed open when this function is called
	{
		char tempBlock[56];
		char tempEncryptionFlag[1];
		int pos;

		inFile_HS.seekg(0);
		inFile_HS.read(tempEncryptionFlag, 1);
		for (pos = 0; pos < 20; pos++)
		{
			inFile_HS.seekg(1 + (56 * pos));
			inFile_HS.read(tempBlock, 56);
			decryptData(tempBlock, tempEncryptionFlag[0]);
			if (*(reinterpret_cast<int*>(&tempBlock) + 8) < score)
			{
				isHighScore = 1;
				return pos;
			}
			else if (*(reinterpret_cast<int*>(&tempBlock) + 8) == score)
			{
				for (pos; pos < 20; pos++)
				{
					inFile_HS.seekg(1 + (56 * pos));
					inFile_HS.read(tempBlock, 56);
					decryptData(tempBlock, tempEncryptionFlag[0]);
					if (*(reinterpret_cast<int*>(&tempBlock) + 8) != score)
					{
						isHighScore = 1;
						return pos;
					}
					if (pos == 19)
					{
						isHighScore = 0;
						return -1;
					}
				}
			}
			else if (pos == 19 && *(reinterpret_cast<int*>(&tempBlock) + 8) >= score) // Excess logic.. oh well.
			{
				isHighScore = 0;															// Addressing issue
				return -1;
			}
		}
	}

	// Reads block [pos] from file and (re)defines readName, readScore, and readDate from it
	void parseBlock(int pos)
	{
		char tempFlag;
		char tempScore[4];
		char readBlock[56];
		blockPos = pos;
		inFile_HS.seekg(0);
		inFile_HS.read(&tempFlag, 1);
		inFile_HS.seekg(1 + (56 * pos));
		inFile_HS.read(readBlock, 56);
		decryptData(readBlock, tempFlag);
		readName = "";
		for (int i = 0; i < 32; i++)
		{
			readName += readBlock[i];
		}
		for (int i = 0; i < 4; i++)
			tempScore[i] = readBlock[i + 32];

		readScore = *reinterpret_cast<int*>(&tempScore);
		
		readDate = "";
		for (int i = 36; i < 56; i++)
			readDate += readBlock[i];
	}
};