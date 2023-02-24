/**
 * Assignment #7: Huffman
 * This assignment implements the file compression service Huffman. It encodes
 * a given text by counting character frequencies, constructing an encoding tree,
 * generating an encoding map, and encoding the original text file. It uses
 * these steps to both compresses and decompresses files.
 * Name: Noor Fakih     Class: CS106B       Instructor: Marty Stepp
 * SUNet ID: nfakih     Spring 2019         TA: Ozi
 */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"


//helper function prototypes
void buildEncodingMapHelper(Map<char, string>& encodingMap, HuffmanNode* encodingTree, string soFar);
HuffmanNode* parseHelper(string& soFar);
void encodeFile(istream& input, Map<char, string> encodingMap, HuffmanOutputFile& output);
void encodeBits(string bits, HuffmanOutputFile& output);
void decodeBits(HuffmanInputFile& input, ostream& output, HuffmanNode* treeRoot, HuffmanNode* currNode);

/**             buildFrequencyTable
 * Constructs a Map<char, int> that pairs each unique character that is
 * passed in by the input stream with that character's frequency and
 * returns that Map.
 */
Map<char, int> buildFrequencyTable(istream& input){
    Map<char, int> table;
    char ch;
    while(input.get(ch)){
        table[ch]++;
    }
    return table;
}

/**             buildEncodingTree
 *  Creates a HuffmanNode for every pair in the passed in frequncy table.
 *  It enqueues each char in a priority queue with that char's frequency as
 *  their priority. While the queue has more than one element, it keeps
 *  merging the two highest priority nodes to create the encoding tree.
 *  Each parent node created will hold no char and be given a priority
 *  that is the combined amount of its children. Once done, the root of
 *  the tree will be returned.
 */
HuffmanNode* buildEncodingTree(Map<char, int>& freqTable){
    PriorityQueue<HuffmanNode*> pq;
    //creates a node for every char in the freqTable
    for(char ch : freqTable.keys()){
        pq.enqueue(new HuffmanNode(ch), freqTable[ch]);
    }

    while(pq.size() > 1){
        //creation of a new parent by merging the two highest priority children
        double parentFreq = pq.peekPriority();
        HuffmanNode* zero = pq.dequeue();
        parentFreq+=pq.peekPriority();
        HuffmanNode* one = pq.dequeue();

        HuffmanNode* parent = new HuffmanNode(zero, one);
        pq.enqueue(parent, parentFreq);
    }
    //root of the tree returned
    return pq.dequeue();
}

/**           flattenTreeToHeader
 *  Recursively travels through tree and builds a string that is represenative
 *  of the whole encodingTree. Leaf nodes are denoted by having a period followed
 *  by the char it holds. Parent nodes will have an open ( before it, and ) indicates
 *  the end of the a sub tree.
 */
string flattenTreeToHeader(HuffmanNode* encodingTree){
    if(encodingTree == nullptr){
        return "";
    }
    //Base Case
    if(encodingTree->isLeaf()){
        string str = ".";
        return str + encodingTree->ch;
    }
    //Recursive Case: otherwise it is a parent so
    return "(" + flattenTreeToHeader(encodingTree->zero) + flattenTreeToHeader(encodingTree->one) + ")";
}

/**         recreateTreeFromHeader
 *  Calls recursice function parseHelper to take the inputted string str
 *  and return the root of the encodingTree the string represents.
 */
HuffmanNode* recreateTreeFromHeader(string str){
    if(str == ""){
        return nullptr;
    }
    return parseHelper(str);
}

/**        parseHelper
 * Helper function for recreateTreeFromHeader. Recursively goes through
 * the passed in string and uses the first character of the string
 * to determine if it shall create a new child or parent node. If it
 * starts with a . then it creates a new child node that stores the char
 * followed by the . and ends that line of recursive calls. Otherwise if
 * it is a parent it erases the opening (, calls the fucntion to recieve the
 * two children of the parent and creates a new parent node holding both.
 * In the end the root of the encoding tree is returned.
 */
HuffmanNode* parseHelper(string& str){
    //if child
    if(str[0] == '.'){
        char c = str[1];
        str = str.substr(2);
        return new HuffmanNode(c);
    }
    //if parent
    if(str[0] == '('){
        str = str.substr(1);
        HuffmanNode* left = parseHelper(str);
        HuffmanNode* right = parseHelper(str);
        str = str.substr(1);
        return new HuffmanNode(left, right);
    }
    return nullptr;
}

/**                 buildEncodingMap
 * Constructs the Map<int, string> that holds the encoding map for compressing the data
 * by calling the recursive function buildEncodingMapHelper which traverses the encoding tree
 * that is passed in. It then returns a copy of the map made.
 */
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree){
    Map<char, string> encodingMap;
    buildEncodingMapHelper(encodingMap, encodingTree, "");
    return encodingMap;
}

/**                 buildEncodingMapHelper
 * Recursive function helper that traverses the tree and writes a Map<int, string> that pairs
 * each char in the input stream with its binary string determined by its position in the
 * encoding tree.
 * The binary string is determined by the path of treeNode->zero and treeNode->one make before
 * getting to a node that has an actual char (i.e. a leaf node).
 */
void buildEncodingMapHelper(Map<char, string>& encodingMap, HuffmanNode* encodingTree, string soFar){
    if(encodingTree->isLeaf()){
        encodingMap.put(encodingTree->ch, soFar);
    } else{
        buildEncodingMapHelper(encodingMap, encodingTree->zero, soFar + "0");
        buildEncodingMapHelper(encodingMap, encodingTree->one, soFar + "1");
    }
}

/**                 freeTree
 * Recursively destroys all the memory for the given node root by
 * calling freeTree on to its children if it is a parent node,
 * otherwise it just deletes the node.
 */
void freeTree(HuffmanNode* t){
    if(!t->isLeaf()){
        freeTree(t->zero);
        freeTree(t->one);
    }
    delete t;
}

/**         compress
 * compresses the passed in input stream and generates a frequency map,
 * uses the map to build an encoding tree, which is then used to build an
 * encoding map. The encoding tree is also used to create a header which
 * is written into the output. Helper function encodeFile is then called
 * to iterate over hte characters in the input and write a bit sequence to
 * each char assigned code. Then it frees the memory held by the encodingTree.
 */
void compress(istream& input, HuffmanOutputFile& output){
    //setting up to compress
    Map<char, int> freqTable =  buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<char, string> encodingMap = buildEncodingMap(encodingTree);
    string flat = flattenTreeToHeader(encodingTree);
    output.writeHeader(flat);

    //iterate over the characters in the input and writing the
    //bit sequence to each character's assigned code
    encodeFile(input, encodingMap, output);

    // Free the tree memory
    freeTree(encodingTree);
}

/**         encodeFile
 * Helper function for compress. It rewinds the input stream to get access the chars.
 * It loops through each individual char in the input stream and calls encodeBits
 * to write to the output file the compressed file.
 */
void encodeFile(istream& input, Map<char, string> encodingMap, HuffmanOutputFile& output){
    rewindStream(input);
    char ch;
    while (input.get(ch)) {
        encodeBits(encodingMap[ch], output);
    }
}

/**     encodeBits
 * Helper function for encodeFile that determines if the bit that will be written into
 * the output file should be 0 or 1.
 */
void encodeBits(string bits, HuffmanOutputFile& output){
    for (int i = 0; i < bits.length(); i++) {
        int bit = (bits[i] == '0') ? 0 : 1;
        output.writeBit(bit);
    }
}

/**             decompress
 * Decompresses the passed in input stream by creating an encoding tree from
 * the header, then calling decodingBits which will decode the input bit stream
 * to its letter representation, and deletes all the memory held by the encodingTree.
 */
void decompress(HuffmanInputFile& input, ostream& output){
    //reconstruct encoding tree
    HuffmanNode* encodingTree = recreateTreeFromHeader(input.readHeader());

    //read bits one by one and decode each bit sequence into a char
    decodeBits(input, output, encodingTree, encodingTree);

    // Free the tree memory
    freeTree(encodingTree);
}

/**         decodeBits
 * Helper function for decompress. Recursively travels through the encodingTree and reads individual
 * bits from the input stream. It translates the bits into directions of when to write to the output stream
 * When it is a child node, the stored char will be written into the output stream, otherwise if it is a
 * parent then it will move on and call itself following the direction indicated from the bit from the input.
 */
void decodeBits(HuffmanInputFile& input, ostream& output, HuffmanNode* treeRoot, HuffmanNode* currNode) {
    //returns 0 or 1, returns -1 only if it is the end of file
    int treePath = input.readBit();

    //Base Case: signals end of stream when -1
    if(treePath == -1){
        return;
    }

    //assigns currNode to point to the zero or one based off the bit
    currNode = (treePath == 0) ? currNode->zero : currNode->one;

    //if parent then move on
    if(!currNode->ch){
        decodeBits(input, output, treeRoot, currNode);
    } else {
        //its a child so take the char and move on
        output.put(currNode->ch);
        currNode = treeRoot;
        decodeBits(input, output, treeRoot, currNode);
    }
}


