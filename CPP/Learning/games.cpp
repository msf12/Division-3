#include "games.h"

int main(int argc, char const *argv[])
{
	cout << "Hello! I am a game playing program!" << endl
		 << "What game would you like to play?" << endl
		 << "[0] Guessing Game" << endl
		 << "[1] Dice Poker" << endl;

	int choice;
	cin >> choice;

	switch(choice)
	{
		case 0:
			cout << "Guessing game it is!" << endl << endl;
			GuessingGame();
			break;
		case 1:
			cout << "Dice poker? Hope you're prepared to lose. ;)" << endl << endl;
			DicePoker();
			break;
		default:
			break;
	}

	return 0;
}

void GuessingGame()
{
	cout << "This is a guessing game." << endl
		 << "I will begin by chosing a number for you to guess." << endl
		 << "The number will be between 1 and 10." << endl
		 << "Good luck!" << endl << endl;

	std::mt19937_64 gen;
	std::uniform_int_distribution<int> dist(1,10);
	int num = dist(gen), guess;

	cout << "I have chosen my number." << endl << "What is your first guess?" << endl;
	cin >> guess;

	while(num - guess)
	{
		cout << "Guess again! ;)" << endl;
		cin >> guess;
	}

	cout << "You got it! :O" << endl;
}

void DicePoker()
{
	cout << "This is dice poker." << endl
		 << "We will each roll 5 dice and then chose up to 5 to re-roll" << endl
		 << "The player with the best poker hand wins." << endl
		 << "Good luck!" << endl << endl;

	std::mt19937_64 gen;
	gen.seed(time(0));
	std::uniform_int_distribution<int> dist(1,6);
	int dice[2][5];

	cout << "The dice have been rolled:" << endl
		 << "Die Nm:  1  2  3  4  5" << endl
		 << "Comptr: ";

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			dice[i][j] = dist(gen);
			cout << "[" << dice[i][j] << "]";
		}
		if(!i)
			cout << endl << "Player: ";
	}

	cout << endl << "Would you like to re-roll any dice? [Y/n]" << endl;
	char choice;
	bool reroll = false;
	cin >> choice;

	switch(choice)
	{
		case 'y':
		case 'Y':
			reroll = true;
			break;
		default:
			reroll = false;
	}


	cout << "Which dice would you like to re-roll? (ex. 1 3 4 5)" << endl;
	string input;
	cin >> input;
	
	int loc = 0;
	
	loc = input.find(" ",loc);
	
	int die = stoi(input.substr(0,loc));

	dice[1][die-1] = dist(gen);


	cout << "Our final dice read:" << endl
		 << "Die Nm:  1  2  3  4  5" << endl
		 << "Comptr: ";

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			cout << "[" << dice[i][j] << "]";
		}
		if(!i)
			cout << endl << "Player: ";
	}

	cout << endl;

}

// int countNumbers(string& s)
// {
// 	int count;
// 	for (int i = 0; i < s.length(); ++i)
// 	{
// 		//if the current char is a number and it's either the last character or followed by a non-numeric value increment the count
// 		if(s[i] >= '0' && s[i] <= '9' && (i+1<s.length() && (s[i+1] < '0' || s[i+1] > '9') || i+1>=s.length()))
// 			count++;
// 	}
// 	return count;
// }

// int* splitInput(string& s, int count)
// {
// 	int* dice = new int[count];

// }