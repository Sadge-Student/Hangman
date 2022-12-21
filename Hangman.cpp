// Hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.

// all includes and definitions
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <string>
#include <cctype>
#include <limits>
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <urlmon.h>
#pragma comment(lib,"urlmon.lib")

// declared global variables
std::vector<std::string> listOfWords;
std::vector<std::string> selectedWords;
HANDLE hConsole;
bool invalidOptionMainMenu = false;

// enum of selectable options within main menu
enum class SELECTED { EXIT, PLAY, INSTRUCTIONS, CREDITS};

// declared functions
void DisplayMenu();
void Play();
void Instructions();
void SelectWords();
void DrawPoll();
void DrawTitle();
bool DownloadWordList();
bool CheckFileExists();
bool ValidInput(std::string string, std::string guessedLetters);

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // store active console screen buffer in variable
    DisplayMenu();
}

void DisplayMenu()
{
    // declare and assign local variables
    int num = -1;
    SELECTED option;

    //Stay in loop unless exited
    do
    {
        system("CLS");
        DrawTitle();

        if (invalidOptionMainMenu)
        {
            SetConsoleTextAttribute(hConsole, 12); // change color of text within console
            std::cout << "Invalid Option!\n" << std::endl;
            SetConsoleTextAttribute(hConsole, 15);
        }

        // take in option and make sure its valid
	    std::cout << "0) Exit\n1) Play\n2) Instructions\n3) Credits" << std::endl;
	    std::cout << std::endl;
        std::cout << "Select an option: ";

	    std::cin >> num;

        option = static_cast<SELECTED>(num);

        //Run specified action based on user input
        switch (option)
        {
        case SELECTED::PLAY:
            SelectWords(); // selects the word for the game
            Play(); // sets up game logic and runs game loop
            invalidOptionMainMenu = false;
            break;
        case SELECTED::INSTRUCTIONS:
            Instructions(); // displays instructions
            invalidOptionMainMenu = false;
            break;
        case SELECTED::CREDITS:
            // display credits
            system("CLS");
            DrawTitle();
            std::cout << "Created by Aaron Delaney" << std::endl;
            std::cout << "\nPress any key to return to main menu" << std::endl;
            _getch();
            invalidOptionMainMenu = false;
            break;
        default:
            invalidOptionMainMenu = true;
            num = -1;

            // resets cin input buffer required if entering invalid characters into an int
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            DisplayMenu();
            break;
        }
    } while (option != SELECTED::EXIT);
}

void Play()
{
    // declare and assign local variables
    bool gameLost = false;
    bool gameFinished = true;
    bool endSession = false;

    std::string word = selectedWords[0]; // selects the first word from the vector containing 10 randomly selected and shuffled words
    std::transform(word.begin(), word.end(), word.begin(), ::toupper); // converts the word from lower case to upper case
    std::string userInput = "";
    std::string guessedLetters = "";
    int lives = selectedWords[0].size();

    // stay in game loop until game is over
    do
    {
        std::string hiddenWord = "";
        gameFinished = true;
        system("CLS"); // clears screen
        DrawPoll(); // runs function to draw ascii art

        // displays amount of lives to user
        std::cout << "Lives: ";
        SetConsoleTextAttribute(hConsole, 12);
        for (int i = 0; i < lives; i++)
        {
            std::cout << "* ";
        }
        std::cout << "(" << std::to_string(lives) << ")" << std::endl;
        SetConsoleTextAttribute(hConsole, 15);

        std::cout << std::endl;
        std::cout << std::endl;

        //Debug displays word to developer
    	//std::cout << word << std::endl;

        // displays how many characters the word has and the replaces the correct guessed letters in the word
    	for (char letter : word)
        {
            if (guessedLetters.find(letter) != std::string::npos)
            {
                std::cout << letter << " ";
                hiddenWord.push_back(letter);
            }
            else
            {
                std::cout << "_ ";
                hiddenWord.push_back('_');
                if (!gameLost)
            		gameFinished = false;
            }
        }

        if (!gameFinished)
        {
            std::cout << std::endl;
            std::cout << std::endl;
            
            std::cout << "guessed letters: " << guessedLetters << std::endl;

            std::cout << std::endl;

            //loop until a valid input is provided e.g no duplicates, only one character and only A-Z
            do
            {
                std::cout << "Guess a letter: ";
                std::cin >> userInput;

                std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper); // converts the input from user from lower case to upper

                std::cout << std::endl;
            } while (!ValidInput(userInput, guessedLetters));

            guessedLetters.push_back(userInput[0]);
            std::cout << std::endl;

            if (word.find(userInput[0]) == std::string::npos)
            {
                lives--;
            }

            if (lives <= 0)
            {
                gameLost = true;
            }
        }
        else
        {
            std::cout << std::endl;

            if (hiddenWord.find('_') != std::string::npos)
            {
                std::cout << std::endl;
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "You Lost! :(" << std::endl;
                SetConsoleTextAttribute(hConsole, 15);
                std::cout << std::endl;
                std::cout << "The word was \"" << word  << "\"" << std::endl;
                std::cout << std::endl;
                SetConsoleTextAttribute(hConsole, 03);
                std::cout << "Press any key to return to main menu";
                SetConsoleTextAttribute(hConsole, 15);
                _getch();
                endSession = true;
            }
            else
            {
                std::cout << std::endl;
                SetConsoleTextAttribute(hConsole, 10);
                std::cout << "You Won!" << std::endl;
                SetConsoleTextAttribute(hConsole, 15);
                std::cout << std::endl;
                std::cout << "It took you " << std::to_string(guessedLetters.size()) << " guesses!" << std::endl;
                SetConsoleTextAttribute(hConsole, 03);
                std::cout << "Press any key to return to main menu";
                SetConsoleTextAttribute(hConsole, 15);
                _getch();
                endSession = true;
            }
        }
    } while (!endSession);
}

void Instructions()
{
    system("CLS");
    DrawTitle();
    std::cout << "How to play" << std::endl;
    std::cout << "Win condition: the player guesses the letters in the hangman word" << std::endl;
    std::cout << "Lose condition: the number of incorrect guesses exceeds the number of letters in the hangman word" << std::endl;
    std::cout << "\nPress any key to return to main menu" << std::endl;
    _getch();
}


void SelectWords()
{
    bool wordListExists = CheckFileExists();

    if (!wordListExists) {
        do
        {
        	DownloadWordList();
            CheckFileExists();
        } while (wordListExists);
    }

    std::ifstream inFile;
    inFile.open("wordlist.txt");

    int numOfLines = 0;
    std::string line;

    // goes through entire file and reads it into vector
    while(std::getline(inFile, line))
    {
        numOfLines++;
        listOfWords.push_back(line);
    }

    // randomly selects 10 words from list using mersenne twister seeded with random device information
    for (int i = 0; i < 10; i++)
    {
        std::random_device rd;
        std::mt19937 mt{rd()};
        int random_number = mt() % numOfLines;

        // for debugging to verify the randomness of words generated
        //std::cout << listOfWords[random_number] << std::endl;

        selectedWords.push_back(listOfWords[random_number]);
    }
    // uses mersenne twister to randomly shuffle the 10 randomly selected words within the vector
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(selectedWords), std::end(selectedWords), rng);
    std::cout << std::endl;

    // for debugging to verify the list is randomly shuffled from the words above
	//for (int i = 0; i < selectedWords.size(); i++)
    //{
    //    std::cout << selectedWords[i] << std::endl;
    //}
    //std::cout << std::endl;
}

// verifies the input is valid
bool ValidInput(std::string string, std::string guessedLetters)
{
    SetConsoleTextAttribute(hConsole, 12);
    if (string.size() > 1)
    {
        std::cout << "To many characters provided" << std::endl;
        SetConsoleTextAttribute(hConsole, 15);
        return false;
    }

    if (string[0] < 65 || string[0] > 90)
    {
        std::cout << "Invalid character only enter between A-Z" << std::endl;
        SetConsoleTextAttribute(hConsole, 15);
        return false;
    }

    if (guessedLetters.find(string[0]) != std::string::npos)
    {
        std::cout << "You guessed that letter already" << std::endl;
        SetConsoleTextAttribute(hConsole, 15);
        return false;
    }
    SetConsoleTextAttribute(hConsole, 15);
    return true;
}

bool CheckFileExists()
{
    // check if file exists
    std::ifstream inFile;
    inFile.open("wordlist.txt");

    if (inFile)
    {
        return true;
    }
	else
    {
        return false;
    }
}


bool DownloadWordList()
{
    // downloads required word list file from my github, however if you wish you could also create your own custom list once it is named "wordlist.txt" and contains a minimum of 10 words
    std::cout << "Missing word list attempting to download..." << std::endl;
    HRESULT hr = URLDownloadToFile(0, L"https://raw.githubusercontent.com/Sadge-Student/wordlist/main/wordlist.txt", L"wordlist.txt", 0, NULL);

	if (hr == S_OK) // page returns correctly
    {
        return true;
    }
	else
    {
        return false;
    }
}


void DrawPoll()
{
    SetConsoleTextAttribute(hConsole, 13);
        std::cout << R"(                                                                            
  ______ ______ ______ ______ ______
 |______|______|______|______|______|
 | | |                          | | |                               
 | | |                          |_|_|                               
 | | | 	                          :::   
 | | | 	                 -========#*.   
 | | | 	              .-===========+-   
 | | | 	         .:--=========-:. ...   
 |_|_| 	      .-==============          
 | | | 	    .-==================--:.    
 | | | 	   -===================-::::.   
 | | | 	  .--------------------:                        
 | | |                                                          
 | | |
 |_|_|__ ______                                                         
 |______|______|
	)" << '\n';
	SetConsoleTextAttribute(hConsole, 15);
}

void DrawTitle()
{
    SetConsoleTextAttribute(hConsole, 10);
    std::cout << R"(
    __  __                  __  ___          
   / / / /___ _____  ____ _/  |/  /___ _____ 
  / /_/ / __ `/ __ \/ __ `/ /|_/ / __ `/ __ \
 / __  / /_/ / / / / /_/ / /  / / /_/ / / / /
/_/ /_/\__,_/_/ /_/\__, /_/  /_/\__,_/_/ /_/ 
                  /____/                     
)" << '\n';
    SetConsoleTextAttribute(hConsole, 15);
}



