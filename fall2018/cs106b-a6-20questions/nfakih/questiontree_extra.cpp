/*                  HW#6: QuestionTree.cpp
 * The program allows the user to play a version of the game 20 Questions. It creates a binary tree
 * from a text file with questions when the read section is selcted. If user opts to select the play
 * option, the program will build a tree from allowing the user to enter their question and answer
 * when the computer can not guess it. Other features are also available to enhance the user's experience.
 * Noor Fakih CS106b Marty Stepp
 */

//includes extra feature where the game is capped to 20questions
#include "questiontree.h"
#include <climits>
#include "strlib.h"

//constructor for the question tree
QuestionTree::QuestionTree()
{
    root = new QuestionNode("A:computer");
    numWon = 0;
    numLost = 0;
    questionsAsked = 0;
}

//deconstructor
QuestionTree::~QuestionTree()
{
    clear(root);
}

//helper function for the deconstructor that dynamically clears all the data
void QuestionTree::clear(QuestionNode* current)
{
    if (current == nullptr)
    {
        return;
    }
    clear(current->no);
    clear(current->yes);
    delete(current);

}

//returns the amount of leafs on the tree, aka the number of possible answers
int QuestionTree::countAnswers() const
{
    int leaf = 1;
    return countA(root, leaf);
}

//helper function that helps countAnswers. it looks for answers, aka nodes
//that do not point to anything.
int QuestionTree::countA(QuestionNode* root, int& leaf) const
{
    if(root == nullptr)
    {
        return leaf;
    }
    if(root->no == nullptr && root->yes == nullptr)
    {
        leaf++;
    }
    else
    {
        countA(root->no, leaf);
        countA(root->yes, leaf);
    }
    return leaf;
}

//returns the number of questions in the question tree
int QuestionTree::countQuestions() const
{
    int ques = 0;
    return countQ(root, ques);
}

//helper function for countQuestions. it looks for nodes that point to other nodes
int QuestionTree::countQ(QuestionNode* root, int& ques) const
{
    if(root == nullptr)
    {
        return ques;
    }
    if(root->no == nullptr && root->yes == nullptr)
    {
        return ques;
    }
    else{
        ques++;
        countQ(root->no, ques);
        countQ(root->yes, ques);
    }
    return ques;
}

//uses the private member variable numLost and returns it
int QuestionTree::gamesLost() const
{
    return numLost;
}

//uses the private member variable numWon and returns it
int QuestionTree::gamesWon() const
{
    return numWon;
}

//combines two trees together
void QuestionTree::mergeData(UI& ui, istream& input)
{
    QuestionNode* temp = new QuestionNode("", nullptr, nullptr);
    readDataHelper(input, temp);
    string question = "Q:" + ui.readLine("Type a Y/N question to distinguish my data from this file:");
    bool answer = ui.readBoolean("And what is the answer for the new data?");
    QuestionNode* newQuestion = new QuestionNode(question, nullptr, nullptr);
    if(answer)
    {
        newQuestion->yes = temp;
        newQuestion->no = root;
    }
    else
    {
        newQuestion->no = temp;
        newQuestion->yes = root;
    }
    root = newQuestion;
}

//returns if the computer won or lost the game
bool QuestionTree::playGame(UI& ui)
{
    QuestionNode* current = root;
    QuestionNode* beforeC = root;
    questionsAsked = 0;
    return play(ui, current, beforeC);
}

//where the feature of the game being capped to only 20 questions is implemented
//helper function for playGame. It will traverse through the tree and ask the questions/answers
//when the user is prompted to verify if the answer is correct, the program will end.
bool QuestionTree::play(UI& ui, QuestionNode* current, QuestionNode* beforeC)
{
    if(current == nullptr)
    {
        return true;
    }
    //if this is an answer leaf
    if(current->no == nullptr && current->yes == nullptr)
    {
        bool answer = ui.readBoolean("Are you thinking of: " + current->data.substr(2) + "? ");

        if(answer)
        {
            ui.print("Hooray, I win!");
            return true;
        }
        else
        {
            string object = "A:" + ui.readLine("Drat, I lost. What was your object?");

            string question = "Q:" + ui.readLine("Type a Y/N question to distinguish " + beforeC->data.substr(2) + " from " + object.substr(2) + ": ");
            bool newAnswer = ui.readBoolean("And what is the answer for " + object.substr(2) +"? ");

            //connnect new ques to new answer
            QuestionNode* newQuestion = new QuestionNode(question, nullptr, nullptr);
            if(newAnswer)
            {
                newQuestion->yes = new QuestionNode(object, nullptr, nullptr);
            }
            else
            {
                newQuestion->no = new QuestionNode(object, nullptr, nullptr);
            }

            //put in new ques
            if(beforeC == current)
            {
                root = newQuestion;
            }
            else if(beforeC->yes == current)
            {
                beforeC->yes = newQuestion;
            }
            else
            {
                beforeC->no = newQuestion;
            }
            return false;
        }
    }
    else
    {
        //implementation of extra feature
        if(questionsAsked < 20)
        {
            bool answer = ui.readBoolean(current->data.substr(2));
            beforeC = current;
            questionsAsked++;

            if(answer)
            {
                return play(ui, current->yes, beforeC);
            }
            else
            {
                return play(ui, current->no, beforeC);
            }
        }
        else
        {
            ui.print("Gosh darn, I lost because I needed more than 20 questions :/");
            return false;
        }
    }
}

//finds the lowest and highest amount of questions one might need to answer before
//the game can end
void QuestionTree::answerRange(int& minDepth, int& maxDepth) const
{
    minDepth = INT_MAX;
    maxDepth = INT_MIN;
    int currentHeight = 0;
    treeHeight(minDepth, maxDepth, currentHeight, root);
}

//number of levels in the current binary question tree
void QuestionTree::treeHeight(int& minDepth, int& maxDepth, int currentHeight, QuestionNode* currentNode) const
{
    if(currentNode == nullptr)
    {
        minDepth = min(minDepth, currentHeight);
        maxDepth = max(maxDepth, currentHeight);
        return;
    }
    currentHeight++;
    treeHeight(minDepth, maxDepth, currentHeight, currentNode->no);
    treeHeight(minDepth, maxDepth, currentHeight, currentNode->yes);
}

//dyncamically clears the memory and calls the helper function readDataHelper
void QuestionTree::readData(istream& input)
{
    clear(root);
    readDataHelper(input, root);
}

//traverses through text file and adds to a new binary tree
void QuestionTree::readDataHelper(istream& input, QuestionNode* current)
{
    string line;
    string prefix;

    if(getline(input, line))
    {
        prefix = line.substr(0,2);
        current = new QuestionNode(line.substr(2), nullptr, nullptr);
        if(prefix == "A:")
        {
            return;
        }

        readDataHelper(input, current->no);
        readDataHelper(input, current->yes);
    }
}

//calls the helper function write
void QuestionTree::writeData(ostream& output)
{
    write(output, root);
}

//write creates a text file with the format needed so it can be used to create a binary tree
void QuestionTree::write(ostream& output, QuestionNode* current)
{
    if(current != nullptr)
    {
        output << current->data << endl;
        if(current->data.substr(0,2) == "A:")
        {
            return;
        }

        write(output, current->yes);
        write(output, current->no);
    }
}
