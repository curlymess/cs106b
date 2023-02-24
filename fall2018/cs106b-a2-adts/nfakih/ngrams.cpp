/*              HW2B: N-Grams Program
 * The objective of the program is to read a user selected file
 * and create randomly generated text that will appear somewhat
 * similar to the writing style of the user selected text.
 * by: Noor Fakih     class: CS106B - 01
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
    cout << "Welcome to CS 106B Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}

/*              void getFile(string&)
 * The getFile() prompts the user to enter a file name and checks to see
 * if it exisits. If not, the user will keep being prompted to enter a
 * different file name until the program is given a file that exisits.
 */
void getFile(string& fileName)
{
    ifstream input;
    fileName = promptUserForFile(input, "Input file name? ",
                                 "Sorry, file does not exisit, please try again");
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

    //The while statement will keep prompting the user to enter an N value
    //until the user enters an acceptable value.
    while(nValue <= 1)
    {
        nValue = getInteger("Please enter a value 2 or greater, value of N? ");
    }

    //The nValue is returned as the whole point of the function is
    //just to get a correct nValue.
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

    //window holds the contents of the "window" as the program shifts to different words
    Queue <string> window;

    //The firstWindow holds the very first n-1 words that are read from the file
    //so it may be used for when the program wants to wrap around the text.
    Vector <string> firstWindow;

    //The selected text file is opened so the program may begin reading the file.
    input.open(fileName);

    //The loop only adds the first n-1 words into the window before it starts reading the whole text.
    for(int i = 0; i < nValue; i++)
    {
        input >> word;

        window.add(word);
        firstWindow.add(word);
    }

    //The while loop reads every word in the file and adds it to the map.
    while(input >> word)
    {
        mapOfWords[window] += word;
        window.dequeue();
        window.enqueue(word);

    }

    //At this point, all the words have been read and stored, so we no longer need the file and it is closed.
    input.close();

    //This final loop uses the firstWindow so that the map wraps around
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
    //This line of code asks the user to enter the number of words they want generated, if they do not
    //want any and/or to quit the program, they can enter 0
    int numOfWords = getInteger("# of random words to generate (0 to quit)? ");

    //This if statement gurantees that the user enters a value greater than the N value for the number of words they wish to generate.
    if(numOfWords <= nValue && numOfWords != 0)
    {
        numOfWords = getInteger("Error-- please select a value greater than the N Value \n # of random words to generate (0 to quit)? ");
    }

    //The if statement is so if the user enters 0, then the program will end, otherwise it will proceed.
    while(numOfWords != 0)
    {
        //The map is copied as a string so it can be used later for printng.
        Vector< Queue <string> > keys = mapOfWords.keys();

        //The random number will range from the index value of 0 to the last index valye in keys.
        int randomNum = randomInteger(0, keys.size()-1);

        //The first items printed from the randomly selected place will be from beginOfStory, and the
        //rest will print from restOfStory.
        Queue <string> beginOfStory = keys[randomNum];
        Queue <string> restOfStory = beginOfStory;

        //currentWord keeps track of the word that is currently being printed.
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

            //The currentWord is printed.
            cout << " " << currentWord;

            //The currentWord is then dequeue, and enqueque again so we do not
            //lose its value and is instead put in the back of the line.
            restOfStory.dequeue();
            restOfStory.enqueue(currentWord);
        }

        //This text signifies that the requested text has been completely printed.
        cout << "... " << endl << endl;

        //The user is asked again how many words they want to generate or if they
        //want to enter 0 to quit the program.
        numOfWords = getInteger("# of random words to generate (0 to quit)? ");

        //This if statement gurantees that the user enters a value greater than the N value for the number of words they wish to generate.
        if(numOfWords <= nValue && numOfWords != 0)
        {
            numOfWords = getInteger("Error-- please select a value greater than the N Value \n # of random words to generate (0 to quit)? ");
        }
    }

    //This text signifies that the user has indicated they wish to terminate the program and it is now complete.
    cout << "Exiting..." << endl;
}

/*                      void playNgrams()
 * The objective of this function is to run all the functions necessary
 * for playing the "game" N-Grams.
 */
void playNgrams()
{
    //A HashMap is used since it is more efficient and the program does not need the content of the text to be in order.
    HashMap< Queue<string>, Vector<string> > mapOfWords;

    //fileName will hold the file name of the text file that the program will read and generate text from.
    string fileName;

    getFile(fileName);

    //nVal holds the number that the words will be grouped in.
    int nVal = getNvalue();

    //The program is now filling the map
    readFile(mapOfWords, fileName, nVal);

    //The program will now create the random text and output it.
    generatingRandomText(mapOfWords, nVal);
}
