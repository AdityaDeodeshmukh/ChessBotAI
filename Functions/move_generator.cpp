#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>
#include<iomanip>

using namespace std;


class ChessBoard{
    public:

        vector<int> board;

        vector<vector<int>> legalMoves;

        const vector<int> directions = {0, 1, 2, 3, 4, 5, 6, 7};

        vector<vector<int>> edges;

        const vector<int> direction_offsets = {8, 1, -8, -1, 9, -7, -9, 7};


        int bking_pos;

        int wking_pos;
            
        ChessBoard(vector<int> board){
            this->board = board;
            for(int pos = 0; pos < board.size(); pos++){
                if(board[pos] == 1)
                    wking_pos = pos;
                else if(board[pos] == -1)
                    bking_pos = pos;
            }
        }
        
        void getEdgeDistance();

        vector<vector<int>> genMovesForEachPiece(int friendly);

        vector<int> genMoves(int start_pos, int friendly, int enemy);

        vector<int> checkCastle(int start_pos, vector<int> &movelist, int friendly, int enemy);

};

//gets the distance of eacj square from edges in all 8 direcions and stores it in edges vector
void ChessBoard::getEdgeDistance(){
    for(int i = 0; i < 64; i++){
        vector <int> distance;
        
        int row = i / 8;
        int col = i % 8;

        distance.push_back(7 - row);
        distance.push_back(7 - col);
        distance.push_back(row);
        distance.push_back(col);
        
        int ne = (7 - row) < (7 - col) ? (7 - row) : (7 - col);
        int se = (7 - col) < row ? (7 - col) : row;
        int sw = row < col ? row : col;
        int nw = (7 - row) < col ? (7 - row) : col;

        distance.push_back(ne);
        distance.push_back(se);
        distance.push_back(sw);
        distance.push_back(nw);

        edges.push_back(distance);
        
    }
}

//generates moves for each friendly piece, that is, the for the pieces whose turn it is currently
vector<vector<int>> ChessBoard::genMovesForEachPiece(int friendly){
    //declaring enemy colour
    int enemy;
    if(friendly == 1)
        enemy = -1;
    else
        enemy = 1;

    vector<int> movelist = {};
    
    //checking each square on the board for friendly pieces
    for(int i = 0; i < 64; i++){
        movelist = {};
        legalMoves.push_back(vector<int>{});

        //if no piece at this position then skip
        if(board[i] == 0)
            continue;
        
        //we're only interested in the pieces having the colour of the one playing this turn
        else if((friendly == 1 and board[i] > 0) or (friendly == -1 and board[i] < 0)){
            movelist = genMoves(i, friendly, enemy);
        }

        //append list of moves to legalmoves vector
        legalMoves[i] = movelist;
    }

    return legalMoves;
}


//generates moves for a given piece and returns a vector of move positions for that piece
vector<int> ChessBoard::genMoves(int start_pos, int friendly, int enemy){
    int start_dir;
    int end_dir;
    int move_limit = 99;

    bool pawn = false;
    
    vector<int> movelist;

    //checking the piece to determine the direction in which it can move
    //the pieces are mapped as follows: 
    //'k' : 1, 'p' : 2, 'b' : 3, 'n' : 4, 'q' : 5, 'r' : 6, 'r(which can castle)' : 7 
    //the black pieces are negative while white pieces are positive

    switch(abs(board[start_pos])){
        case 1:
            start_dir = 0;
            end_dir = 8;
            checkCastle(start_pos, movelist, friendly, enemy);
            break;
        case 2:
            pawn = true;
            if(board[start_pos] < 0){
                start_dir = 0;
                end_dir = 1;
            }
            else{
                start_dir = 2;
                end_dir = 3;
            }
            break;
        case 3:
            start_dir = 4;
            end_dir = 8;
        case 4:
            break;
        
        case 5:
            start_dir = 0;
            end_dir = 8;
            break;
        case 6:
        
        case 7:
            start_dir = 0;
            end_dir = 4;
            break;
    }


    
    //checking if the current piece is a knight
    if(abs(board[start_pos]) != 4){    

        //iterating for each direction
        for(int direction = start_dir; direction < end_dir; direction++){

            //getting the move offset for current direction 
            
            int offset = direction_offsets[direction];
            
            int pos = start_pos + offset;

            //checking for a pawn in non-starting position or for king
            //move limit is important for pieces like kings and pawns which can only move a 
            //limited number of squares in a particular direction
            
            if(abs(board[start_pos]) == 1 || (board[start_pos] == -2 && start_pos > 15) || (board[start_pos] == 2 && start_pos < 48))
                move_limit = 1;
            
            //checking for pawn in starting position
            else if((board[start_pos] == -2 && start_pos > 7 && start_pos < 16) || (board[start_pos] == 2 && start_pos < 56 && start_pos > 47))
                move_limit = 2;
            
            //iterating until the edge in the current direction is reached
            for(int dir = 0; dir < edges[start_pos][direction] && move_limit > 0; dir++){
                
                //if current position is empty, add it to movelist vector and increase the position by the offset

                if(board[pos] == 0){
                    
                    if((!movelist.empty() and find(movelist.begin(), movelist.end(), pos) == movelist.end()) or movelist.empty())
                        movelist.push_back(pos);
                    pos += offset;
                    move_limit--;
                }

                //if piece is not a pawn and the square has an enemy piece in the direction the piece is moving, capture it
                //pawns capture diagonally so must check for them here
                else if(not pawn and((friendly == 1 and board[pos] < 0) or (friendly == -1 and board[pos] > 0))){
                    movelist.push_back(pos);
                    break;
                }

                //if a friendly piece is encountered, it cannot be captured and hence we break out of the loop
                else if((friendly == 1 and board[pos] > 0) or (friendly == -1 and board[pos] < 0))
                    break;
                
            }

            //addtional pawn moves like enpassant or attacking
            //en passant not yet done
            if(pawn){
                if(friendly == 1){
                    if(edges[start_pos][4] and board[start_pos + 9] < 0)
                        movelist.push_back(start_pos + 9);
                    if(edges[start_pos][7] and board[start_pos + 7] < 0)
                        movelist.push_back(start_pos + 7);
                    
                }
            

                else{
                    if(edges[start_pos][5] and board[start_pos - 7] > 0)
                        movelist.push_back(start_pos - 7);
                    if(edges[start_pos][6] and board[start_pos - 9] > 0)
                        movelist.push_back(start_pos - 9);
                }
            }
        }
    }

    //getting knight moves
    else{
        int row = start_pos / 8;
        int col = start_pos % 8;
    
        if((row + 2 < 8) && (col + 1 < 8) && ((friendly == 1 && board[(row + 2) * 8 + col + 1] <= 0) || friendly == -1 && board[(row + 2) * 8 + col + 1] >= 0))
            movelist.push_back((row + 2) * 8 + col + 1);
        
        if((row + 2 < 8) && (col - 1 >= 0) && ((friendly == 1 && board[(row + 2) * 8 + col - 1] <= 0) || friendly == -1 && board[(row + 2) * 8 + col - 1] >= 0))
            movelist.push_back((row + 2) * 8 + col - 1);
        
        if((row - 2 >= 0) && (col + 1 < 8) && ((friendly == 1 && board[(row - 2) * 8 + col + 1] <= 0) || friendly == -1 && board[(row - 2) * 8 + col + 1] >= 0))
            movelist.push_back((row - 2) * 8 + col + 1);
        
        if((row - 2 >= 0) && (col - 1 >= 0) && ((friendly == 1 && board[(row - 2) * 8 + col - 1] <= 0) || friendly == -1 && board[(row - 2) * 8 + col - 1] >= 0))
            movelist.push_back((row - 2) * 8 + col - 1);
        
        if((row + 1 < 8) && (col + 2 < 8) && ((friendly == 1 && board[(row + 1) * 8 + col + 2] <= 0) || friendly == -1 && board[(row + 1) * 8 + col + 2] >= 0))
            movelist.push_back((row + 1) * 8 + col + 2);
        
        if((row - 1 >=0) && (col + 2 < 8) && ((friendly == 1 && board[(row - 1) * 8 + col + 2] <= 0) || friendly == -1 && board[(row - 1) * 8 + col + 2] >= 0))
            movelist.push_back((row - 1) * 8 + col + 2);
        
        if((row + 1 < 8) && (col - 2 >= 0) && ((friendly == 1 && board[(row + 1) * 8 + col - 2] <= 0) || friendly == -1 && board[(row + 1) * 8 + col - 2] >= 0))
            movelist.push_back((row + 1) * 8 + col - 2);
        
        if((row - 1 >= 0) && (col - 2 >= 0) && ((friendly == 1 && board[(row - 1) * 8 + col - 2] <= 0) || friendly == -1 && board[(row - 1) * 8 + col - 2] >= 0))
            movelist.push_back((row - 1) * 8 + col - 2);
        
    }
    

    return movelist;
}


vector<int> ChessBoard::checkCastle(int start_pos, vector<int> &movelist, int friendly, int enemy){
    //checking if castling is possible
    int low_lim , up_lim;
    if(friendly == 1){
        if((start_pos != 60) or ((board[63] != 7) and board[56] != 7))
            return movelist;
        else{
            low_lim = 56;
            up_lim = 63;
        }
    }
    else{ 
        if((start_pos != 4) or ((board[0] != -7) and board[7] != -7))
            return movelist;
        else{
            low_lim = 0;
            up_lim = 7;
        }
    
    }

    int pos = start_pos + 1;
    while(pos < up_lim and board[pos] == 0){
        //check if there is a check at this position
        pos++;
        
    }
    
    if(pos == up_lim){
        movelist.push_back(start_pos + 1);
        movelist.push_back(start_pos + 2);
    }

    pos = start_pos - 1;

    while(pos > low_lim and board[pos] == 0){
        //check if there is a check at this position
        pos--;
    }
    
    if(pos == low_lim){
        movelist.push_back(start_pos - 1);
        movelist.push_back(start_pos - 2);
    }

    return movelist;
}

int main(){
    
    /*for(int i = 0; i < 8; i++){
        vector<int> temp;
        for(int j = 0; j < 8; j++){
            if(i * 8 + j == 12)
                temp.push_back(2);
            //else if(i * 8 + j == 20)
                //temp.push_back(-1);
            else if(i * 8 + j == 19)
                temp.push_back(-1);
            else if(i * 8 + j == 21)
                temp.push_back(-1);
            else
                temp.push_back(0);
        }
        oldboard.push_back(temp);
    }*/

    vector<int> board = {-7, 0, 0, 0, -1, -3, -4, -6, -2, -2, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 2, 2, 2, 6, 4, 3, 5, 1, 3, 0, 6};

    for(int i = 0; i < 64; i++){
        
        if(i == 26)
            board[i] = -2;
        if(i == 36)
            board[i] = 2;
        if(i == 45)
            board[i] = 4;
        if(i == 51)
            board[i] = 0;

        if(i % 8 == 0)
            cout<<endl;
        
        cout<<setw(3)<<board[i];
    }


    cout<<endl<<endl;
    ChessBoard b(board);

    b.getEdgeDistance();
    b.genMovesForEachPiece(-1);

    for(int i = 0; i < 64; i++){
        //cout<<board[i]<<" ";
            if(b.legalMoves[i].size()){
                cout<<"legal moves for piece "<<b.board[i]<<": ";
                for(int j = 0; j < b.legalMoves[i].size(); j++){
                    //cout<<b.legalMoves[i][j]<<" ";
                    int row = b.legalMoves[i][j] / 8;
                    int col = b.legalMoves[i][j] % 8;
                    cout<<"("<<row<<", "<<col<<") ";
                }
                cout<<endl;
            }
    }
    
}