/*          HW#6 QuestionTree.h
 * Holds the names of all the functions, public and private, and private member variables that will
 * be used in the class QuestionTree.
 */
#ifndef _questiontree_h
#define _questiontree_h

#include <iostream>
#include <string>
#include "questionnode.h"
#include "ui.h"
using namespace std;

class QuestionTree {
public:
    QuestionTree();
    ~QuestionTree();
    void answerRange(int& minDepth, int& maxDepth) const;
    int countAnswers() const;
    int countQuestions() const;
    int gamesLost() const;
    int gamesWon() const;
    void mergeData(UI& ui, istream& input);
    bool playGame(UI& ui);
    void readData(istream& input);
    void writeData(ostream& output);

private:
    QuestionNode* root;
    int numLost;
    int numWon;

    //helper functions
    int countA(QuestionNode* root, int& leaf) const;
    int countQ(QuestionNode* root, int& ques) const;
    void readDataHelper(istream& input, QuestionNode* current);
    void clear(QuestionNode* root);
    void write(ostream& output, QuestionNode* current);
    bool play(UI& ui, QuestionNode* current, QuestionNode* beforeC);
    void treeHeight(int& minDepth, int& maxDepth, int currentHeight, QuestionNode* currentNode) const;
};

#endif // _questiontree_h
