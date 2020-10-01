#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    // start with 0s on every point
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            grid[r][c] = 0;
}

bool History::record(int r, int c)
{
    //if out of bounds
    if (r < 1 || c < 1 || r > m_rows || c > m_cols)
        return false;
    
    (grid[r - 1][c - 1])++;
    return true;
}

void History::display() const
{
    clearScreen();
    
    string alp = ".ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char draw[MAXROWS][MAXCOLS];
    
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            if (grid[r][c] >= 26)
                draw[r][c] = 'Z';
            else
                draw[r][c] = alp[grid[r][c]];
    
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << draw[r][c];
        cout << endl;
    }
    
    cout << endl;
}
