/*                  HW#6 tilelist.h
 * The tilelist.h file states what will be defined in the tilelist.cpp file.
 * It also includes the member function changingColor (extra).
 * by: Noor Fakih       CS106B      Marty Stepp
 */

#ifndef _tilelist_h
#define _tilelist_h

#include <iostream>
#include <string>
#include <iomanip>
#include "timer.h"
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();         //constructor
    ~TileList();
    void addFront(TileNode* node);  //member functions aka methods
    void addBack(TileNode* node);
    void clear();
    void debug();
    TileNode* getFront() const;
    void highlight(int x, int y);
    void lower(int x, int y);
    bool raise(int x, int y);
    TileNode* remove(int x, int y);
    int removeAll(int x, int y);
    void changingColors(int x, int y);

    //member variables (fields)
private:
    TileNode* front;

};

#endif // _tilelist_h
