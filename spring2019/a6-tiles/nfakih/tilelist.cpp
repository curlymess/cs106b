/*                  HW#6 tilelist.cpp
 * This program calls the tilelist.h header file. It provides functions
 * so the user may interact with the TileList object more in the GUI
 * by creating a TileList made of TileNode objects, and a class so the user
 * may change the order of the TileList, add to it, remove from it, and clear
 * the whole screen.
 * (extra) Includes the extra feature colorChange that allows the user to change
 * the color of a tile when a user right clicks on it.
 * by: Noor Fakih       CS106B      Marty Stepp
 */
#include "tilelist.h"
#include <algorithm>
#include "strlib.h"
using namespace std;

/*          TileList()
 * The constructor for TileList is created with an empty
 * front pointer.
 */
TileList::TileList(){
    front = nullptr;
    back = nullptr;
}

/*      ~TileList()
 * This is a deconstructor that the program will
 * call when it ends to delete the memory of all
 * the TileNodes.
 */
TileList::~TileList(){
    clear();
}

/*      addBack(TileNode* newNode)
 * A new TileNode will be added to the back of the list.
 */
void TileList::addBack(int x, int y, int width, int height, string color) {
    TileNode* newNode = new TileNode(x, y, width, height, color, back, nullptr);
    if (back != nullptr){
        back->next = newNode;
    } else {
        front = newNode;
    }
    back = newNode;
}

/*      addFront(TileNode* newNode)
 * A new TileNode will be added to the front of the list.
 */
void TileList::addFront(int x, int y, int width, int height, string color) {
    TileNode* newNode = new TileNode(x, y, width, height, color, nullptr, front);
    if(front != nullptr){
        front->prev = newNode;
    } else {
        back = newNode;
    }
    front = newNode;
}

/*          clear(int x, int y)
 * The function will remove all the TileNodes in the list and
 * deltes both the node and its memory space.
 */
void TileList::clear() {
    TileNode *tile = front;
    TileNode *prev = nullptr;
        while(tile != nullptr){
            prev = tile;
            tile = tile->next;
            deleteTile(prev);
        }
}

/*      deleteTile(TileNode *tile)
 * frees up the memory allocated for the single
 * tileNode passed to it.
 */
void TileList::deleteTile(TileNode *tile) {
    if (tile == front){
        front = front->next;
    } else if (tile == back){
        back = back->prev;
    }
    if (tile->prev != nullptr){
        tile->prev->next = tile->next;
    }
    if (tile->next != nullptr){
        tile->next->prev = tile->prev;
    }
    if (front == back){
        front = back = nullptr;
    }
    delete tile;
}


/*          debug()
 * The debug() option will print out all the information
 * on every TileNode in the TileList.
 */
void TileList::debug(){
    TileNode *tile = front;
    int counter = 1;
    while(tile != nullptr){
        cout << "Tile #" << counter << ":\tx = " <<  tile->x << "\ty = " << tile->y << "\tcolor= " << tile->color << endl;
        tile = tile->next;
        counter++;
    }
}

/*          drawAll(GWindow& window)
 * draws all the tiles in the linkedList
 */
void TileList::drawAll(GWindow& window) const {
    TileNode *temp = front;
    while(temp != nullptr){
        temp->draw(window);
        temp = temp->next;
    }
}

/*      getBack()
 * getBack() will return the pointer back that holds
 * the value for the last TileNode in the TileList.
 */
TileNode* TileList::getBack() const {
    return back;
}

/*      getFront()
 * getFront() will return the pointer front that holds
 * the value for the first TileNode in the TileList.
 */
TileNode* TileList::getFront() const{
    return front;
}

/*          highlight(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will change the color to yellow. It will
 * return true if there is a tile in the given location.
 */
bool TileList::highlight(int x, int y){
    TileNode *tile = locatedTile(front,  x, y);
    if(tile != nullptr){
        tile->color = "yellow";
        return true;
    }
    return false;
}

/*          lower(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will bring it to the bottom of the list and
 * return true.
 */
bool TileList::lower(int x, int y)
{   
    TileNode *tile = locatedTile(front, x, y);
    changingColors(tile);
    if (tile != nullptr && tile != front) {
        if (tile == back){
            back = back->prev;
        }
        if (tile->next != nullptr){
            tile->next->prev = tile->prev;
        }
        if (tile->prev != nullptr){
            tile->prev->next = tile->next;
        }

        front->prev = tile;
        tile->next = front;
        tile->prev = nullptr;
        front = tile;
        return true;
    }
    return false;
}

/*          merge(int x, int y)
 * Combines all the tiles touching the tileNode at the indicated
 * x and y coordinate, and replaces them with a single box.
 */
void TileList::merge(int x, int y) {
    TileNode *tile = locatedTile(front, x, y);
    if(tile != nullptr){
        string newColor = tile->color;
        int minX = tile->x;
        int minY = tile->y;
        int maxX = tile->x;
        int maxY = tile->y;

        while(tile != nullptr){
            TileNode *toDelete = tile;
            tile = tile->next;
                if (toDelete->contains(x, y)){
                    deleteTile(toDelete);
                    if(tile->x < minX){
                        minX = tile->x;
                    }
                    if(tile->x > maxX){
                        maxX = tile->x;
                    }
                    if(tile->y < minY){
                        minY = tile->y;
                    }
                    if(tile->y > maxY){
                        maxY = tile->y;
                    }
                }
            }
        addFront(minX, minY, maxX+minX, maxY+minY, newColor);
        }
}

/*          raise(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will bring it to the top of the list
 * and return true.
 */
bool TileList::raise(int x, int y){
    TileNode *tile = locatedTile(front, x, y);
    changingColors(tile);
    if (tile != nullptr && tile != back) {
        if (tile == front){
            front = front->next;
        }
        if (tile->prev != nullptr){
            tile->prev->next = tile->next;
        }
        if (tile->next != nullptr){
            tile->next->prev = tile->prev;
        }
        back->next = tile;
        tile->prev = back;
        tile->next = nullptr;
        back = tile;
        return true;
    }
    return false;
}

/*          remove(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will remove it from the list and
 * return true.
 */
bool TileList::remove(int x, int y) {

    TileNode *tile = locatedTile(front, x, y);
    if(tile != nullptr){
        deleteTile(tile);
        return true;
    }
    return false;
}

/*          removeAll(int x, int y)
 * The function will look for the TileNodes in the
 * TileList that contains the point (x,y).
 * If there is one, then it will remove it from the list and
 * permantly delete the memory space of each tile. It will
 * return how many tiles were deleted.
 */
int TileList::removeAll(int x, int y)
{
    int counter = 0;
    TileNode *tile = front;
    while(tile != nullptr){
        TileNode *toDelete = tile;
        tile = tile->next;
        if (toDelete->contains(x, y)){
            deleteTile(toDelete);
            counter++;
        }
    }
    return counter;
}

/*          locatedTile
 *  returns the tileNode that is at the inputted x and y location
 */
TileNode* TileList::locatedTile(TileNode* front, int x, int y) {
    TileNode *tile = front;
    while (tile != nullptr && !tile->contains(x, y)){
        tile = tile->next;
    }
    return tile;
}

/*      changingColors(int x, int y)
 * The function allows the user to change the color of a tile when
 * a user right clicks it while raise or lower is selected.
 */
void TileList::changingColors(TileNode* tile)
{
    int r = randomInteger(0, 255);
    int g = randomInteger(0, 255);
    int b = randomInteger(0, 255);

    std::ostringstream colorout;
    colorout << "#" << std::setbase(16) << std::setfill('0') << r << g << b;
    std::string newColor = colorout.str();

    cout<< newColor << endl;
    if(tile != nullptr){
        tile->color = newColor;
    }
}
