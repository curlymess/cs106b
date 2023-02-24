/*                          HW#4B: Boggle Word Search
 * The objective of the program is to play the word game Boggle. *EXTRA FEATURE* The user
 * selects what size board they wish to play on: 4x4, 5x5, or 6x6. The user will have the
 * chance to input as many words as they want. When they give up, the computer will then
 * find the remaining words and the winner is decided based off their scores.
 * by: Noor Fakih       class: CS106B       Instructor: Marty Stepp
 */

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "bogglegui.h"
using namespace std;

//human word search almost works
//wednesday 10/24 last lecture that might be on exam

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

                    BoggleGUI::setHighlighted(row + i, col + j, true);
                    answer = lookForNeighbors(row + i, col + j, word.substr(1), board);

                    if(answer)
                    {
                        return answer;
                    }
                    else{
                        BoggleGUI::setHighlighted(row + i, col + j, false);
                        board.set(row + i, col + j, word[0]);
                    }
                }

            }
        }
    }

    return answer;
}

/*                          setScore
 * The function checks every word the human player entered
 * and determines, by the length of the word, how many points
 * the user has gained from the word and adds it to their score.
 */
void setScore(string word)
{
    if(word.length() > 7)
    {
        BoggleGUI::scorePointsHuman(11);
    }
    else if(word.length() == 7)
    {
        BoggleGUI::scorePointsHuman(5);

    }
    else if(word.length() == 6)
    {
        BoggleGUI::scorePointsHuman(3);

    }
    else if(word.length() == 5)
    {
        BoggleGUI::scorePointsHuman(2);

    }
    else
    {
        BoggleGUI::scorePointsHuman(1);

    }

}

/*                      bool humanWordSearch
 * The function verifies if the user inputted word is at least 4 letters long
 * and in the dictionary. It will then pass it onto the lookForNeighbors
 * function which will then verify if it can be found on the board. It will
 * return true if all these conditions are met and the word is found.
 */
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    bool answer = false;
    Grid<char> copy = board;
    word = toUpperCase(word);
    BoggleGUI::clearHighlighting();

    if(word.length() < BoggleGUI::MIN_WORD_LENGTH || !dictionary.contains(word))
    {
        return false;
    }

    for(int r = 0; r < BoggleGUI::BOARD_SIZE_MAX; r++)
    {
        for(int c = 0; c < BoggleGUI::BOARD_SIZE_MAX; c++)
        {
            if(board.inBounds(r, c))
            {
                if(board.get(r, c) == word[0])
                {
                    if(answer)
                    {
                        break;
                    }
                    BoggleGUI::clearHighlighting();

                    BoggleGUI::setHighlighted(r, c, true);
                    answer = lookForNeighbors(r, c, word.substr(1), copy);
                    copy = board;

                }
            }

        }
    }

    if(answer)
    {
        setScore(word);
    }

    return answer;
}


/*                  void setCscore
 * The function will take each word the computer finds and increments the score
 * based on the word's length and how much it is worth, ex. word length 7, +11pts.
 */
void setCscore(string word)
{
    if(word.length() > 7)
    {
        BoggleGUI::scorePointsComputer(11);
    }
    else if(word.length() == 7)
    {
        BoggleGUI::scorePointsComputer(5);

    }
    else if(word.length() == 6)
    {
        BoggleGUI::scorePointsComputer(3);

    }
    else if(word.length() == 5)
    {
        BoggleGUI::scorePointsComputer(2);

    }
    else
    {
        BoggleGUI::scorePointsComputer(1);

    }
}

/*              searchForNeighbors
 * The function will create words based off the starting point given by computerWordSearch.
 * If the words meet the criteria of word length, being in the dictionary and not being submitted
 * already by the human, then it will add that word to its answer.
 */
void searchForNeighbors(string word, int row, int col, Lexicon& dictionary, Grid<char>& board, Set<string>& human, Set<string>& computerWords)
{

    if(dictionary.contains(word) && !human.contains(word) && word.length() >= BoggleGUI::MIN_WORD_LENGTH)
    {
        computerWords.add(word);
        setCscore(word);
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

/*                      computerWordSearch
 * The function will iterate through all the letters in the board and give it to searchForNeighbors
 * as a starting point to search for suitable words. It will then return a Set<string> that holds
 * all the words the computer has found.
 */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {

    Set<string> answer;
    Grid<char> gridCopy = board;

    for(int r = 0; r < BoggleGUI::BOARD_SIZE_MAX; r++)
    {
        for(int c = 0; c < BoggleGUI::BOARD_SIZE_MAX; c++)
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
