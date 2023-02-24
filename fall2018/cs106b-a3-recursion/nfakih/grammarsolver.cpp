/*                  HW3B: grammersolver.cpp
 * The objective of this program is to generate random sentences that
 * follow a set of grammar rules which the user selects by inputting
 * a file name.
 * by Noor Fakih    class: CS106B - 01      Professor: Marty Stepp
 */
#include "grammarsolver.h"
#include "map.h"
using namespace std;


/*              readFile(istream, Map<string,Vector<string>>)
 * The readFile function takes a file filled with grammar rules and formats
 * them so they may be entered into a Map. The nonterminals are entered as
 * keys, while terminals are inserted as values.
 */
void readFile(istream& input, Map< string, Vector<string> >& rules)
{
    string line;

    //It iterates through the input file, and reads it line by line, storin one line at
    //a time into the string variable named line.
    while(getline(input, line))
    {
        //The line is split into two strings, the terimnal and its values.
        Vector<string> split = stringSplit(line, "::=");

        //It takes the second string from above that holds the values and splits it further
        //by the | symbol which stands for 'or'.
        Vector<string> token = stringSplit(split[1], "|");

        //The key and its associated values are then inserted into the Map rules.
        rules.put(split[0], token);
    }

}

/*               generate(string, Map<string, Vector<string>>)
 * The function generate() will take the current symbol and randomly choose a value
 * to associate with it. It will continue to run until it changes all the nonterminal
 * values into a terminal value which will then be stored into a Vector<string>.
 */
string generate(string symbol, Map<string, Vector<string>> rules)
{
    string v;
    //Keys are nonterminals, so if it is terminal it will end.
    if( !rules.containsKey(symbol) )
    {
        v = v + " " + symbol;
        return v;
    }
    else
    {
        Vector<string> valuesInMap = (rules.get(symbol));

        //a random integer that can be found as one of the indexes in valuesInMap is generated
        int randomInt = randomInteger(0, valuesInMap.size()-1);

        //The new symbol is the value of the vector ValuesInMap at the randomly choosen index
        symbol = valuesInMap[randomInt];
        Vector<string> subtoken = stringSplit(symbol, " ");

        //The loop iterates through the values of the subtokens which are the values of the symbol
        //and asks the program to run through each value. If the value is terminal, it will be added
        //to the Map, otherwise it will keep calling generate until it breaks down to a terminal.
        for(int i = 0; i < subtoken.size(); i ++)
        {
            string results = generate(subtoken[i], rules);
            v = v + results;
        }
    }
    return v;
}


/*                   grammarGenerate(input, symbol, times)
 * The function calls readFile() to fill the Map with grammar rules, and
 * then runs through the number of times the user asked to so that the
 * program will create times number of sentences.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times)
{
    string v;

    Vector<string> answer;
    Map<string, Vector <string> > rules;

    //The map is filled.
    readFile(input, rules);

    while(times != 0){
        v = generate(symbol, rules);
        answer.add(v);
        times--;
    }



    return answer;

}
