#ifndef History_hpp
#define History_hpp

#include "globals.h"

class History
{
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    private:
        int m_rows;
        int m_cols;
        int grid[MAXROWS][MAXCOLS];
};

#endif // History_hpp
