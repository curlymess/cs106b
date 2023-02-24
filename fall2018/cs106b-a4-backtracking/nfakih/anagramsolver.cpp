/*                          HW#4A: Anagrams
 * The objective of the program is to find all the anagrams of the user inputted word/phrase in the max number of words
 * allowed in each set. Each set will have no more than the number of max words allowed, but could possibly have less.
 * by: Noor Fakih       class: CS106B       Instructor: Marty Stepp
 */

#include <string>
#include "letterinventory.h"
#include "set.h"
#include "strlib.h"
using namespace std;


/*              findAllPossibleWords
 * The function will iterate through the dictionary and find all the possible word sets that can be made by
 * the phrase. It will output each successful set that can use all the letters in a phrase without going over
 * the max number allowed.
 */
void findAllPossibleWords(LetterInventory& li, Set<string>& dictionary,  Vector<string>& possibleWords, int max, int maxCounter, int& counter)
{
    if(li.isEmpty() && possibleWords.size() == max)
    {
        cout << possibleWords.toString() << endl;
        maxCounter = max;
        counter++;
        return;
    }

    string current;

    for(string word : dictionary)
    {
        if(li.contains(word))
        {
            li.subtract(word);
            possibleWords.add(word);
            maxCounter -= 1;
            findAllPossibleWords(li, dictionary, possibleWords, max, maxCounter, counter);
            possibleWords.remove(possibleWords.size()-1);
            li.add(word);
        }

    }
}

/*              findAnagrams
 * The function will call findAllPossibleWords. It will check to make sure the user inputted
 * max is nonnegative and will return the number of sets that were possible to make according
 * to both the phrase and the max.
 */
int findAnagrams(string phrase, int max, Set<string>& dictionary)
{
    if(max < 0)
    {
        throw max;
    }

    int counter = 0;
    int maxCounter = 0;
    Vector<string> wordsPossible;

    LetterInventory li(toLowerCase(phrase));

    findAllPossibleWords(li, dictionary, wordsPossible, max, maxCounter, counter);

    return counter;
}




