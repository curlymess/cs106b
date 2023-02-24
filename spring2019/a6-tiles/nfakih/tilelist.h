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
#include "gwindow.h"
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();
    ~TileList();
    void addBack(int x, int y, int width, int height, string color);
    void addFront(int x, int y, int width, int height, string color);
    void clear();
    void debug();
    void drawAll(GWindow& window) const;
    TileNode* getBack() const;
    TileNode* getFront() const;
    bool highlight(int x, int y);
    bool lower(int x, int y);
    void merge(int x, int y);
    bool raise(int x, int y);
    bool remove(int x, int y);
    int removeAll(int x, int y);

private:
    TileNode* front;
    TileNode* back;

    void deleteTile(TileNode* tile);
    TileNode* locatedTile(TileNode* front, int x, int y);
    void changingColors(TileNode* tile);
};

#endif // _tilelist_h
