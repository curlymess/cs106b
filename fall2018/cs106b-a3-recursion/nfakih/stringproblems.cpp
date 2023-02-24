/*                  HW3A: stringproblems.cpp
 * The program will have four functions available: lettersOnly(),
 * isBalanced(), printBinary() and contains() for the user to
 * choose so they can modify strings.
 * by Noor Fakih      Class: CS106B-01     Professor: Marty Stepp
 */

#include "stringproblems.h"
#include "ctype.h"
#include<iostream>
using namespace std;

/*                  string lettersOnly()
 * The function will return a modified string that only has alpha
 * characters, meaning no punctuation, symbols, spaces or numbers.
 */
string lettersOnly(string s)
{
    //Base Case: if the string is empty then no modification needed, return.
    if(s.empty())
    {
        return s;
    }
    else
    {
        //Checks to see if the first index is alpha
        //if not then it will call the funtion with a substr
        //that does not include the first index
        if(!isalpha(s[0]))
        {
            return lettersOnly(s.substr(1));
        }
        //if it is alpha, then it will keep the first index
        //and call the function as a substring
        else
        {
            return s[0] + lettersOnly(s.substr(1));
        }
    }
}

/*              bool findClosingBracket()
 * The function will return if it was able to find an opening
 * and closing bracket pair next to each other and erase it
 */
bool findClosingBracket(string openingBracket, string closingBracket, string& s)
{
    int openingPos = s.rfind(openingBracket);

    //The if statement checks to see if the closing bracket
    //is right next to the opening bracket
    if(s.find(closingBracket) == openingPos+1)
    {
        //If yes then it will erase that pait
        s.erase(s.begin() + (openingPos+1));
        s.erase(s.begin() + openingPos);
        return true;
    }

    return false;
}

/*                  bool findingPairs()
 * The function will see if it can find the indicated openBracket,
 * if so then it will call findClosingBracket. If findClosingBracket
 * returns true, it means it found the matching bracket and erased them
 * so the program can then move on to the next set of brackets. If
 * it can not, then it will return false, meaning there is no pair.
 */
bool findingPairs(string openBracket, string closeBracket, string& s)
{
    //It checks to see if there is an opening bracket
    if(s.rfind(openBracket)  != string::npos)
    {
        //If there is an openbracket, it will see if there is a closing bracket
        //alongside with it, and then erase them.
        if(findClosingBracket(openBracket, closeBracket, s))
        {
            return isBalanced(s);
        }
    }

    //It returns false if there is no matching pair.
    return false;
}

/*              bool isBalanced()
 * If the string is empty, that means that it is balanced, either
 * because the original string is empty or because after all the
 * iterations, all the pairs have been erased and found to be
 * balanced. Else, it will return false, meaning that there
 * are bracket(s) that are unmatched.
 */
bool isBalanced(string s)
{
    if(s.empty())
    {
        return true;
    }
    else
    {
        //checks to see if there are any pairs, if so then it will run through the program until it is done.
        if(findingPairs("(", ")", s) || findingPairs("{", "}", s) || findingPairs("[", "]", s) || findingPairs("<", ">", s) )
        {
            return isBalanced(s);
        }
    }

    return false;
}

/*              void printBinary()
 * The function changes a hexadecimal number into a binary number.
 */
void printBinary(int n)
{
    if(n == 1 || n == 0)
    {
        cout << n;
    }
    else
    {
        //It checks to see if the hexadecimal number is negative
        if(n < 0)
        {
            //It prints the negative sign out front.
            cout << "-";

            //The number is then multiplied by -1 so it will run from
            //not on as postive.
            n *= -1;

            //printBinary is ran again at half of n
            printBinary(n/2);

            //and the remainder which would be 0 or 1 is printed.
            cout << n%2;
        }
        else
        {
            //Half of the number is passed on each iteration and the remainder,
            //which is the makeup of the binary number is outputted.
            printBinary(n/2);
            cout << n%2;
        }
    }
}


/*                   bool contains()
 * The function contains() will check to see if the second string
 * is found in the first string.
 */
bool contains(string s1, string s2)
{
    //If s2 is empty, then it can be found in s1.
    if(s2 == "")
    {
        return true;
    }

    //s1 is empty then you can not find s2 inside of it.
    else if(s1 == "")
    {
        return false;
    }
    else
    {
        //It is case insensitive and will check to see if each index is the same
        //if it is then it will move on to the next index by creating a substring of both starting
        //at index 1
        if(tolower(s1[0]) == tolower(s2[0]))
        {
            return contains(s1.substr(1), s2.substr(1));
        }

        //otherwise, it will create a substring starting at index 1 of s1 but keep
        //s2 the same so it will keep looking to see if the first index of s2 can be
        //found in s1.
        else
        {
            return contains(s1.substr(1), s2);
        }
    }
}



















