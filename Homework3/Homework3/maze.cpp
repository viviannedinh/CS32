//
//  maze.cpp
//  Homework3
//
//  Created by Vivianne Dinh on 2/9/20.
//  Copyright © 2020 Vivianne Dinh. All rights reserved.
//

//#include <iostream>
//#include <string>
//#include <stack>
//using namespace std;
//
//bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

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

//int main()
//{
//    string maze[10] = {
//            "XXXXXXXXXX",
//            "X...X..X.X",
//            "X.XXX....X",
//            "X.X.XXXX.X",
//            "XXX......X",
//            "X...X.XX.X",
//            "X.X.X..X.X",
//            "X.XXXX.X.X",
//            "X..X...X.X",
//            "XXXXXXXXXX"
//        };
//
//        if (pathExists(maze, 10,10, 4,3, 1,8))
//            cout << "Solvable!" << endl;
//        else
//            cout << "Out of luck!" << endl;
//}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    if (sr == er && sc == ec)
        return true;
    
    maze[sr][sc] = 'X';

    // check west
    if (maze[sr][sc-1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    
    // check east
    if (maze[sr][sc+1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
        
    // check north
    if (maze[sr-1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    
    // check south
    if (maze[sr+1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;

    return false;
}
