/*              HW2: N-Grams Program
 * The objective of the program is to read a user selected file
 * and create randomly generated text that will appear somewhat
 * similar to the writing style of the user selected text.
 * by: Noor Fakih <nfakih@stanford.edu>
 * class: CS106B    Term: Spring 2019
 */


#include <iostream>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "hashmap.h"
#include "vector.h"
#include "random.h"
#include "queue.h"
using namespace std;


//function prototypes (further detail where functions are defined)
//It prints out information about the program.
void welcome();
//It asks user for a file name and checks to see if it exists, if not, user is reprompted.
void getFile(string&);
//It asks user for n val, if n is less than 2, user is reprompted.
int  getNvalue();
//It function reads the file and inserts groups of words the size of n-1.
void readFile(HashMap< Queue <string>, Vector <string> > &, string &, const int);
//It asks user for the amount of number words wanted, then the random text is printed and strung together as a sentence.
void generatingRandomText(HashMap< Queue <string>, Vector <string> > &, const int);
//It runs all the above functions except welcome() to play the game.
void playNgrams();


int main()
{
    welcome();
    playNgrams();
    return 0;
}


/*                      void welcome()
 * This function will create almost a sort of welcome screen for our
 * "game." It will print out important identifying information so
 * the user may understand what the objective of the program is
 * and will better understand how to run it.
 */
void welcome()
{
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you.\n" << endl;
}

/*              void getFile(string&)
 * The getFile() prompts the user to enter a file name and checks to see
 * if it exisits. If not, the user will keep being prompted to enter a
 * different file name until the program is given a file that exisits.
 */
void getFile(string& fileName)
{
    ifstream input;
    fileName = promptUserForFile(input, "Input file name? ");
}


/*              int getNvalue()
 * This function asks the user for the n-value. If the user enters a
 * number smaller than 2 or a value that is not an integer, such as a
 * char or decimal, then the program will continusely prompt the user
 * for a correct value to be entered. The n value is returned in the
 * end of the function.
 */
int getNvalue()
{
    int nValue = getInteger("Value of N?");
    while(nValue <= 1)
    {   cout << "N must be 2 or greater." << endl;
        nValue = getInteger("Value of N? ");
    }
    cout << endl;
    return nValue;
}

/*      void readFile(Map<Queue<string>, Vector<string>>&, string&, int)
 * This function takes the fileName that was recieved earlier from the getFile
 * function and opens it for reading. It will populate the Queue window with
 * the first n-1 words in the file. The function will then read the whole file
 * word by word, shifting the window by one and populating the map with the
 * different groups of words and the words that have directly followed them.
 */
void readFile(HashMap< Queue <string>, Vector <string> >& mapOfWords, string& fileName, const int nValue){
    ifstream input;
    string word;
    Queue <string> window;
    Vector <string> firstWindow;
    input.open(fileName);
    for(int i = 0; i < nValue; i++)
    {
        input >> word;
        window.add(word);
        firstWindow.add(word);
    }
    while(input >> word)
    {
        mapOfWords[window] += word;
        window.dequeue();
        window.enqueue(word);
    }
    input.close();
    for(int i = 0; i < nValue; i++)
    {
        word = firstWindow[i];
        mapOfWords[window] += word;
        window.dequeue();
        window.enqueue(word);
    }
}

/*  void generatingRandomText(Map<Queue<string>, Vector<string>>&)
 * The program will ask the user for the number of random words they would
 * like the program to print. If 0 is entered, the program ends, otherwise
 * it will continue. The function will choose a random starting point in the
 * map and begin printing the number of random words selected. The output
 * should look like a similar style to the original input file choosen.
 */
void generatingRandomText(HashMap< Queue <string>, Vector <string> >& mapOfWords, const int nValue)
{
    int numOfWords = getInteger("# of random words to generate (0 to quit)? ");

    if(numOfWords <= nValue && numOfWords != 0)
    {
        cout << "Must be at least " + std::to_string(nValue) + " words.\n" << endl;
        numOfWords = getInteger("# of random words to generate (0 to quit)? ");
    }

    while(numOfWords != 0)
    {
        Vector< Queue <string> > keys = mapOfWords.keys();
        int randomNum = randomInteger(0, keys.size()-1);
        Queue <string> beginOfStory = keys[randomNum];
        Queue <string> restOfStory = beginOfStory;
        string currentWord;
        cout << "...";

        //The while statement prints out all the words in it, dequeuing them as it goes,
        //so it will end once its empty, meaning once all the words in it have been printed.
        while(!beginOfStory.isEmpty())
        {
            cout << " " << beginOfStory.dequeue();
        }

        //The rest of story is printed using restOfStory until the program has printed the amount
        //specified by the user.
        for(int counter = 0; counter < (numOfWords - nValue); counter++)
        {
            randomNum = randomInteger(0, mapOfWords[restOfStory].size()-1);
            currentWord = mapOfWords[restOfStory][randomNum];
            cout << " " << currentWord;
            restOfStory.dequeue();
            restOfStory.enqueue(currentWord);
        }
        cout << " ... " << endl << endl;
        numOfWords = getInteger("# of random words to generate (0 to quit)? ");
        if(numOfWords <= nValue && numOfWords != 0)
        {
            numOfWords = getInteger("Must be at least " + std::to_string(nValue) + " words.");
        }
    }
    cout << "Exiting." << endl;
}

/*                      void playNgrams()
 * The objective of this function is to run all the functions necessary
 * for playing the "game" N-Grams.
 */
void playNgrams()
{
    HashMap< Queue<string>, Vector<string> > mapOfWords;
    string fileName;
    getFile(fileName);
    int nVal = getNvalue();
    readFile(mapOfWords, fileName, nVal);
    generatingRandomText(mapOfWords, nVal);
}
