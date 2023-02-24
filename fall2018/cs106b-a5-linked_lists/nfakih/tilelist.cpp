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

/*          TileList()
 * The constructor for TileList is created with an empty
 * front pointer.
 */
TileList::TileList()
{
    front = nullptr;
}

/*      ~TileList()
 * This is a deconstructor that the program will
 * call when it ends to delete the memory of all
 * the TileNodes.
 */
TileList::~TileList()
{
    clear();
}

/*      addBack(TileNode* newNode)
 * A new TileNode will be added to the back of the list.
 */
void TileList::addBack(TileNode* newNode)
{
    if(front == nullptr)
    {
        front = newNode;
        front->next = nullptr;
    }
    else
    {
        TileNode *current = front;
        while(current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newNode;
        newNode->next = nullptr;
    }
}

/*      addFront(TileNode* newNode)
 * A new TileNode will added to the front of the list.
 */
void TileList::addFront(TileNode* newNode)
{
    if(front == nullptr)
    {
        front = newNode;
        front->next = nullptr;
    }
    else
    {
        newNode->next = front;
    }
    front = newNode;
}

/*          debug()
 * The debug() option will print out all the information
 * on every TileNode in the TileList.
 */
void TileList::debug()
{
    TileNode *tile = front;
    int counter = 1;

    while(tile != nullptr)
    {
        cout << "Tile #" << counter << ":\tx = " <<  tile->x << "\ty = " << tile->y << "\tcolor= " << tile->color << endl;
        tile = tile->next;
        counter++;
    }
}

/*      getFront()
 * getFront() will return the pointer front that holds
 * the value for the first TileNode in the TileList.
 */
TileNode* TileList::getFront() const
{
    return front;
}

/*          highlight(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will change the color to yellow.
 */
void TileList::highlight(int x, int y)
{
    TileNode *tile = front;
    while(tile != nullptr && !tile->contains(x, y))
    {
        tile = tile->next;
    }
    if(tile != nullptr)
    {
        tile->color = "yellow";
    }
}

/*          lower(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will bring it to the bottom of the list.
 */
void TileList::lower(int x, int y)
{
    TileNode *tile = front;
    TileNode *prev = nullptr;
    TileNode *end = front;

    while(tile != nullptr && !tile->contains(x, y))
    {
        prev = tile;
        tile = tile->next;
    }

    while(end->next != nullptr)
    {
        end = end->next;
    }

    if(tile == nullptr)
    {
        return;
    }
    if(tile->next == nullptr) {
        return;
    }
    if(tile == end) {
        return;
    }
    if(prev == nullptr) {
        front = front->next;
        tile->next = nullptr;
        end->next = tile;
    } else {
        prev->next = tile->next;
        tile->next = nullptr;
        end->next = tile;
    }
}

/*          raise(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will bring it to the top of the list
 * and return true.
 */
bool TileList::raise(int x, int y)
{

    TileNode *tile = front;
    TileNode *prev = nullptr;
    while(tile != nullptr && !tile->contains(x, y))
    {
        prev = tile;
        tile = tile->next;
    }

    if(tile == nullptr || tile == front)
    {
        return false;
    }

    prev->next = tile->next;
    tile->next = front;
    front = tile;

    return true;
}

/*          remove(int x, int y)
 * The function will look for the topmost TileNode in the
 * TileList according to the z-order that contains the point (x,y).
 * If there is one, then it will remove it from the list and
 * return the TileNode that was deleted.
 */
TileNode* TileList::remove(int x, int y) {

    TileNode *tile = front;
    TileNode *prev = nullptr;
    while(tile != nullptr && !tile->contains(x, y))
    {
        prev = tile;
        tile = tile->next;
    }

    if(tile == nullptr)
    {
        return nullptr;
    }
    if(tile == front)
    {
        front = front->next;
        tile = nullptr;
    }
    else
    {
        prev->next = tile->next;
    }

    return tile;
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
    TileNode *prev = nullptr;
    while(tile != nullptr)
    {
        if(tile->contains(x, y))
        {
            if(tile == front)
            {
                front = front->next;
                delete tile;
                counter++;
            }
            else
            {
                prev->next = tile->next;
                delete tile;
                counter++;
            }
        }

        prev = tile;
        tile = tile->next;
    }

    return counter;
}

/*          clear(int x, int y)
 * The function will remove all the TileNodes in the list and
 * deltes both the node and its memory space.
 */
void TileList::clear()
{
    if(front != nullptr)
    {
        TileNode *tile = front;
        TileNode *prev = nullptr;

        while(tile->next != nullptr)
        {
            prev = tile;
            tile = tile->next;
            delete prev;
        }

        front = nullptr;
    }
}

/*      changingColors(int x, int y)
 * The function allows the user to change the color of a tile when
 * a user right clicks it while raise or lower is selected.
 */
void TileList::changingColors(int x, int y)
{

    int r = randomInteger(0, 255);
    int g = randomInteger(0, 255);
    int b = randomInteger(0, 255);
    std::ostringstream colorout;
    colorout << "#" << std::setbase(16) << std::setfill('0') << r << g << b;
    std::string newColor = colorout.str();

    cout<< newColor << endl;

    TileNode *tile = front;

        while(tile != nullptr && !tile->contains(x, y))
        {
            tile = tile->next;

        }

        if(tile != nullptr)
        {
            tile->color = newColor;
        }


}

