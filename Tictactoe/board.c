using namespace std;

void Board::Board()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j<3; ++j)
        {
            board[i][j] = ' ';
        }
    }
}

void Board::display()
{
    cout << "---------" << endl;
    for(int i = 0; i<3; ++i)
    {
        for(int j = 0; j<3; ++j)
        {
            std::cout << board[i][j] << endl;
        }
        cout << "----------" << endl;
    }
}

char Board::winner()
{
    //Horizontal check
    for(int i = 0; i<3; ++i)
    {
        //Starts from second element and checks for equality with the first element
        for(int j = 1; j<3; ++j)
        {
            if(board[i][j] != board[i][0])
                break;
            else if(j == 2) //ending
                return board[i][j];
        }
    }

    //Vertical check
    for(int i = 0; i<3; ++i)
    {
        //Starts from second element and checks for equality with the first element
        for(int j = 1; j<3; ++j)
        {
            if(board[j][i] != board[0][i])//Check for equality with the first element
                break;
            else if(j==2)
                return board[j][i];
        }
    }

    //Diagonal
    if(board[0][0] == board[1][1] && board[0][0] == board[2][2])
    {
        return board[0][0];
    }
    if(board[0][2] == board[1][1] && board[0][2] == board[2][0])
        return board[0][0];

    return 'D';//Return Draw position
}
