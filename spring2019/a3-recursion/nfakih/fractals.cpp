/*                      HW3C: fractals.cpp
 * The program draws Sierpinski Triangles in the order and size that the user inputs
 * and has a floodFill function that will change the color of the object selected.
 */
#include "fractals.h"
using namespace std;


/*                  drawSierpinskiTraingle()
 * The function will draw Sierpinski Triangles at the size and order indicated.
 */
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {

    //If the coordinate values are out of bounds or the order is 0,
    // an error will be thrown so nothing will be drawn out of bounds
    // and a triangle of order 0 draws nothing.
    if(order != 0){
        if(x < 0 || y < 0 || size < 0)
        {
            throw "Error: values selected are not supported";
        }

        //The base case, which allows the program to know it should end as it
        //stops calling itself after it draws the last triangle at order 1.
        else if( order == 1 )
        {
            window.drawPolarLine(x, y, size, 0);
            window.drawPolarLine(x, y, size, -60);
            window.drawPolarLine(x + size, y, size, -120);
        }
        else
        {
            //drawSierpinskiTriangle will keep calling itself, decreasing the order and size,
            // until it hits the base case of order 1.
            drawSierpinskiTriangle(window, x, y, size/2, order - 1);
            drawSierpinskiTriangle(window, x + size/2, y, size/2, order - 1);
            drawSierpinskiTriangle(window, x + size/4, y + ((size  / 2) *  sqrt(3) / 2), size/2, order - 1);
        }
    }
}

/*                          void originalColor()
 * The function originalColor will check to see if current pixel is the original color.
 * If it is, then it will be changed, but if it is any other color it will be ignored.
 */
void floodFillHelper(GWindow& window, int x, int y, int color, int original, int& count)
{
 if (window.inCanvasBounds(x, y) && window.getPixel(x, y) == original) {

        //Every time a pixel is changes, count is incremented.
        count++;

        //It changes the pixel to the indicated color
        window.setPixel(x, y, color);

        //It will check the pixels left, right, up and down to see if in bounds
        // and call originalColor on it.
        if(window.inCanvasBounds(x+1, y))
        {
            floodFillHelper(window, x+1, y, color, original, count);
        }
        if(window.inCanvasBounds(x, y+1))
        {
            floodFillHelper(window, x, y+1, color, original, count);
        }
        if(window.inCanvasBounds(x-1, y))
        {
            floodFillHelper(window, x-1, y, color, original, count);
        }
        if(window.inCanvasBounds(x, y-1))
        {
            floodFillHelper(window, x, y-1, color, original, count);
        }
    }

}

/*              int floodFill()
 * The function checks to see if the coordinates are in bound, if they
 * are then it will call original color to change the color of the
 * pixels. It will return the number of pixels that were changed.
 */
int floodFill(GWindow& window, int x, int y, int color) {

    int original = window.getPixel(x, y);
    int count = 0;

    //The pixels check to see if the coordinates are in bounds, if not, an error is thrown
    if(!window.inCanvasBounds(x, y))
    {
        throw "out of bounds";
    }
    else
    {
        if(original != color){
            //The function is called to change the color of the pixels.
            floodFillHelper(window, x, y, color, original, count);
        }
    }
    return count;   // The number of pixels changed
}

