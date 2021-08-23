
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>
#include<iomanip>

using namespace std;

//S, E, N, W, SE, NE, NW, SW

#define S 0
#define E 1
#define N 2
#define W 3
#define SE 4
#define NE 5
#define NW 6
#define SW 7



class ChessBoard{
    public:

        vector<int> board;

        vector<vector<int>> legalMoves;

        const vector<int> directions = {0, 1, 2, 3, 4, 5, 6, 7};

        vector<vector<int>> edges;

        const vector<int> direction_offsets = {8, 1, -8, -1, 9, -7, -9, 7};
        int En_pessant_pos;

        int bking_pos;

        int wking_pos;
        int check_piece_pos;
            
        ChessBoard(vector<int> board){
            this->board = board;
            En_pessant_pos=64;
            for(int pos = 0; pos < board.size(); pos++){
                if(board[pos] == 1)
                    wking_pos = pos;
                else if(board[pos] == -1)
                    bking_pos = pos;
                if(abs(board[pos]) == 9)
                    En_pessant_pos=pos;
            }
        }
        
        void getEdgeDistance();

        vector<vector<int>> genMovesForEachPiece(int friendly);

        void genMoves(int start_pos, int friendly, int checks);

        void checkCastle(int start_pos, vector<int> &move, int friendly);

        int IsCheck(int friendly);

        void ChangeBoard(int start,int end);

        void makeLegal(int friendly, int checks, int kingpos);

        int getPieceDirection(int kingpos, int piece_pos = 64);

        
};


void ChessBoard::ChangeBoard(int start, int end)
{
    if(board[start] == 1)
        wking_pos = end;
    if(board[start] == -1)
        bking_pos = end;
    
    int friendly = 0;
    if (board[start] > 0)
        friendly = 1;
    if (board[start] < 0)
        friendly = -1;
    if (friendly == 0)
        return;
    if (En_pessant_pos != 64)
    {
        if (abs(board[start]) == 2 && abs(board[end]) == 9)
        {
            board[start] = 0;
            board[end] = 2 * friendly;
            board[end + 8 * friendly] = 0;
            En_pessant_pos = 64;
            return;
        }
    }
    if(En_pessant_pos!=64)
    {
        board[En_pessant_pos]=0;
    }
    En_pessant_pos = 64;
    if (abs(board[start]) == 2 && abs(end - start) == 16)
    {
        board[start] = 0;
        board[end] = 2 * friendly;
        board[(end + start) / 2] = 9 * (friendly);
        En_pessant_pos=(end+start)/2;
        return;
    }
    if (abs(board[start]) == 1 && abs(end - start) == 2)
    {
        if (board[start] == 1 && (end - start) > 0)
        {
            board[start] = 0;
            board[end] = 1;
            board[63] = 0;
            board[(start + end) / 2] = 6;
            if(board[56]==7)
            {
                board[56]=6;
            }
            return;
        }
        if (board[start] == 1 && (end - start) < 0)
        {
            board[start] = 0;
            board[end] = 1;
            board[56] = 0;
            board[(start + end) / 2] = 6;
            if(board[63]==7)
            {
                board[63]=6;
            }
            return;
        }
        if (board[start] == -1 && (end - start) > 0)
        {
            board[start] = 0;
            board[end] = -1;
            board[7] = 0;
           
            board[(start + end) / 2] = -6;
            if(board[0]==-7)
            {
                board[0]=-6;
            }
            return;
        }
        if (board[start] == -1 && (end - start) < 0)
        {
            board[start] = 0;
            board[end] = -1;
            board[0] = 0;
            board[(start + end) / 2] = -6;
            if(board[7]==-7)
            {
                board[7]=-6;
            }
            return;
        }
    }
    if(board[start]==-1)
    {
        if(board[0]==-7)
        {
            board[0]=-6;
        }
        if(board[7]==-7)
        {
            board[7]=-6;
        }
    }
    if(board[start]==1)
    {
        if(board[63]==7)
        {
            board[63]=6;
        }
        if(board[56]==7)
        {
            board[56]=6;
        }
    }
    if(abs(board[start])==7)
    {
        board[end] = board[start]-1*friendly;
        board[start] = 0;
        return;  
    }
    board[end] = board[start];
    board[start] = 0;
    return;
}


int ChessBoard::IsCheck(int friendly)
{

    int kingpos;
    int check = 0;
    if (friendly == 1)
    {
        kingpos = wking_pos;
    }
    else if (friendly == -1)
    {
        kingpos = bking_pos;
    }
    int i = 0, j = 0;
    int newpos = kingpos;
    //rook and queen check
    for (i = 0; i < 4; i++)
    {
        newpos = kingpos;

        for (j = 1; j <= edges[kingpos][i]; j++)
        {

            switch (i)
            {
            case 0:
                newpos = newpos + 8;

                break;
            case 1:
                newpos = newpos + 1;
                break;
            case 2:
                newpos = newpos - 8;
                break;
            case 3:
                newpos = newpos - 1;
                break;

            }
            if (board[newpos] * friendly > 0 && board[newpos]*friendly!=9)
            {
                break;
            }
            if (board[newpos] == -4 * friendly || board[newpos] == -2 * friendly || board[newpos] == -3 * friendly || board[newpos] == -1 * friendly)
            {
                break;
            }
            if (board[newpos] == -friendly * 6 || board[newpos] == -friendly * 7 || board[newpos] == -friendly * 5)
            {
                check++;
                check_piece_pos = newpos;
                break;
            }

        }
    }
    bool pawncheck = false;
    //bishop, queen and pawn check
    for (i = 4; i < 8; i++)
    {
        newpos = kingpos;
        for (j = 1; j <= edges[kingpos][i]; j++)
        {
            pawncheck = false;
            switch (i)
            {
            case 4:
                newpos = newpos + 8 + 1;
                if (friendly == -1 && j == 1)
                {
                    pawncheck = true;
                }
                break;
            case 5:
                newpos = newpos + 1 - 8;
                if (friendly == 1 && j == 1)
                {
                    pawncheck = true;
                }
                break;
            case 6:
                newpos = newpos - 8 - 1;
                if (friendly == 1 && j == 1)
                {
                    pawncheck = true;
                }
                break;
            case 7:
                newpos = newpos - 1 + 8;
                if (friendly == -1 && j == 1)
                {
                    pawncheck = true;
                }
                break;

            }
            if (board[newpos] * friendly > 0 && board[newpos]*friendly!=9)
            {
                break;
            }
            if (board[newpos] == -friendly * 6 || board[newpos] == -friendly * 7||board[newpos]==-friendly*4)
            {
                break;
            }
            if (board[newpos] == -friendly * 5 || board[newpos] == -friendly * 3)
            {
                check++;
                check_piece_pos = newpos;
                break;
            }
            if (pawncheck)
            {
                if (board[newpos] == -friendly * 2)
                {
                    check++;
                    check_piece_pos = newpos;
                    break;
                }
            }
            if (!pawncheck)
            {
                if (board[newpos] == -friendly * 2)
                {
                    break;
                }
            }

        }
    }
    //knight check
    int dirnY[4] = { -2,-1,1,2 };
    int dirnX[2];

    for (i = 0; i < 4; i++)
    {
        int newpos = kingpos + dirnY[i] * 8;
        if (newpos >= 64 || newpos < 0)
            continue;
        int row1 = newpos / 8;
        if (abs(dirnY[i]) == 2)
        {
            dirnX[0] = -1;
            dirnX[1] = 1;
        }
        if (abs(dirnY[i]) == 1)
        {
            dirnX[0] = -2;
            dirnX[1] = 2;
        }
        for (j = 0; j < 2; j++)
        {
            int npos = newpos + dirnX[j];
            int row2 = npos / 8;
            if (row1 != row2)
                continue;
            if (board[npos] == -friendly * 4)
            {
                check++;
                check_piece_pos = npos;

            }
        }

    }
    return check;
}

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
        
        int ne_dist = (7 - row) < (7 - col) ? (7 - row) : (7 - col);
        int se_dist = (7 - col) < row ? (7 - col) : row;
        int sw_dist = row < col ? row : col;
        int nw_dist = (7 - row) < col ? (7 - row) : col;

        distance.push_back(ne_dist);
        distance.push_back(se_dist);
        distance.push_back(sw_dist);
        distance.push_back(nw_dist);

        edges.push_back(distance);
        
    }
}

//generates moves for each friendly piece, that is, the for the pieces whose turn it is currently
vector<vector<int>> ChessBoard::genMovesForEachPiece(int friendly){
    legalMoves.clear();
    
    int checks = IsCheck(friendly);
    //cout<<"checks = "<<checks<<endl;

    int kingpos;
    if(friendly == 1)
        kingpos = wking_pos;
    else
        kingpos = bking_pos;

    if(checks > 1){
        genMoves(kingpos, friendly, checks);
    }

    else{    
        //checking each square on the board for friendly pieces
        for(int i = 0; i < 64; i++){
            //if no piece at this position then skip
            if(board[i] == 0 || abs(board[i]) == 9)
                continue;
            
            //we're only interested in the pieces having the colour of the one playing this turn
            else if((friendly == 1 && board[i] > 0) || (friendly == -1 && board[i] < 0)){
                genMoves(i, friendly, checks);
            }

            //append list of moves to legalmoves vector
        }
    }

    makeLegal(friendly, checks, kingpos);

    return legalMoves;
}


void ChessBoard::makeLegal(int friendly, int checks, int kingpos){
    
    auto legalMoves_it = legalMoves.begin();

    if(checks == 1){
        int check_dir = getPieceDirection(kingpos);
        
        if(check_dir < 0){
            

            while(legalMoves_it != legalMoves.end()){
                auto move_it = legalMoves_it->begin();

                if(*move_it != kingpos && *(move_it + 1) != check_piece_pos){
                    legalMoves.erase(legalMoves_it);
                }

                else
                    legalMoves_it++;
            }
        }
            

        else{    
            int num_squares = abs((kingpos - check_piece_pos) / direction_offsets[check_dir]);

            vector<int> legal_squares;
            
            int new_pos = kingpos + direction_offsets[check_dir];
            
            while(num_squares > 0){
                legal_squares.push_back(new_pos);
                num_squares--;
                new_pos += direction_offsets[check_dir];
            }

            

            while(legalMoves_it != legalMoves.end()){

                auto move_it = legalMoves_it->begin();

                if(*move_it == kingpos){
                    legalMoves_it++;
                    continue;
                }

                auto pos = find(legal_squares.begin(), legal_squares.end(), *(move_it + 1));

                if(pos == legal_squares.end()){

                    if(*move_it != friendly * 2 || *(move_it + 1) != (check_piece_pos - (8 * friendly))){
                        legalMoves.erase(legalMoves_it);
                        continue;
                    }
                }
                
                legalMoves_it++;
            }
            
        }
    }
    
    if(checks == 0 || checks == 1){

        legalMoves_it = legalMoves.begin();
        
        while(legalMoves_it != legalMoves.end()){
            
            
            int piece_pos = *(legalMoves_it->begin());

            //checking if the piece being considered is not the king
            if(piece_pos == kingpos){
                legalMoves_it++;
                continue;
            }

            int piece_dir = getPieceDirection(kingpos, piece_pos);

            if(piece_dir == -1){
                legalMoves_it++;
                continue;
            }
            

            int new_pos = piece_pos;

            vector<int> legal_squares;
            bool confine_direction = false;
            
            for(int start = 0; start < edges[piece_pos][piece_dir]; start++){
                new_pos += direction_offsets[piece_dir];
                if((friendly == 1 && board[new_pos] > 0) || (friendly == -1 && board[new_pos] < 0))
                    break;

                else if(piece_dir < 4 && (board[new_pos] == -friendly * 5 || board[new_pos] == -friendly * 6 || board[new_pos] == -friendly * 7)){
                    confine_direction = true;
                    break;
                }

                else if(piece_dir >= 4 && (board[new_pos] == -friendly * 5 || board[new_pos] == -friendly * 3)){
                    confine_direction = true;
                    break;
                }

                else if(board[new_pos] != 0)
                    break;
            }
            

            if(confine_direction == true){

                int opp_dir;
                if(piece_dir < 2 || (piece_dir > 4 && piece_dir < 6))
                    opp_dir = piece_dir + 2;
                
                else
                    opp_dir = piece_dir - 2;
                
                int enemy_pos = new_pos;
                
                new_pos = piece_pos;
                
                for(int start = 0; start < edges[piece_pos][opp_dir]; start++){
                    new_pos += direction_offsets[opp_dir];

                    if(board[new_pos] != 0 && new_pos != kingpos){
                        confine_direction = false;
                        break;
                    }
                }

                if(confine_direction == false){
                    legalMoves_it++;
                    break;
                }

                for(int pos = kingpos + direction_offsets[piece_dir]; pos != enemy_pos; pos += direction_offsets[piece_dir]){
                    legal_squares.push_back(pos);
                }

                legal_squares.push_back(enemy_pos);

                while(legalMoves_it != legalMoves.end() && *(legalMoves_it->begin()) == piece_pos){
                    auto move_it = legalMoves_it->begin() + 1;
                    auto pos = find(legal_squares.begin(), legal_squares.end(), *move_it);
                    if(pos == legal_squares.end()){
                        legalMoves.erase(legalMoves_it);
                    }
                    else{
                        legalMoves_it++;
                    }
                }
            }

            else
                legalMoves_it++;
                        
        }
    }

    

    else{
        
        while(legalMoves_it != legalMoves.end()){
            auto move_it = legalMoves_it->begin();
            
            if(*move_it != kingpos){
                legalMoves.erase(legalMoves_it);
                continue;
            }

            legalMoves_it++;
        }
    }

    //legalizing king moves

    
    legalMoves_it = legalMoves.begin();
    bool kmoves_found = false;
    while(legalMoves_it != legalMoves.end()){
        bool erased = false;
        auto move_it = legalMoves_it->begin();
        
        if(*move_it != kingpos && kmoves_found == false){
            legalMoves_it++;
            continue;
        }

        else if(*move_it != kingpos && kmoves_found == true)
            break;

        else{
            kmoves_found = true;
            int old_kpos = kingpos;
            int new_kpos = *(move_it + 1);
            
            if(friendly == 1)
                wking_pos = new_kpos;
            else
                bking_pos = new_kpos;
            
            int temp_check = IsCheck(friendly);

            if(temp_check > 0){

                if(new_kpos - old_kpos == 1 && checks == 0){
                    vector<int> temp = {kingpos, new_kpos + 1};
                    auto castling_pos = find(legalMoves.begin(), legalMoves.end(), temp);

                    if(castling_pos != legalMoves.end()){
                        legalMoves.erase(castling_pos);
                        legalMoves_it--;
                    }
                }

                else if(new_kpos - old_kpos == -1 && checks == 0){
                    vector<int> temp = {kingpos, new_kpos - 1};
                    auto castling_pos = find(legalMoves.begin(), legalMoves.end(), temp);

                    if(castling_pos != legalMoves.end()){
                        legalMoves.erase(castling_pos);
                        legalMoves_it--;
                    }
                }


                legalMoves.erase(legalMoves_it);
                erased = true;
            }

            if(friendly == 1)
                wking_pos = old_kpos;
            else
                bking_pos = old_kpos;

            if(erased == false)
                legalMoves_it++;        
        }
    }
}

int ChessBoard::getPieceDirection(int kingpos, int piece_pos){

    if(piece_pos == 64)
        piece_pos = check_piece_pos;
    
    if(abs(board[piece_pos]) == 4)
        return -1;
    
    int diff = kingpos - piece_pos;
    if(diff < 0){
        if(diff % 9 == 0){
            return SE;
        }
        else if(diff % 8 == 0){
            return S;
        }

        else if(diff % 7 == 0){
            return SW;
        }

        else if(kingpos / 8 == piece_pos / 8)
            return E;
        
        else
            return -1;
    }

    else{
        if(diff % 9 == 0){
            return NW;
        }
        else if(diff % 8 == 0){
            return N;
        }

        else if(diff % 7 == 0){
            return NE;
        }

        else if(kingpos / 8 == piece_pos / 8)
            return W;

        else 
            return -1;
    }
}

//generates moves for a given piece and returns a vector of move positions for that piece
void ChessBoard::genMoves(int start_pos, int friendly, int checks){
    int start_dir;
    int end_dir;
    int move_limit = 99;

    bool pawn = false;
    
    vector<int> move;
    move.push_back(start_pos);

    //checking the piece to determine the direction in which it can move
    //the pieces are mapped as follows: 
    //'k' : 1, 'p' : 2, 'b' : 3, 'n' : 4, 'q' : 5, 'r' : 6, 'r(which can castle)' : 7 
    //the black pieces are negative while white pieces are positive

    switch(abs(board[start_pos])){
        case 1:
            start_dir = 0;
            end_dir = 8;
            if(checks == 0)
                checkCastle(start_pos, move, friendly);
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
                
                move.push_back(pos);
                
                //if current position is empty, add it to move vector and increase the position by the offset
                //9 and -9 are en passant squares. They do not have an actual piece on them
                if(board[pos] == 0 || abs(board[pos]) == 9){
                    
                    //if((!legalMoves.empty() && find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end()) || legalMoves.empty()){
                        
                    //}
                    legalMoves.push_back(move);
                    

                    pos += offset;
                    move_limit--;
                }

                //if piece is not a pawn and the square has an enemy piece in the direction the piece is moving, capture it
                //pawns capture diagonally so must check for them here
                else if(pawn == false && ((friendly == 1 && board[pos] < 0) || (friendly == -1 && board[pos] > 0))){
                    legalMoves.push_back(move);
                    move.pop_back();
                    break;
                }

                //if a friendly piece is encountered, it cannot be captured and hence we break out of the loop
                else if((friendly == 1 && board[pos] > 0) || (friendly == -1 && board[pos] < 0)){
                    move.pop_back();
                    break;
                }

                move.pop_back();
                
            }

            //addtional pawn moves like enpassant or attacking
            //en passant not yet done
            if(pawn){
                if(friendly == 1){
                    if(edges[start_pos][5] && board[start_pos - 7] < 0){
                        move.push_back(start_pos - 7);
                        legalMoves.push_back(move);
                        move.pop_back();
                    }
                    if(edges[start_pos][6] && board[start_pos - 9] < 0){
                        move.push_back(start_pos - 9);
                        legalMoves.push_back(move);
                        move.pop_back();
                    }
                    
                }
            
                else{
                    if(edges[start_pos][4] && board[start_pos + 9] > 0){
                        move.push_back(start_pos + 9);
                        legalMoves.push_back(move);
                        move.pop_back();
                    }
                    if(edges[start_pos][7] && board[start_pos + 7] > 0){
                        move.push_back(start_pos + 7);
                        legalMoves.push_back(move);
                        move.pop_back();
                    }
                }
            }
        }
    }

    //getting knight moves
    else{
        int row = start_pos / 8;
        int col = start_pos % 8;

        if((row + 2 < 8) && (col + 1 < 8) && ((friendly == 1 && board[(row + 2) * 8 + col + 1] <= 0) || friendly == -1 && board[(row + 2) * 8 + col + 1] >= 0))
        {
            move.push_back((row + 2) * 8 + col + 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row + 2 < 8) && (col - 1 >= 0) && ((friendly == 1 && board[(row + 2) * 8 + col - 1] <= 0) || friendly == -1 && board[(row + 2) * 8 + col - 1] >= 0))
        {
            move.push_back((row + 2) * 8 + col - 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 2 >= 0) && (col + 1 < 8) && ((friendly == 1 && board[(row - 2) * 8 + col + 1] <= 0) || friendly == -1 && board[(row - 2) * 8 + col + 1] >= 0))
        {
            move.push_back((row - 2) * 8 + col + 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 2 >= 0) && (col - 1 >= 0) && ((friendly == 1 && board[(row - 2) * 8 + col - 1] <= 0) || friendly == -1 && board[(row - 2) * 8 + col - 1] >= 0))
        {
            move.push_back((row - 2) * 8 + col - 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row + 1 < 8) && (col + 2 < 8) && ((friendly == 1 && board[(row + 1) * 8 + col + 2] <= 0) || friendly == -1 && board[(row + 1) * 8 + col + 2] >= 0))
        {
            move.push_back((row + 1) * 8 + col + 2);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 1 >=0) && (col + 2 < 8) && ((friendly == 1 && board[(row - 1) * 8 + col + 2] <= 0) || friendly == -1 && board[(row - 1) * 8 + col + 2] >= 0))
        {
            move.push_back((row - 1) * 8 + col + 2);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row + 1 < 8) && (col - 2 >= 0) && ((friendly == 1 && board[(row + 1) * 8 + col - 2] <= 0) || friendly == -1 && board[(row + 1) * 8 + col - 2] >= 0))
        {
            move.push_back((row + 1) * 8 + col - 2);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 1 >= 0) && (col - 2 >= 0) && ((friendly == 1 && board[(row - 1) * 8 + col - 2] <= 0) || friendly == -1 && board[(row - 1) * 8 + col - 2] >= 0))
        {
            move.push_back((row - 1) * 8 + col - 2);
            legalMoves.push_back(move);
            move.pop_back();
        }
    }
    

    
}


void ChessBoard::checkCastle(int start_pos, vector<int> &move, int friendly){
    //checking if castling is possible
    int low_lim, up_lim;
    if(friendly == 1){
        if((start_pos != 60) || ((board[63] != 7) && board[56] != 7))
            return;
        else{
            low_lim = 56;
            up_lim = 63;
        }
    }
    else{ 
        if((start_pos != 4) || ((board[0] != -7) && board[7] != -7))
            return;
        else{
            low_lim = 0;
            up_lim = 7;
        }
    
    }

    int pos = start_pos + 1;
    while(pos < up_lim && board[pos] == 0){
        //check if there is a check at this position
        pos++;
        
    }
    
    if(pos == up_lim && abs(board[pos]) == 7){
        //move.push_back(start_pos + 1);
        //legalMoves.push_back(move);
        //move.pop_back();

        move.push_back(start_pos + 2);
        legalMoves.push_back(move);
        move.pop_back();
    }

    pos = start_pos - 1;

    while(pos > low_lim && board[pos] == 0){
        //check if there is a check at this position
        pos--;
    }
    
    if(pos == low_lim && abs(board[pos]) == 7){
        //move.push_back(start_pos - 1);
        //legalMoves.push_back(move);
        //move.pop_back();

        move.push_back(start_pos - 2);
        legalMoves.push_back(move);
        move.pop_back();
    }
}


int main(){

    vector<int> board = {-7, 0, 0, 0, -1, -3, -4, -6, -2, -2, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 6, 4, 3, 5, 1, 3, 0, 7};

    for(int i = 0; i < 64; i++){
        
        if(i == 26)
            board[i] = -2;
        //else if(i == 36 || i == 27 || i == 31)
        //    board[i] = 2;
        else if(i == 27)
            board[i] = 2;
        else if(i == 24)
            board[i] = -5;
        //else if(i == 51)
        //    board[i] = -5;
        //else if(i == 45)
        //    board[i] = -4;
        //else if(i == 44)
        //   board[i] = -5;
        //else if(i == 51)
        //    board[i] = 3;
        else if(i == 18)
            board[i] = -9;
        //else if(i == 23)
        //    board[i] = 6;
        //else if(i == 33)
        //    board[i] = -4;

        if(i % 8 == 0)
            cout<<endl;
        
        cout<<setw(3)<<board[i];
    }


    cout<<endl<<endl;
    ChessBoard b(board);

    b.getEdgeDistance();
    b.genMovesForEachPiece(1);

    cout<<"possible moves: "<<endl;
    for(int i = 0; i < b.legalMoves.size(); i++){
        cout<<b.legalMoves[i][0]<<", "<<b.legalMoves[i][1]<<endl;
    }
    
}
