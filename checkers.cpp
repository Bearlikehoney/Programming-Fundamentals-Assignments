#include <iostream>
using namespace std;

int main() 
{
    // This program is part of the checkers game, it will display the board and pieces
    char board[8][8] = {    
        {' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b'},
        {'b', ' ', 'b', ' ', 'b', ' ', 'b', ' '},
        {' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'r', ' ', 'r', ' ', 'r', ' ', 'r', ' '},
        {' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r'},
        {'r', ' ', 'r', ' ', 'r', ' ', 'r', ' '}
    };

    cout << "  a b c d e f g h" << endl;
    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            cout << board[i][j] << " ";
        }
        cout << 8 - i << endl;
    }   
    return 0;
}