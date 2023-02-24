/*                          HW#4: Boggle Word Search
 * The objective of the program is to play the word game Boggle.The user will have the
 * chance to input as many words as they want. When they give up, the computer will then
 * find the remaining words and the winner is decided based off their scores.
 * by: Noor Fakih       class: CS106B       Instructor: Marty Stepp
 */
#include "boggle.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "set.h"
#include "shuffle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "gui.h"
using namespace std;


// function prototype declarations
void intro();
void playGame(Grid<char>);
Grid<char> setUpBoard();
bool lookForNeighbors(int, int, string, Grid<char>&);
void settingScore(string, string, int&);
void searchForNeighbors(string, int, int, Lexicon&, Grid<char>&, Set<string>&, Set<string>&);
int getComputerScore(Set<string>);
bool printWarning(string, Set<string>&);

int main() {
    intro();
    bool play = true;
    Grid<char> board(BOARD_SIZE, BOARD_SIZE);

    while(play){
        playGame(board);
        play = getYesOrNo("Play again?");
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/*                  intro()
 * Prints a welcome message that introduces the program to the user.
 */
void intro() {
    cout << "Welcome to CS 106B Boggle!" << endl;
    cout << "This game is a search for words on a 2-D board of letter cubes." << endl;
    cout << "The good news is that you might improve your vocabulary a bit." << endl;
    cout << "The bad news is that you're probably going to lose miserably to" << endl;
    cout << "this little dictionary-toting hunk of silicon." << endl;
    cout << "If only YOU had 16 gigs of RAM!" << endl;
    cout << endl;
    getLine("Press Enter to begin the game ...");
}

void playGame(Grid<char> board){
    gui::initialize(BOARD_SIZE, BOARD_SIZE);
    board = setUpBoard();

    Set<string> humanWords;
    string enteredWord;
    int humanScore = 0;
    cout << "\nIt's your turn!\nYour words: " << humanWords.toString() << "\nYour score: " << humanScore << endl;
    cout << "Type a word (or Enter to stop): ";
    getline(cin, enteredWord);

        while(enteredWord.length() != 0){
                if(printWarning(enteredWord, humanWords)){
                    if(!humanWordSearch(board, enteredWord)){
                        cout << "That word can't be formed on this board." << endl;
                    }
                    else{
                        humanWords.add(toUpperCase(enteredWord));
                        cout << "You found a new word! \"" << toUpperCase(enteredWord) << "\"\n" << endl;
                        settingScore("human", enteredWord, humanScore);
                    }
                        cout << "Your words: " << humanWords.toString() << endl;
                        cout << "Your score: " << humanScore << endl;

                        gui::recordWord("human", toUpperCase(enteredWord));
                }

            cout << "Type a word (or Enter to stop): ";
            getline(cin, enteredWord);
        }


        Lexicon dictionary = Lexicon(DICTIONARY_FILE);
        Set<string> computerWords = computerWordSearch(board, dictionary, humanWords);

        cout << "\nIt's my turn!" << endl;
        cout << "My words: " << computerWords.toString() << endl;
        int computerScore = getComputerScore(computerWords);
        cout << "My score: " << computerScore << endl;

        if(computerScore > humanScore){
            cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!\n" << endl;
        }
        else{
            cout << "WOW, you defeated me! Congratulations!\n" << endl;
        }
}

/*                  setUpBoard()
 * The function allows the user to tell the game to generate a random board
 * or use one the user inputs. It returns the board that has been created.
 */
Grid<char> setUpBoard(){
    Vector<string> myVector = LETTER_CUBES;
    Grid<char> myGrid(BOARD_SIZE, BOARD_SIZE);
    string myString;
    string input;
    int i =0;

    cout << endl;
    if(getYesOrNo("Generate a random board?")){
        shuffle(myVector);
        for(int row = 0; row < BOARD_SIZE; row++){
            for(int col = 0; col < BOARD_SIZE; col++){
                myString = shuffle(myVector[i]);
                myString = toUpperCase(myString);
                i++;
                myGrid.set(row, col, myString[0]);
            }
        }
        shuffle(myGrid);
    } else{
        cout << "Type the 16 letters on the board: ";
        getline(cin, myString);
        while(myString.length() != BOARD_SIZE*BOARD_SIZE){
            cout << "Invalid board string. Try again." << endl;
            cout << "Type the 16 letters on the board: ";
            getline(cin, myString);
        }
        input = toUpperCase(myString);
            for(int row = 0; row < BOARD_SIZE; row++){
                for(int col = 0; col < BOARD_SIZE; col++){
                    myGrid.set(row, col, input[i]);
                    cout << input[i];
                    i++;
                }
                cout << endl;
            }
    }
    gui::labelCubes(myGrid);
    return myGrid;
}


/*                      settingScore
 * The function checks every word the player entered
 * and determines, by the length of the word, how many points
 * the player has gained from the word and adds it to their score.
 */
void settingScore(string player, string word, int& currScore)
{
    if(word.length() > 7)
    {
        currScore+=11;
    }
    else if(word.length() == 7)
    {
        currScore+=5;

    }
    else if(word.length() == 6)
    {
        currScore+=3;

    }
    else if(word.length() == 5)
    {
        currScore+=2;
    }
    else
    {
        currScore++;
    }
    gui::setScore(player, currScore);
}

/*                  getComputerScore
 * The function iterates through the Set words which holds the words the computer found,
 * and adds up the computer's points. It returns the computer's total score.
 */
int getComputerScore(Set<string> words){
    int score = 0;
    for(string w : words){
        settingScore("computer", w, score);
    }
    return score;
}


/*                      bool humanWordSearch
 * The function verifies if the user inputted word is at least 4 letters long
 * and in the dictionary. It will then pass it onto the lookForNeighbors
 * function which will then verify if it can be found on the board. It will
 * return true if all these conditions are met and the word is found.
 */
bool humanWordSearch(Grid<char>& board, string word) {

    bool answer = false;
    Grid<char> copy = board;
    word = toUpperCase(word);
    gui::clearHighlighting();

    if(word.length() == 0)
    {
        return false;
    }

    for(int r = 0; r < BOARD_SIZE_MAX; r++)
    {
        for(int c = 0; c < BOARD_SIZE_MAX; c++)
        {
            if(board.inBounds(r, c))
            {
                if(board.get(r, c) == word[0])
                {
                    if(answer)
                    {
                        break;
                    }
                    gui::clearHighlighting();
                    gui::setHighlighted(r, c, true);
                    answer = lookForNeighbors(r, c, word.substr(1), copy);
                    copy = board;
                }
            }

        }
    }
    return answer;
}

/*              printWarning()
 * This function serves the purpose of making sure the human inputted word is valid
 * and a legal move in Boggle and returns whether or not it is. It prints the apporiate
 * warning message if it is not valid.
 */
bool printWarning(string word, Set<string>& humanWords){
    Lexicon dictionary(DICTIONARY_FILE);
    bool warningNeeded = false;

    if(humanWords.contains(toUpperCase(word))){
        cout << "You have already found that word." << endl;
    }
    else if(!dictionary.contains(word)){
        cout << "That word is not found in the dictionary." << endl;
    }
    else if(word.length() < MIN_WORD_LENGTH){
        cout << "The word must have at least " << MIN_WORD_LENGTH << " letters." << endl;
    }
    else if(word.length() != 0){
        warningNeeded = true;
    }

    return warningNeeded;
}

/*              bool lookForNeighbors
 * The objective of the function is to verify the user inputted word can be
 * made with the given board by checking "neighboring" letters such as
 * letters near it horizontally, vertically and diagonally. It returns true
 * if the word can be made on the board.
 */
bool lookForNeighbors(int row, int col, string word, Grid<char>& board)
{
    bool answer = false;
    if (word.length() == 0)
    {
        return true;
    }

    //iterates through the neighbors of the current letter block
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1 ; j <= 1; j++)
        {
            if(board.inBounds(row + i, col + j) && board.get(row + i, col + j) != 1  &&  word[0] == board.get(row + i, col + j))
            {
                if(!(i == 0 && j == 0))
                {
                    //marking the letters used
                    board.set(row + i, col + j, 1);

                    gui::setHighlighted(row + i, col + j, true);
                    answer = lookForNeighbors(row + i, col + j, word.substr(1), board);

                    if(answer)
                    {
                        return answer;
                    }
                    else{
                        gui::setHighlighted(row + i, col + j, false);
                        board.set(row + i, col + j, word[0]);
                    }
                }

            }
        }
    }

    return answer;
}

/*                      computerWordSearch
 * The function will iterate through all the letters in the board and give it to searchForNeighbors
 * as a starting point to search for suitable words. It will then return a Set<string> that holds
 * all the words the computer has found.
 */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> answer;
    Grid<char> gridCopy = board;

    for(int r = 0; r < BOARD_SIZE; r++)
    {
        for(int c = 0; c < BOARD_SIZE; c++)
        {
            string word;
            if(board.inBounds(r, c))
            {
                word = board.get(r, c);
                //marking words used
                gridCopy.set(r, c, 1);
                searchForNeighbors(word, r, c, dictionary, gridCopy, humanWords, answer);

                //resetting board aka unchoose
                gridCopy = board;
            }
        }
    }
    return answer;
}


/*              searchForNeighbors
 * The function will create words based off the starting point given by computerWordSearch.
 * If the words meet the criteria of word length, being in the dictionary and not being submitted
 * already by the human, then it will add that word to its answer.
 */
void searchForNeighbors(string word, int row, int col, Lexicon& dictionary, Grid<char>& board, Set<string>& human, Set<string>& computerWords)
{
    if(dictionary.contains(word) && !human.contains(word) && word.length() >= MIN_WORD_LENGTH)
    {
        computerWords.add(word);
        gui::recordWord("computer", word);
    }
    if(!dictionary.containsPrefix(word))
    {
        return;
    }

    //iterating through all the neighbors of the current letter block
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1 ; j < 2; j++)
        {

            if(board.inBounds(row + i, col + j))
            {
                char tempC = board.get(row + i, col + j);

                if(tempC != 1)
                {
                    //choose
                    board.set(row + i, col + j, 1);

                    searchForNeighbors(word + tempC, row + i, col + j, dictionary, board, human, computerWords);

                    //unchoose
                    board.set(row + i, col + j, tempC);
                }
            }
        }
    }
}
