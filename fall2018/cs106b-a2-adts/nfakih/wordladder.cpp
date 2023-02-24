/*              HW2A: WordLadder Program
 * The objective of the program is to create the shortest
 * "word ladder" possible from the two user selected words.
 * A word ladder is created by changing one letter at a time,
 * while the new words are still actual words. The user
 * selected words are verified to make sure they are not
 * the same word, different lengths and are found in the
 * dictionary.
 * by: Noor Fakih     class: CS106B - 01
 */


#include <iostream>
#include "console.h"
#include "filelib.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h"

using namespace std;

//function prototypes (more in depth comments near function definitions)
//Welcome() prints out information so the user may understand the purpose of the program.
void welcome();
//This function gets the user selected dictionary that will be used to verify the words.
void getDictionary(Lexicon&);
//This function gets the two user selected words and verifies they meet the requirements.
void getTwoWords(string&, string&, const Lexicon);
//This function is called in getTwoWords when the user input has not met the input requirements.
void choice(string&, string&);
//This function takes the two user selected words, and fills the Queue with possible word ladders.
void fillQueue(Queue < Stack <string>>&, const string, const string, const Lexicon, Stack<string>&);
//This fucntion takes the shortest word ladder found and outputs the answer.
void answerOutput(Stack <string>&, const string, const string);
//This function runs the necessary functions needed to play the game WordLadder
void playWordLadder();

int main()
{

    welcome();
    playWordLadder();

    return 0;
}

//function definitions
/*                      void welcome()
 * This function will create almost a sort of welcome screen for our
 * "game." It will print out important identifying information so
 * the user may understand what the objective of the program is
 * and will better understand how to run it.
 */
void welcome()
{
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl;
}


/*           void getDictionary(Lexicon& dictionaryInUse)
 * getDictionary's function prompts the user to input a dictionary file, and checks
 * if it exists. If the file does not exist, then the user is reprompted to enter
 * a new file name until it can verify that it does exist. Once the file is found,
 * it populates the Lexicon, dictionaryInUse, with the user inputted file.
 */
void getDictionary(Lexicon& dictionaryInUse)
{
    ifstream input;
    string dictionaryInput = promptUserForFile(input, "Dictionary file name: ", "Unable to open that file.  Try again.");

    //the file selected has been put into the ifstream input, and it loads it into the Lexicon dictionaryInUse.
    input >> dictionaryInUse;
}

/*           void choice(string& word1, string& word2)
 * This function is called in getTwoWords. If the user inputs two words and they do
 * not meet the program's requirements, then they are prompted to choose whether they
 * wish to change word 1, word 2, or both words. If the input is invalid, then the
 * program will, by default, be prompted to change both words 1 and 2.
 */
void choice(string& word1, string& word2)
{
    int choice;

    //The user is prompted to enter an integer and given the instructions on what
    //each integer value means.
    choice = getInteger("\nplease enter 1 to change word 1 \nor 2 to change word 2 \nor 3 to change both");

    //The switch  statement takes the user inputted choice and executes the choice.
    switch(choice)
    {
    //If the user selected 1, then the user will be prompted to only change word1.
    case 1:
        word1 = toLowerCase(getLine("\nNew Word 1 (or Enter to quit): "));
        break;

        //If the user selected 2, then the user will be prompted to only change word2.
    case 2:
        word2 = toLowerCase(getLine("\nNew Word 2 (or Enter to quit): "));
        break;

        //If the user selected 3, then the user will be prompted to change both word 1 and 2.
    case 3:
        word1 = toLowerCase(getLine("\nNew Word 1 (or Enter to quit): "));
        word2 = toLowerCase(getLine("New Word 2 (or Enter to quit): "));
        break;

        //If the user inputted any number other than 1-3, then they will prompted to change both word 1 and 2.
    default:
        cout << "Invalid choice made, automatically prompting to change both words" << endl;
        word1 = toLowerCase(getLine("\nNew Word 1 (or Enter to quit): "));
        word2 = toLowerCase(getLine("New Word 2 (or Enter to quit): "));
        break;
    }
}

/*        void getTwoWords(string &word1, string &word2, const Lexicon dictionaryInUse)
 * The objective of the program is to recieve two words that meet the requirements of not being
 * the same length, being contained in the dictionary/user selected file, and not being the same
 * word. If any of these requirements are failed, then the program will call choice() so they
 * may change one or both words.
 */
void getTwoWords(string &word1, string &word2, const Lexicon dictionaryInUse){

    word1 = toLowerCase(getLine("\nWord one (or Enter to quit): "));
    word2 = toLowerCase(getLine("Word two (or Enter to quit): "));

    //value that will determine if the while loop ends
    int ender = 0;

    while(ender == 0)
    {
        //It checks if the user entered 0 to end the program.
        if(word1.length() == 0 || word2.length() == 0)
        {
            cout << "Have a nice day!" << endl;
            break;
        }
        //It checks to see if both words are in the Lexicon.
        else if(!dictionaryInUse.contains(word1) || !dictionaryInUse.contains(word2))
        {
            cout << "The two words must be found in the dictionary." << endl;

            //Calls choice() since a requirement has been failed and new word(s) must be inputted.
            choice(word1, word2);
        }
        //It checks to see if the words are the same length, if not then user will be prompted to change words.
        else if(word1.length() != word2.length())
        {
            cout << "\nThe two words must be the same length." << endl;

            //Calls choice() since a requirement has been failed and new word(s) must be inputted.
            choice(word1, word2);
        }
        //It checks to see if the words inputted were the same and then asked to change one or both of them if they are.
        else if(word1 == word2)
        {
            cout << "The two words must be different." << endl;

            //Calls choice() since a requirement has been failed and new word(s) must be inputted.
            choice(word1, word2);
        }
        else
        {
            //The while loop will end once it reaches this part of the if-else if- else statement
            //because it means that word1 and word2 fulfill the above requirements for user input.
            ender++;
        }
    }
}


/*                             void fillQueue(Queue<Stack<string>>& word1Q,
 *      const string word1,const string word2, const Lexicon dictionaryInUse, Stack<string>& answerStack)
 * The objective of the function is go through word one and create word stacks by changing one letter at a time.
 * Once it creates a stack that reaches word2 in the shortest way possible, it saves it into answerStack and
 * passes it on.
 */
void fillQueue(Queue < Stack <string> >& word1Q, const string word1,const string word2, const Lexicon dictionaryInUse, Stack<string>& answerStack)
{
    string newWord;
    string currentWord;
    Lexicon usedWords;
    Stack<string> stackInQ;

    //The while loop will run through until answerStack is filled or the program runs through all of word1Q
    //and never populates answerStack.
    while(!word1Q.isEmpty() && answerStack.isEmpty())
    {
        stackInQ = word1Q.dequeue();
        currentWord = stackInQ.peek();

        //The loop ends when word2 is found and the current stack is saved in answerStack.
        if(currentWord == word2)
        {
            answerStack = stackInQ;
        }
        else
        {
            //The nested for loops will run through the ascii code which represents the letters a-z. It will take the letter denoted by
            //the first nested for loop and attempt to create a word by changing the current word at the indicated index from the second
            //for loop. It checks to see if those words are included in the Lexicon dictionaryInUse. If it is, then it will be added in a stack.
            for(int asciiCode = 97; asciiCode <=  122; asciiCode++)
            {
                for(int i = 0; i < word1.length(); i++)
                {
                    newWord = currentWord;
                    newWord[i] = asciiCode;

                    //        for each valid unused English word w

                    if(newWord == word2){
                        stackInQ.push(newWord);
                        answerStack = stackInQ;
                    }

                    if(dictionaryInUse.contains(newWord) && !usedWords.contains(newWord))
                    {
                        //that is a "neighbor" (differs by 1 letter)
                        //of the word on top of s:
                        Stack<string> newStack = stackInQ;


                        newStack.push(newWord);
                        word1Q.enqueue(newStack);
                        usedWords.add(newWord);
                    }
                }
            }

        }
    }
}


/*      void answerOutput(Stack <string>& answerStack, const string word1, const string word2)
 * The answerStack is found in fillQueue. If it is empty then it will outprint that a word ladder
 * could not be found, if it is not empty then it will print out the contents of the shortest
 * word ladder that goes from word1 to word2 by changing only one letter at time.
 *
 */
void answerOutput(Stack <string>& answerStack, const string word1, const string word2)
{
    //It checks to see if the answerStack is empty and if it is, then it will print that none could be found.
    if(answerStack.isEmpty())
    {
        cout << "Can not find any possible word ladders from " << word1 << " to " << word2 << "." << endl;
    }
    else
    {
        cout << "word ladder from " << word1 << " to " << word2 << ": " << endl;

        //While loop prints the contents of answer word ladder.
        while (!answerStack.isEmpty())
        {
            cout << answerStack.pop() << " ";
        }

        cout << endl;
    }


}

/*          void playWordLadder()
 * The function will call the necessary functions needed to run the game
 * WordLadders.
 */
void playWordLadder()
{
    //The two strings are given a value so it will run the first iteration of the while loop below.
    string word1 = "a";
    string word2 = "a";

    Stack <string> answerStack;
    Lexicon dictionaryInUse;

    getDictionary(dictionaryInUse);

    //The while loop until the user presses the Enter key when prompted to enter a word since the
    //words' length will then be zero and pressing enter signifies the user wants to terminate the program.
    while(word1.length() != 0 && word2.length() != 0)
    {
        getTwoWords(word1, word2, dictionaryInUse);

        if(word1.length() != 0 && word2.length() != 0)
        {
            Queue< Stack <string> > word1Q = {{word1}};
            fillQueue(word1Q, word1, word2, dictionaryInUse, answerStack);

            answerOutput(answerStack, word1, word2);
        }
    }
}



