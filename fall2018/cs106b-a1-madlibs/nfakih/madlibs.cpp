//homework #1
//madlibs c++ program
    //reads supported .txt files and plays a madlib game
//cs106B - 01
//by Noor Fakih

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;


//prototypes
void welcome();
void readFile(int&, string*, string);
int  count(string);//gets number of placholders per line
void getResponses(string*, string*, int&);//asks user what word to replace placeholders with
void printStory(string*, string*, string, int);//replaces placeholders w responses
bool anotherStory(int, int);//allows more stories to be played, if not then prints stats

int main() {

    int totalCount = 0;//total number of words btw <> in a story
    string wordType[50];//array for placeholders
    int finalCount = 0;//total num of placeholders replaced
    int totalStories = 0;//counter for the num of stories used
    bool again = true;//var to end while loop if no more stories want to be played
    ifstream inputFile;
    string fileName;
    welcome();

    while(again)//loop will run until user indicates they no longer wants to play
    {
        fileName = promptUserForFile(inputFile,"Enter your MadLib file name: ", "Sorry, file does not exist, please try again");

        readFile(totalCount, wordType, fileName);
        finalCount += totalCount;//keeps track of the total num of placeholders in each story played
        totalStories++;//increments each time a new story is played
        string responses[totalCount];
        getResponses(wordType, responses, totalCount);
        printStory(wordType, responses, fileName, totalCount);

        again = anotherStory(totalStories, finalCount);
        inputFile.close();
    }

    return 0;
}


//functions
//It will just display a welcome message for the game so the user understands the program
void welcome(){
    cout << "Welcome to CS 106B Mad Libs!" << endl
         << "I will ask you to provide various words"<< endl
         << "and phrases to fill in a story." << endl
         << "At the end, I will display your story to you." << endl << endl;
}

//The user will be asked for a file that they want to use for the MadLib game
//
void readFile(int &totalCount, string wordType[], string fileName){
    //variables
        ifstream inputFile;
        string line;//for reading the file
        int startPos;//where the < is located
        int endPos;//where > is located
        string word;//word in between < and >
        int counter;//number of words btw <> in each line
        int openCheckPos, closeCheckPos;
        string checkStr;
            totalCount = 0;//resets total count of placeholders in the story

            int i = 0;//counter that tells pos to add placeholder type in wordType
            inputFile.open(fileName);

            while(getline(inputFile, line))
            {

                //will run the current line of code and store the num of placeholders
                counter = count(line);

                //store the total number of placeholders in the story as the program runs through the story
                totalCount += counter;

                while (counter != 0)
                {

                     startPos = stringIndexOf(line, "<");
                     endPos = stringIndexOf(line, ">");

                     //creates suubstring which would be checked for any unmatched brackets
                     checkStr = line.substr(startPos, endPos-startPos+1);

                     //looks for the < and > found searching backwards
                     openCheckPos = checkStr.rfind("<");
                     closeCheckPos = checkStr.rfind(">");

                     //compares the pos of the < found from searching the substring forward and from searching the string backwards
                     if(openCheckPos != startPos){
                         if(openCheckPos > startPos){
                             startPos = openCheckPos;
                         }
                     }

                     //compares the pos of the > found from searching the substring forward and from searching the string backwards
                     if(closeCheckPos != endPos){
                         if(closeCheckPos > endPos){
                             endPos = closeCheckPos;
                         }
                     }

                     word = line.substr(startPos+1, endPos-startPos-1);

                     //takes out the placeholder between the < >
                        //if endPos > startPos than must add 1 to get rid of > in the current string
                     if(endPos > startPos)
                     {
                         line = line.substr(endPos+1, line.length());
                     }
                     else
                     {
                         line = line.substr(endPos, line.length());
                     }

                     counter--;
                     wordType[i]= word;
                     i++;
                }
            }
            inputFile.close();
}

//It will count the number of instances of a placeholder in each line of
//the mad lib txt file in use.
int count(string l)
{
    int openB = 0;//count var for opening brackets
    int endB = 0;//count var for closing brackets
    int pairedB = 0;//num of paired brackets
    int i;

    for (i=0; i < l.length(); i++)
    {
        // checking each character in string
        if (l[i] == '<')
        {
            openB++;
        }
        if (l[i] == '>')
        {
            endB++;
        }
    }

        //checks for number of pairs
    if(endB > openB){
        pairedB = openB;
    }
    else{
        pairedB = endB;
    }

    return pairedB;//returns number of placeholders in the line
}

//It will ask the user for what they want to replace each placeholder with.
void getResponses(string word[], string responses[], int &size){
    int s;

    for(s = 0; s < size; s++)
    {
        if(toLowerCase(word[s][0]) == 'a' || toLowerCase(word[s][0]) == 'e'
                || toLowerCase(word[s][0]) == 'i' || toLowerCase(word[s][0]) == 'o'
                || toLowerCase(word[s][0]) == 'u')
        {
            cout <<  "Please enter an " << word[s] << ": ";
        }
        else
        {
            cout <<  "Please enter a " << word[s] << ": ";
        }
        getline(cin, responses[s]);
    }
}

//It will replace the placeholders with the users input and
//display the full story. Then it will report how many
//placeholders were replaced.
void printStory(string word[], string answer[], string fileName, int count){
    ifstream inputFile;
    string line;
    string story;
    int c = 0;

    cout << "Your MadLib story: " << endl;
    inputFile.open(fileName);
    while(getline(inputFile, line))
    {
        while(line.find('<') != string::npos)
        {
            //replaces placeholder with user input
            line.replace(line.find('<'), word[c].length()+2, answer[c]);
            c++;//keeps track of what word should replace next placeholder
        }
        cout << line << endl;
        story.append(line);//strings the lines together to display full story
    }
    inputFile.close();

    cout << count << " placeholder(s) replaced." << endl;

}

// It gets user anser if another game is to be played, if not then prints stats of
//the games played so far
bool anotherStory(int stories, int placeHolders){
    bool again;

    again = getYesOrNo("Do another Mad Lib (Y/N)?",
               "Please type a word that starts with 'Y' or 'N'.");
    cout << endl;

    if(again != true)
    {
        cout << "Mad Lib stories you created: " << stories << endl;
        cout << "Total placeholders replaced: " << placeHolders << " ("
             << fixed << setprecision(1) << double(placeHolders/stories) << " per story)" << endl;
    }

    return again;//returns T or F so the program knows to terminate or not
}
