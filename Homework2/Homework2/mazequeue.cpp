//
//  mazequeue.cpp
//  Homework2
//
//  Created by Vivianne Dinh on 2/2/20.
//  Copyright © 2020 Vivianne Dinh. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <queue>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

int main()
{
    string maze[10] = {
            "XXXXXXXXXX",
            "X...X..X.X",
            "X.XXX....X",
            "X.X.XXXX.X",
            "XXX......X",
            "X...X.XX.X",
            "X.X.X..X.X",
            "X.XXXX.X.X",
            "X..X...X.X",
            "XXXXXXXXXX"
        };
    
        if (pathExists(maze, 10,10, 4,3, 1,8))
            cout << "Solvable!" << endl;
        else
            cout << "Out of luck!" << endl;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    // make copy of maze
    string copy[nRows];
    for (int i = 0; i < nRows; i++)
        copy[i] = maze[i];
    
    queue<Coord> coordQueue;
    
    // discover start coordinate
    Coord a(sr,sc);
    coordQueue.push(a);
    copy[sr][sc] = 'X';
    Coord end(er,ec);
    
    while (!coordQueue.empty())
    {
        Coord b = coordQueue.front();
        
        coordQueue.pop();
        if (b.r() == er && b.c() == ec)
            return true;
            
        // check west
        if (copy[b.r()][b.c()-1] == '.')
        {
            copy[b.r()][b.c()-1] = 'X';
            coordQueue.push(Coord(b.r(),b.c()-1));
        }
        
        // check east
        if (copy[b.r()][b.c()+1] == '.')
        {
            copy[b.r()][b.c()+1] = 'X';
            coordQueue.push(Coord(b.r(),b.c()+1));
        }
            
        // check north
        if (copy[b.r()-1][b.c()] == '.')
        {
            copy[b.r()-1][b.c()] = 'X';
            coordQueue.push(Coord(b.r()-1,b.c()));
        }
        
        // check south
        if (copy[b.r()+1][b.c()] == '.')
        {
            copy[b.r()+1][b.c()] = 'X';
            coordQueue.push(Coord(b.r()+1,b.c()));
        }
    }

    return false;
}
