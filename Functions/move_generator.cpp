#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>
#include<iomanip>
#include<chrono>
#include<random>
using namespace std;
long countmoves;
//defining direction macros

#define S 0
#define E 1
#define N 2
#define W 3
#define SE 4
#define NE 5
#define NW 6
#define SW 7

//defining piece macros

#define KING 1
#define PAWN 2
#define BISHOP 3
#define KNIGHT 4
#define QUEEN 5
#define ROOK 6
#define ROOK_CASTLE 7
#define EN_PASSANT_SQ 9
#define EMPTY_SQ 0

//defining colour macros
#define WHITE 1
#define BLACK -1



class ChessBoard{
    
    public:
    class node
    {
        public:
        uint64_t HashVal;
        float Eval;
        int depth;
        int move_num;
        node * next;
    };
        int move_num;
        node* nd_arr[2]; 
        uint64_t random_num[64][16];
        uint64_t rand;
        static const unsigned long c=65536;
        
        node** nd = new node*[c];
        vector<int> board;

        vector<vector<int>> legalMoves;

        const vector<int> directions = {0, 1, 2, 3, 4, 5, 6, 7};
        const float map_pawnW[64]={
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0.1     ,0.1    ,0.15   ,0.2    ,0.2    ,0.15   ,0.1    ,0.1    ,
            0.12    ,0.12   ,0.2    ,0.3    ,0.3    ,0.2    ,0.12   ,0.12   ,
            0.20    ,0.22   ,0.25   ,0.35   ,0.35   ,0.25   ,0.22   ,0.20   ,
            0.30    ,0.32   ,0.35   ,0.45   ,0.45   ,0.35   ,0.32   ,0.30   ,
            0.60    ,0.62   ,0.65   ,0.75   ,0.75   ,0.65   ,0.62   ,0.60   ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
        };
        const float map_pawnB[64]={
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0.60    ,0.62   ,0.65   ,0.75   ,0.75   ,0.65   ,0.62   ,0.60   ,
            0.30    ,0.32   ,0.35   ,0.45   ,0.45   ,0.35   ,0.32   ,0.30   ,
            0.20    ,0.22   ,0.25   ,0.35   ,0.35   ,0.25   ,0.22   ,0.20   ,
            0.12    ,0.12   ,0.2    ,0.3    ,0.3    ,0.2    ,0.12   ,0.12   ,
            0.1     ,0.1    ,0.15   ,0.2    ,0.2    ,0.15   ,0.1    ,0.1    ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
        };
        const float map_rook_mid_W[64]={
            0       ,0      ,0.2    ,0.2    ,0.2    ,0.2    ,0      ,0      ,
            0       ,0      ,0.1    ,0.1    ,0.1    ,0.1    ,0      ,0      ,
            0       ,0      ,0.1    ,0.1    ,0.1    ,0.1    ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0.5     ,0.5    ,0.5    ,0.5    ,0.5    ,0.5    ,0.5    ,0.5    ,
            0.3     ,0.3    ,0.3    ,0.3    ,0.3    ,0.3    ,0.3    ,0.3    ,
        };
        const float map_rook_mid_B[64]={
            0.3     ,0.3    ,0.3    ,0.3    ,0.3    ,0.3    ,0.3    ,0.3    ,
            0.5     ,0.5    ,0.5    ,0.5    ,0.5    ,0.5    ,0.5    ,0.5    ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0.1    ,0.1    ,0.1    ,0.1    ,0      ,0      ,
            0       ,0      ,0.1    ,0.1    ,0.1    ,0.1    ,0      ,0      ,
            0       ,0      ,0.2    ,0.2    ,0.2    ,0.2    ,0      ,0      ,
        };
        const float map_minor_mid_W[64]={
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0.2    ,0      ,0      ,0      ,0      ,0.2    ,0      ,
            0       ,0.1    ,0.15   ,0.1    ,0.1    ,0.15   ,0.1    ,0      ,
            0       ,0.2    ,0.25   ,0.25   ,0.25   ,0.25   ,0.2    ,0      ,
            0       ,0.2    ,0.25   ,0.25   ,0.25   ,0.25   ,0.2    ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
        };
        const float map_minor_mid_B[64]={
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
            0       ,0.2    ,0.25   ,0.25   ,0.25   ,0.25   ,0.2    ,0      ,
            0       ,0.2    ,0.25   ,0.25   ,0.25   ,0.25   ,0.2    ,0      ,
            0       ,0.1    ,0.15   ,0.1    ,0.1    ,0.15   ,0.1    ,0      ,
            0       ,0.2    ,0      ,0      ,0      ,0      ,0.2    ,0      ,
            0       ,0      ,0      ,0      ,0      ,0      ,0      ,0      ,
        };
        vector<vector<int>> edges;

        const vector<int> direction_offsets = {8, 1, -8, -1, 9, -7, -9, 7};

        int En_pessant_pos;

        int bking_pos;

        int wking_pos;
        int check_piece_pos;

        int move_count;
        int half_move;
        
        ChessBoard(){};

        ChessBoard(vector<int> board,int half){
            move_num=10;
            half_move=half;
            this->board = board;
            En_pessant_pos=64;
            for(long s=0;s<c;s++)
            {
                nd[s]=NULL;
            }
            for(int pos = 0; pos < board.size(); pos++){
                /*if(board[pos] > 0 && abs(board[pos]) != EN_PASSANT_SQ){
                    whitepieces.push_back(pos);
                }

                else if(board[pos] < 0 && abs(board[pos]) != EN_PASSANT_SQ){
                    blackpieces.push_back(pos);
                }*/

                if(board[pos] == WHITE * KING)
                    wking_pos = pos;
                else if(board[pos] == BLACK * KING)
                    bking_pos = pos;
                
                if(abs(board[pos]) == EN_PASSANT_SQ)
                    En_pessant_pos=pos;
            }
            mt19937_64 gen (random_device{}());
            for(int i=0;i<64;i++)
            {
                for(int j=0;j<16;j++)
                {
                    
                    random_num[i][j]=gen();
                }
            }
            rand=gen();
        }
        ChessBoard(const ChessBoard &old_board)
        {
            edges=old_board.edges;
            bking_pos=old_board.bking_pos;
            wking_pos=old_board.wking_pos;
            board=old_board.board;
        }
        
        void getEdgeDistance();

        vector<vector<int>> genMovesForEachPiece(int friendly);

        void genMoves(int &start_pos, int &friendly, int &checks);

        void checkCastle(int &start_pos, vector<int> &move, int &friendly);

        int IsCheck(int friendly);

        int ChangeBoard(int start,int end);
        
        void Promote(int sqr,int p,int friendly);
        uint64_t Gen_Zobrist_Key(int plr);
        void makeLegal(int &friendly, int &checks, int &kingpos);
        node* create_node(int evaluation,int alpha,int beta,uint64_t hash,int depth,int move_num);
        void search_node(uint64_t hash);
        int getPieceDirection(int &kingpos, int piece_pos = 64);
        void CountNodes();
        int TestHash();

        vector<vector<int>> getCaptureMoves(int plr);

        
};
void ChessBoard::Promote(int sqr, int p, int friendly)
{
    if (p == 0){
        board[sqr] = friendly * QUEEN;
    }
    if (p == 1){
        board[sqr] = friendly * KNIGHT;
    }
    if (p == 2){
        board[sqr] = friendly * BISHOP;
    }
    if (p == 3){
        board[sqr] = friendly * ROOK;
    }
}

int ChessBoard::ChangeBoard(int start, int end)
{   
    half_move++;
    if(board[end]!=0)
    {
        half_move=0;
    }
    if(abs(board[start])==WHITE*PAWN)
    {
        half_move=0;
    }
    int val=0;
    if(board[start]==WHITE * PAWN && end<=7 && end>=0)
    {
        val=1;
    }
    if(board[start]==BLACK * PAWN && end<=63 && end>=56)
    {
        val=-1;
    }
    if(board[start] == WHITE * KING)
        wking_pos = end;
    if(board[start] == BLACK * KING)
        bking_pos = end;
    
    int friendly = 0;
    if (board[start] > 0)
        friendly = WHITE;
    if (board[start] < 0)
        friendly = BLACK;
    if (friendly == 0)
        return(val);
    
    //updating the piece positions in the piece position storage vectors
    //handling castling and en passant later
    /*if(!(abs(board[start]) == PAWN && abs(board[end]) == EN_PASSANT_SQ) && !(board[start] == friendly * KING && abs(start - end) == 2)){
        if(friendly == WHITE){
            auto piecepos = find(whitepieces.begin(), whitepieces.end(), start);
            *piecepos = end;
            if(board[end] < 0 && abs(board[end]) != EN_PASSANT_SQ){
                piecepos = find(blackpieces.begin(), blackpieces.end(), end);
                blackpieces.erase(piecepos);
            }   
        }

        else{
            auto piecepos = find(blackpieces.begin(), blackpieces.end(), start);
            *piecepos = end;
            if(board[end] > 0 && abs(board[end]) != EN_PASSANT_SQ){
                piecepos = find(whitepieces.begin(), whitepieces.end(), end);
                whitepieces.erase(piecepos);
            }
        }
    }*/

    //changing the actual board now
    if (En_pessant_pos != 64)
    {
        if (abs(board[start]) == PAWN && abs(board[end]) == EN_PASSANT_SQ)
        {
            board[start] = EMPTY_SQ;
            board[end] = PAWN * friendly;
            board[end + 8 * friendly] = EMPTY_SQ;
            En_pessant_pos = 64;

            /*handling en passant piece position update*/
            /*if(friendly == WHITE){
                auto piecepos = find(whitepieces.begin(), whitepieces.end(), start);
                *piecepos = end;
                piecepos = find(blackpieces.begin(), blackpieces.end(), (end + 8 * friendly));
                blackpieces.erase(piecepos);
            }

            else{
                auto piecepos = find(blackpieces.begin(), blackpieces.end(), start);
                *piecepos = end;
                piecepos = find(whitepieces.begin(), whitepieces.end(), (end + 8 * friendly));
                whitepieces.erase(piecepos);
            }*/

            return(val);
        }
    }
    if(En_pessant_pos!=64)
    {
        board[En_pessant_pos]=0;
    }
    En_pessant_pos = 64;
    if (abs(board[start]) == PAWN && abs(end - start) == 16)
    {
        board[start] = EMPTY_SQ;
        board[end] = 2 * friendly;
        board[(end + start) / 2] = EN_PASSANT_SQ * (friendly);
        En_pessant_pos=(end+start)/2;
        return(val);
    }
    if (abs(board[start]) == KING && abs(end - start) == 2)
    {
        if (board[start] == WHITE * KING && (end - start) > 0)
        {
            board[start] = EMPTY_SQ;
            board[end] = WHITE * KING;
            board[63] = EMPTY_SQ;
            board[(start + end) / 2] = WHITE * ROOK;
            if(board[56]==WHITE * ROOK_CASTLE)
            {
                board[56]=WHITE * ROOK;
            }

            //implementing piece position update for 
            //white king side castling
            /*auto piecepos = find(whitepieces.begin(), whitepieces.end(), start);
            *piecepos = end;
            
            piecepos = find(whitepieces.begin(), whitepieces.end(), 63);
            *piecepos = (start + end) / 2;
            */
            return(val);
        }
        if (board[start] == WHITE * KING && (end - start) < 0)
        {
            board[start] = EMPTY_SQ;
            board[end] = WHITE * KING;
            board[56] = EMPTY_SQ;
            board[(start + end) / 2] = WHITE * ROOK;
            if(board[63]==WHITE * ROOK_CASTLE)
            {
                board[63]=WHITE * ROOK;
            }

            //implementing piece position update for 
            //white queen side castling
            /*auto piecepos = find(whitepieces.begin(), whitepieces.end(), start);
            *piecepos = end;
            
            piecepos = find(whitepieces.begin(), whitepieces.end(), 56);
            *piecepos = (start + end) / 2;
            */
            return(val);
        }
        if (board[start] == BLACK * KING && (end - start) > 0)
        {
            board[start] = EMPTY_SQ;
            board[end] = BLACK * KING;
            board[7] = EMPTY_SQ;
           
            board[(start + end) / 2] = BLACK * ROOK;
            if(board[0]==BLACK * ROOK_CASTLE)
            {
                board[0]=BLACK * ROOK;
            }

            //implementing piece position update for 
            //black king side castling
            /*auto piecepos = find(blackpieces.begin(), blackpieces.end(), start);
            *piecepos = end;
            
            piecepos = find(blackpieces.begin(), blackpieces.end(), 7);
            *piecepos = (start + end) / 2;
            */
            return(val);
        }
        if (board[start] == BLACK * KING && (end - start) < 0)
        {
            board[start] = EMPTY_SQ;
            board[end] = BLACK * KING;
            board[0] = EMPTY_SQ;
            board[(start + end) / 2] = BLACK * ROOK;
            if(board[7]==BLACK * ROOK_CASTLE)
            {
                board[7]=BLACK * ROOK;
            }

            //implementing piece position update for 
            //black king side castling
            /*auto piecepos = find(blackpieces.begin(), blackpieces.end(), start);
            *piecepos = end;
            
            piecepos = find(blackpieces.begin(), blackpieces.end(), 0);
            *piecepos = (start + end) / 2;
            */
            return(val);
        }
    }
    if(board[start]==BLACK * KING)
    {
        if(board[0]==BLACK * ROOK_CASTLE)
        {
            board[0]=BLACK * ROOK;
        }
        if(board[7]==BLACK * ROOK_CASTLE)
        {
            board[7]=BLACK * ROOK;
        }
    }
    if(board[start]==WHITE * KING)
    {
        if(board[63]==WHITE * ROOK_CASTLE)
        {
            board[63]=WHITE * ROOK;
        }
        if(board[56]==WHITE * ROOK_CASTLE)
        {
            board[56]=WHITE * ROOK;
        }
    }
    if(abs(board[start])==ROOK_CASTLE)
    {
        board[end] = board[start]-1*friendly;
        board[start] = EMPTY_SQ;
        return(val);  
    }
    board[end] = board[start];
    board[start] = EMPTY_SQ;
    return(val);
}


int ChessBoard::IsCheck(int friendly)
{
    int kingpos;
    int check = 0;
    if (friendly == WHITE)
    {
        kingpos = wking_pos;
    }
    else if (friendly == BLACK)
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
            if (board[newpos] * friendly > 0 && board[newpos]*friendly!=EN_PASSANT_SQ)
            {
                break;
            }
            if (board[newpos] == -KNIGHT * friendly || board[newpos] == -PAWN * friendly || board[newpos] == -BISHOP * friendly || (board[newpos] == -KING * friendly&&j!=1))
            {
                break;
            }
            if(board[newpos]==-friendly*KING && j==1)
            {
                check++;
                check_piece_pos = newpos;
                break;  
            }
            if (board[newpos] == -friendly * ROOK || board[newpos] == -friendly * ROOK_CASTLE || board[newpos] == -friendly * QUEEN)
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
                if (friendly == BLACK && j == 1)
                {
                    pawncheck = true;
                }
                break;
            case 5:
                newpos = newpos + 1 - 8;
                if (friendly == WHITE && j == 1)
                {
                    pawncheck = true;
                }
                break;
            case 6:
                newpos = newpos - 8 - 1;
                if (friendly == WHITE && j == 1)
                {
                    pawncheck = true;
                }
                break;
            case 7:
                newpos = newpos - 1 + 8;
                if (friendly == BLACK && j == 1)
                {
                    pawncheck = true;
                }
                break;

            }
            
            if (board[newpos] * friendly > 0 && board[newpos]*friendly!=EN_PASSANT_SQ)
            {
                break;
            }
            if(board[newpos]==-friendly*KING && j==1)
            {
                check++;
                check_piece_pos = newpos;
                break;  
            }
            if (board[newpos] == -friendly * ROOK || board[newpos] == -friendly * ROOK_CASTLE || board[newpos]==-friendly*KNIGHT || (board[newpos]==-friendly*KING && j!=1))
            {
                break;
            }
            if (board[newpos] == -friendly * QUEEN || board[newpos] == -friendly * BISHOP)
            {
                check++;
                check_piece_pos = newpos;
                break;
            }
            if (pawncheck)
            {
                if (board[newpos] == -friendly * PAWN)
                {
                    check++;
                    check_piece_pos = newpos;
                    break;
                }
            }
            if (!pawncheck)
            {
                if (board[newpos] == -friendly * PAWN)
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
            if (board[npos] == -friendly * KNIGHT)
            {
                check++;
                check_piece_pos = npos;

            }
        }

    }
    return check;
}

//gets the distance of each square from edges in all 8 direcions and stores it in edges vector
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
    move_count = 0;
    
    int checks = IsCheck(friendly);
    //cout<<"checks = "<<checks<<endl;

    int kingpos;
    if(friendly == WHITE)
        kingpos = wking_pos;
    else
        kingpos = bking_pos;

    if(checks > 1){
        genMoves(kingpos, friendly, checks);
    }

    else{
        /*
        if(friendly == WHITE){
            for(int i = 0; i < whitepieces.size(); i++){
                genMoves(whitepieces[i], friendly, checks);
            }
        }

        else{
            for(int i = 0; i < blackpieces.size(); i++){
                genMoves(blackpieces[i], friendly, checks);
            }
        }
        */
        
        //checking each square on the board for friendly pieces
        for(int i = 0; i < 64; i++){
            //if no piece at this position then skip
            if(board[i] == EMPTY_SQ || abs(board[i]) == EN_PASSANT_SQ)
                continue;
            
            //we're only interested in the pieces having the colour of the one playing this turn
            else if((friendly == WHITE && board[i] > 0) || (friendly == BLACK && board[i] < 0)){
                genMoves(i, friendly, checks);
            }

            //append list of moves to legalmoves vector
        }
        
    }

    makeLegal(friendly, checks, kingpos);

    return legalMoves;
}


void ChessBoard::makeLegal(int &friendly, int &checks, int &kingpos){
    
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

                    if(board[*move_it] == friendly * PAWN && board[check_piece_pos] == -friendly * PAWN && *(move_it + 1) == (check_piece_pos - (8 * friendly))){
                        legalMoves_it++;
                        continue;
                    }
                    else{
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

            //checking if the piece being considered is the king
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
                if((friendly == WHITE && board[new_pos] > 0) || (friendly == BLACK && board[new_pos] < 0))
                    break;

                else if(piece_dir < 4 && (board[new_pos] == -friendly * QUEEN || board[new_pos] == -friendly * ROOK || board[new_pos] == -friendly * ROOK_CASTLE)){
                    confine_direction = true;
                    break;
                }

                else if(piece_dir >= 4 && (board[new_pos] == -friendly * QUEEN || board[new_pos] == -friendly * BISHOP)){
                    confine_direction = true;
                    break;
                }

                else if(board[new_pos] != EMPTY_SQ)
                    break;
            }
            

            if(confine_direction == true){

                int opp_dir;
                if(piece_dir < 2 || (piece_dir >= 4 && piece_dir < 6))
                    opp_dir = piece_dir + 2;
                
                else
                    opp_dir = piece_dir - 2;
                
                int enemy_pos = new_pos;
                
                new_pos = piece_pos;
                
                for(int start = 0; start < edges[piece_pos][opp_dir]; start++){
                    new_pos += direction_offsets[opp_dir];

                    if(new_pos == kingpos)
                        break;
                    
                    if(board[new_pos] != EMPTY_SQ){
                        confine_direction = false;
                        break;
                    }
                }

                if(confine_direction == false){
                    legalMoves_it++;
                    continue;
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
    //bool kmoves_found = false;
    while(legalMoves_it != legalMoves.end()){
        bool erased = false;
        auto move_it = legalMoves_it->begin();

        if(board[*move_it] == friendly * PAWN && board[*(move_it + 1)] == -friendly * EN_PASSANT_SQ){
            vector<int> temp_board = board;

            ChessBoard temp(temp_board,half_move);

            temp.edges = edges;

            temp.ChangeBoard(*move_it, *(move_it + 1));

            int temp_checks = temp.IsCheck(friendly);
            
            if(temp_checks > 0){
                legalMoves.erase(legalMoves_it);
                continue;
            }

            else{
                legalMoves_it++;
            }
        }


        /*if(board[*move_it] == friendly * PAWN && ((*(move_it + 1) >= 0 && *(move_it + 1) <= 7) || (*(move_it + 1) >= 56 && *(move_it + 1) <= 63))){
            move_count += 3;
        }*/
        
        else if(*move_it != kingpos /*&& kmoves_found == false*/){
            legalMoves_it++;
            continue;
        }

        //else if(*move_it != kingpos && kmoves_found == true)
        //    break;

        else{
            //kmoves_found = true;
            int old_kpos = kingpos;
            
            int new_kpos = *(move_it + 1);
            
            board[old_kpos] = EMPTY_SQ;

            if(friendly == WHITE)
                wking_pos = new_kpos;
            else
                bking_pos = new_kpos;
            
            int temp_check = IsCheck(friendly);

            board[old_kpos] = friendly * KING;

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

            if(friendly == WHITE)
                wking_pos = old_kpos;
            else
                bking_pos = old_kpos;

            if(erased == false)
                legalMoves_it++;        
        }
    }

    move_count += legalMoves.size();
}

int ChessBoard::getPieceDirection(int &kingpos, int piece_pos){

    bool check = false;

    if(piece_pos == 64){
        piece_pos = check_piece_pos;
        check = true;
    }
    
    if(abs(board[piece_pos]) == KNIGHT && check == true)
        return -1;
    
    int row_kingpos = kingpos / 8;
    int col_kingpos = kingpos % 8;

    int row_piecepos = piece_pos / 8;
    int col_piecepos = piece_pos % 8;

    int diff = kingpos - piece_pos;
    if(diff < 0){
        if(row_kingpos - row_piecepos == col_kingpos - col_piecepos){
            return SE;
        }
        else if(col_kingpos == col_piecepos){
            return S;
        }

        else if(row_kingpos - row_piecepos == -(col_kingpos - col_piecepos)){
            return SW;
        }

        else if(row_kingpos == row_piecepos)
            return E;
        
        else
            return -1;
    }

    else{
        if(row_kingpos - row_piecepos == col_kingpos - col_piecepos){
            return NW;
        }
        else if(col_kingpos == col_piecepos){
            return N;
        }

        else if(row_kingpos - row_piecepos == -(col_kingpos - col_piecepos)){
            return NE;
        }

        else if(row_kingpos == row_piecepos)
            return W;

        else 
            return -1;
    }
}

//generates moves for a given piece and returns a vector of move positions for that piece
void ChessBoard::genMoves(int &start_pos, int &friendly, int &checks){
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
            
            if(abs(board[start_pos]) == KING || (board[start_pos] == BLACK * PAWN && start_pos > 15) || (board[start_pos] == WHITE * PAWN && start_pos < 48))
                move_limit = 1;
            
            //checking for pawn in starting position
            else if((board[start_pos] == BLACK * PAWN && start_pos > 7 && start_pos < 16) || (board[start_pos] == WHITE * PAWN && start_pos < 56 && start_pos > 47))
                move_limit = 2;
            
            //iterating until the edge in the current direction is reached
            for(int dir = 0; dir < edges[start_pos][direction] && move_limit > 0; dir++){
                
                move.push_back(pos);
                
                //if current position is empty, add it to move vector and increase the position by the offset
                //9 and -9 are en passant squares. They do not have an actual piece on them
                if(board[pos] == EMPTY_SQ || abs(board[pos]) == EN_PASSANT_SQ){
                    
                    //if((!legalMoves.empty() && find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end()) || legalMoves.empty()){
                        
                    //}
                    legalMoves.push_back(move);
                    if(pawn == true && ((friendly == WHITE && pos >= 0 && pos <= 7) || (friendly == BLACK && pos >= 56 && pos <= 63))){
                        legalMoves.push_back(move);
                        legalMoves.push_back(move);
                        legalMoves.push_back(move);
                    }

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
                else if((friendly == WHITE && board[pos] > 0) || (friendly == BLACK && board[pos] < 0)){
                    move.pop_back();
                    break;
                }

                move.pop_back();
                
            }

            //addtional pawn moves like enpassant or attacking
            
            if(pawn){
                if(friendly == WHITE){
                    if(edges[start_pos][5] && board[start_pos - 7] < 0){
                        move.push_back(start_pos - 7);
                        legalMoves.push_back(move);

                        if(start_pos - 7 >= 0 && start_pos - 7 <= 7){
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                        }

                        move.pop_back();
                    }
                    if(edges[start_pos][6] && board[start_pos - 9] < 0){
                        move.push_back(start_pos - 9);
                        legalMoves.push_back(move);

                        if(start_pos - 9 >= 0 && start_pos - 9 <= 7){
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                        }

                        move.pop_back();
                    }
                    
                }
            
                else{
                    if(edges[start_pos][4] && board[start_pos + 9] > 0){
                        move.push_back(start_pos + 9);
                        legalMoves.push_back(move);

                        if(start_pos + 9 >= 56 && start_pos + 9 <= 63){
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                        }

                        move.pop_back();
                    }
                    if(edges[start_pos][7] && board[start_pos + 7] > 0){
                        move.push_back(start_pos + 7);
                        legalMoves.push_back(move);

                        if(start_pos + 7 >= 56 && start_pos + 7 <= 63){
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                            legalMoves.push_back(move);
                        }

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

        if((row + 2 < 8) && (col + 1 < 8) && ((friendly == WHITE && board[(row + 2) * 8 + col + 1] <= 0) || friendly == BLACK && board[(row + 2) * 8 + col + 1] >= 0))
        {
            move.push_back((row + 2) * 8 + col + 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row + 2 < 8) && (col - 1 >= 0) && ((friendly == WHITE && board[(row + 2) * 8 + col - 1] <= 0) || friendly == BLACK && board[(row + 2) * 8 + col - 1] >= 0))
        {
            move.push_back((row + 2) * 8 + col - 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 2 >= 0) && (col + 1 < 8) && ((friendly == WHITE && board[(row - 2) * 8 + col + 1] <= 0) || friendly == BLACK && board[(row - 2) * 8 + col + 1] >= 0))
        {
            move.push_back((row - 2) * 8 + col + 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 2 >= 0) && (col - 1 >= 0) && ((friendly == WHITE && board[(row - 2) * 8 + col - 1] <= 0) || friendly == BLACK && board[(row - 2) * 8 + col - 1] >= 0))
        {
            move.push_back((row - 2) * 8 + col - 1);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row + 1 < 8) && (col + 2 < 8) && ((friendly == WHITE && board[(row + 1) * 8 + col + 2] <= 0) || friendly == BLACK && board[(row + 1) * 8 + col + 2] >= 0))
        {
            move.push_back((row + 1) * 8 + col + 2);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 1 >=0) && (col + 2 < 8) && ((friendly == WHITE && board[(row - 1) * 8 + col + 2] <= 0) || friendly == BLACK && board[(row - 1) * 8 + col + 2] >= 0))
        {
            move.push_back((row - 1) * 8 + col + 2);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row + 1 < 8) && (col - 2 >= 0) && ((friendly == WHITE && board[(row + 1) * 8 + col - 2] <= 0) || friendly == BLACK && board[(row + 1) * 8 + col - 2] >= 0))
        {
            move.push_back((row + 1) * 8 + col - 2);
            legalMoves.push_back(move);
            move.pop_back();
        }

        if((row - 1 >= 0) && (col - 2 >= 0) && ((friendly == WHITE && board[(row - 1) * 8 + col - 2] <= 0) || friendly == BLACK && board[(row - 1) * 8 + col - 2] >= 0))
        {
            move.push_back((row - 1) * 8 + col - 2);
            legalMoves.push_back(move);
            move.pop_back();
        }
    }
    

    
}


void ChessBoard::checkCastle(int &start_pos, vector<int> &move, int &friendly){
    //checking if castling is possible
    int low_lim, up_lim;
    if(friendly == WHITE){
        if((start_pos != 60) || ((board[63] != WHITE * ROOK_CASTLE) && board[56] != WHITE * ROOK_CASTLE))
            return;
        else{
            low_lim = 56;
            up_lim = 63;
        }
    }
    else{ 
        if((start_pos != 4) || ((board[0] != BLACK * ROOK_CASTLE) && board[7] != BLACK * ROOK_CASTLE))
            return;
        else{
            low_lim = 0;
            up_lim = 7;
        }
    
    }

    int pos = start_pos + 1;
    while(pos < up_lim && board[pos] == EMPTY_SQ){
        //check if there is a check at this position
        pos++;
        
    }
    
    if(pos == up_lim && abs(board[pos]) == ROOK_CASTLE){
        //move.push_back(start_pos + 1);
        //legalMoves.push_back(move);
        //move.pop_back();

        move.push_back(start_pos + 2);
        legalMoves.push_back(move);
        move.pop_back();
    }

    pos = start_pos - 1;

    while(pos > low_lim && board[pos] == EMPTY_SQ){
        //check if there is a check at this position
        pos--;
    }
    
    if(pos == low_lim && abs(board[pos]) == ROOK_CASTLE){
        //move.push_back(start_pos - 1);
        //legalMoves.push_back(move);
        //move.pop_back();

        move.push_back(start_pos - 2);
        legalMoves.push_back(move);
        move.pop_back();
    }
}
int ChessBoard:: TestHash()
{
    node *temp=new node();
    node *temp1=new node();
    node *temp2=new node();
    nd[0]=temp1;
    nd[c-1]=temp2;
    
    temp->HashVal=7;
    nd[0]->next=temp;
    nd[c-1]->HashVal=1;
    nd[0]->HashVal=5;
    int x=nd[0]->next->HashVal-nd[c-1]->HashVal+nd[0]->HashVal;
    return(x);
}
uint64_t ChessBoard:: Gen_Zobrist_Key(int plr)
{
    uint64_t zobrist_key=0;
    int piece=0;
    for(int i=0;i<64;i++)
    {
        piece=board[i];
        if(piece==0)
            continue;
        if(piece==1)
            zobrist_key=zobrist_key^random_num[i][0];
        if(piece==-1)
            zobrist_key=zobrist_key^random_num[i][1];
        if(piece==2)
            zobrist_key=zobrist_key^random_num[i][2];
        if(piece==-2)
            zobrist_key=zobrist_key^random_num[i][3];
        if(piece==3)
            zobrist_key=zobrist_key^random_num[i][4];
        if(piece==-3)
            zobrist_key=zobrist_key^random_num[i][5];
        if(piece==4)
            zobrist_key=zobrist_key^random_num[i][6];
        if(piece==-4)
            zobrist_key=zobrist_key^random_num[i][7];
        if(piece==5)
            zobrist_key=zobrist_key^random_num[i][8];
        if(piece==-5)
            zobrist_key=zobrist_key^random_num[i][9];
        if(piece==6)
            zobrist_key=zobrist_key^random_num[i][10];
        if(piece==-6)
            zobrist_key=zobrist_key^random_num[i][11];
        if(piece==7)
            zobrist_key=zobrist_key^random_num[i][12];
        if(piece==-7)
            zobrist_key=zobrist_key^random_num[i][13];
        if(piece==9)
            zobrist_key=zobrist_key^random_num[i][14];
        if(piece==-9)
            zobrist_key=zobrist_key^random_num[i][15];

    }
    if(plr==-1)
    {
        zobrist_key=zobrist_key^rand;
    }
    return(zobrist_key);
}
ChessBoard:: node* ChessBoard:: create_node(int alpha,int beta,int eval,uint64_t hash,int depth,int move_num)
{
    node* nde=new node();
    if(alpha>127)
        alpha=130;
    if(beta>127)
        beta=130;
    nde->HashVal=hash;
    nde->depth=depth;
    if(eval>127)
        eval=130;
    nde->Eval=eval;
    nde->move_num=move_num;
    return nde;
}
void ChessBoard:: search_node(uint64_t hash)
{
    nd_arr[0]=NULL;
    nd_arr[1]=NULL;
    long loc=hash%c;
    nd_arr[0]=nd[loc];

    if(nd[loc]==NULL)
    {
        return;
    }
    
    for(node* temp=nd[loc];temp!=NULL;temp=temp->next)
    {
        
        if(temp==NULL)
        {
            break;
        }
        if(temp->move_num<move_num)
        {
            nd_arr[0]=temp;
            nd_arr[1]=temp;
            return;
        }
        if(temp->HashVal==hash)
        {
            nd_arr[0]=temp;
            nd_arr[1]=NULL;
            return;
        }
        else{
            nd_arr[1]=temp;
        }
    }
    return;
}

long long evaluate(int depth, ChessBoard &base_board, int &plr){
    vector<vector<int>> moves;

    try{
        if(depth == 0)
            return 1;
        

        long long move_count = 0;

        moves = base_board.genMovesForEachPiece(plr);
        vector<int> temp_board = base_board.board;
        int w_king=base_board.wking_pos;
        int b_king=base_board.bking_pos;
        int en_pessant=base_board.En_pessant_pos;
        int half_move=base_board.half_move;
        //int check_piece = base_board.check_piece_pos;
        for(int i = 0; i < moves.size(); i++){
            //ChessBoard new_board = base_board;
            
            //int captured = moves[i][1];
            //int captured_piece = base_board.board[captured];

            /*if(depth == 5){
                cout<<moves[i][0]<<", "<<moves[i][1]<<": \n";
            }*/

            base_board.ChangeBoard(moves[i][0], moves[i][1]);
            
            plr = -plr;

            move_count += evaluate(depth - 1, base_board, plr);
            
            plr = -plr;
            /*if(depth == 6){
                cout<<"After changing "<<moves[i][0]<<", "<<moves[i][1]<<": ";
                cout<<move_count<<endl;
            }*/

            base_board.board = temp_board;
            base_board.wking_pos=w_king;
            base_board.bking_pos=b_king;
            base_board.En_pessant_pos=en_pessant;
            base_board.half_move=half_move;
            //base_board.check_piece_pos = check_piece;
        }
        
        
        //printing for debugging
        if(depth == 5){
            
            /*for(int i = 0; i < 64; i++){
                if(i % 8 == 0)
                    cout<<endl;
                
                cout<<setw(3)<<base_board.board[i];
            }
            cout<<endl;*/

            cout<<move_count<<endl;
        }

        return move_count;
    }
    catch(...){
        for(int i = 0; i < 64; i++){
            if(i % 8 == 0)
                cout<<endl;
            
            cout<<setw(3)<<base_board.board[i]<<",";
        }

        cout<<endl;
        cout<<"player: "<<plr<<endl;
        
        for(int i = 0; i < base_board.legalMoves.size(); i++){
            cout<<base_board.legalMoves[i][0]<<", "<<base_board.legalMoves[i][1]<<endl;
        }

        cout<<endl;
        cout<<base_board.En_pessant_pos;
        exit(0);
    }
}

vector<vector<int>> ChessBoard::getCaptureMoves(int plr){
    vector<vector<int>> capturemovelist;
    for(auto move_it = legalMoves.begin(); move_it != legalMoves.end(); move_it++){
        auto move_start = move_it->begin();
        auto move_end = move_start + 1;
        if(board[*move_end] != 0 && (board[*move_start] != plr * PAWN && board[*move_end] != -plr * EN_PASSANT_SQ)){
            capturemovelist.push_back(*move_it);
        }
        else if(board[*move_start] == plr * PAWN && board[*move_end] != 0){
            capturemovelist.push_back(*move_it);
        }
    }

    return capturemovelist;
}

int getBoardValue(ChessBoard &b, int &plr){
    static const int values[] = {0, 0, 1, 3, 3, 9, 5, 5};
    int totalvalue = 0;
    
    /*
    for(auto it = b.whitepieces.begin(); it != b.whitepieces.end(); it++){
        totalvalue += values[b.board[*it]];
    }

    for(auto it = b.blackpieces.begin(); it != b.blackpieces.end(); it++){
        totalvalue -= values[abs(b.board[*it])];
    }
    */
    for(int i = 0; i < 64; i++){
        if(b.board[i] < 0 && abs(b.board[i]) != EN_PASSANT_SQ){
            totalvalue -= values[abs(b.board[i])];
        }
        else if(b.board[i] > 0 && abs(b.board[i]) != EN_PASSANT_SQ){
            totalvalue += values[b.board[i]];
        }
            
    }
    return totalvalue;
}

int minimax(ChessBoard &b, int &plr, int depth){
    if(depth == 0){
        countmoves++;
        //cout<<getBoardValue(b, plr)<<endl;
        return getBoardValue(b, plr);
    }
    
    vector<vector<int>> moves;
    moves = b.genMovesForEachPiece(plr);
    vector<int> temp_board = b.board;
    int w_king = b.wking_pos;
    int b_king = b.bking_pos;
    int en_pessant = b.En_pessant_pos;
    int half_move = b.half_move;

    int bestvalue = -99999;
    int promoteval = 0;
    for(int i = 0; i < moves.size(); i++){
        //cout<<moves[i][0]<<", "<<moves[i][1]<<endl;
        int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

        if(ispromote != 0 && promoteval < 4){
            b.Promote(moves[i][1], promoteval, plr);
            promoteval++;
            //if(promoteval <= 3)
            //    i--;
        }
        if(promoteval == 4)
            promoteval = 0;

        plr = -plr;

        int value = -minimax(b, plr, depth - 1);
        
        if(value > bestvalue){
            bestvalue = value;
        }
        
        plr = -plr;

        b.board = temp_board;
        b.wking_pos = w_king;
        b.bking_pos = b_king;
        b.En_pessant_pos = en_pessant;
        b.half_move = half_move;

        
    }

    return bestvalue;
}

int searchCaptures(ChessBoard &b, int plr, int alpha, int beta){
    int value = getBoardValue(b, plr);
    if(plr == WHITE){
        if(value >= beta)
            return value;
        alpha = max(value, alpha);
    }
    else{
        if(value <= alpha)
            return value;
        
        beta = min(value, beta);
    }

    vector<vector<int>> moves;
    moves = b.genMovesForEachPiece(plr);
    vector<int> temp_board = b.board;
    int w_king = b.wking_pos;
    int b_king = b.bking_pos;
    int en_pessant = b.En_pessant_pos;
    int half_move = b.half_move;
    
    vector<vector<int>> capturemoves = b.getCaptureMoves(plr);

    int promoteval = 0;
    if(plr == WHITE){
        value = -127;
        for(int i = 0; i < capturemoves.size(); i++){
            int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

            if(ispromote != 0 && promoteval < 4){
                b.Promote(moves[i][1], promoteval, plr);
                promoteval++;
                //if(promoteval <= 3)
                //    i--;
            }
            if(promoteval == 4)
                promoteval = 0;

            plr = -plr;

            value = max(value, searchCaptures(b, plr, alpha, beta));

            plr = -plr;

            b.board = temp_board;
            b.wking_pos = w_king;
            b.bking_pos = b_king;
            b.En_pessant_pos = en_pessant;
            b.half_move = half_move;
            

            if(value >= beta)
                break;
                
            
            alpha = max(alpha, value);
        }
        return value;
    }
    else{
        value = 127;
        for(int i = 0; i < moves.size(); i++){
            
            int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

            if(ispromote != 0 && promoteval < 4){
                b.Promote(moves[i][1], promoteval, plr);
                promoteval++;
                //if(promoteval <= 3)
                //    i--;
            }
            if(promoteval == 4)
                promoteval = 0;
            
            plr = -plr;

            value = min(value, searchCaptures(b, plr, alpha, beta));

            plr = -plr;

            b.board = temp_board;
            b.wking_pos = w_king;
            b.bking_pos = b_king;
            b.En_pessant_pos = en_pessant;
            b.half_move = half_move;
            

            if(value <= alpha)
            {
                //flag=true;
                break;
            }
            beta = min(value, beta);
        }
        return value;
    }
}


int minimaxAlphaBetaZobrist(ChessBoard &b, int plr, int depth, int alpha, int beta){
    
    int val=130;
    bool flag=false;
    uint64_t key= b.Gen_Zobrist_Key(plr);
    b.search_node(key);
    bool array_null=false;
    bool no_node=true;
    long loc=0;
    
    if(b.nd_arr[0]==NULL && b.nd_arr[1]==NULL)
    {
        array_null=true;
        loc=key%(b.c);
    }
    else if(b.nd_arr[0]!=NULL && b.nd_arr[1]==NULL)
    {
        no_node=false;
        if(b.nd_arr[0]->Eval<=127 && b.nd_arr[0]->depth>=depth)
        {   
            return(b.nd_arr[0]->Eval);   
        }
    }
    if(b.nd_arr[0]!=NULL && b.nd_arr[1]!=NULL)
    {
        no_node=false;
    }
    
    if(depth == 0){
        
        int value=getBoardValue(b, plr);
        val=value;
        if(b.nd_arr[0]!=NULL && b.nd_arr[1]!=NULL)
        {
            b.nd_arr[0]->move_num=b.move_num;
            b.nd_arr[0]->Eval=val;
            b.nd_arr[0]->depth=depth;
        }
        if(array_null)
        {
            b.nd[loc]=b.create_node(alpha,beta,val,key,depth,b.move_num);
        }
        else if(no_node)
        {
            b.nd_arr[1]->next=b.create_node(alpha,beta,val,key,depth,b.move_num);
        }
        else
        {
            if(b.nd_arr[0]->Eval>127 && val<=127)
            {
                b.nd_arr[0]->Eval=val;
                b.nd_arr[0]->depth=depth;
            }
        }
        countmoves++;
        //return getBoardValue(b, plr);
        //calling the function for searching capture moves
        return searchCaptures(b, plr, alpha, beta);
    }
    
    vector<vector<int>> moves;
    
    moves = b.genMovesForEachPiece(plr);
    
    if(moves.size() == 0){
        if(b.IsCheck(plr) > 0){
            if(plr == WHITE)
                return -128;
            else
                return 128;
        }
        else
            return 0;
    }
    
    vector<int> temp_board = b.board;
    int w_king = b.wking_pos;
    int b_king = b.bking_pos;
    int en_pessant = b.En_pessant_pos;
    int half_move = b.half_move;
    
    int promoteval = 0;
    if(plr == WHITE){
        //int value = -99999;
        int value = -127;
        for(int i = 0; i < moves.size(); i++){
            int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

            if(ispromote != 0 && promoteval < 4){
                b.Promote(moves[i][1], promoteval, plr);
                promoteval++;
                //if(promoteval <= 3)
                //    i--;
            }
            if(promoteval == 4)
                promoteval = 0;

            plr = -plr;

            value = max(value, minimaxAlphaBetaZobrist(b, plr, depth - 1, alpha, beta));

            plr = -plr;

            b.board = temp_board;
            b.wking_pos = w_king;
            b.bking_pos = b_king;
            b.En_pessant_pos = en_pessant;
            b.half_move = half_move;
            

            if(value >= beta)
                break;
                
            
            alpha = max(alpha, value);
        }
        b.search_node(key);
        
        if(!flag)
        {
            val=value;
        }
        if(b.nd_arr[0]!=NULL && b.nd_arr[1]!=NULL)
        {
            b.nd_arr[0]->move_num=b.move_num;
            b.nd_arr[0]->Eval=val;
            b.nd_arr[0]->depth=depth;
        }
        if(array_null)
        {
            b.nd[loc]=b.create_node(alpha,beta,val,key,depth,b.move_num);
        }
        else if(no_node)
        {
            b.nd_arr[1]->next=b.create_node(alpha,beta,val,key,depth,b.move_num);
        }
        else
        {
            if(b.nd_arr[0]->Eval>127 && val<=127)
            {
                b.nd_arr[0]->Eval=val;
                b.nd_arr[0]->depth=depth;
            }
            if(b.nd_arr[0]->Eval<=127 && val<=127 && b.nd_arr[0]->depth<depth)
            {
                b.nd_arr[0]->Eval=val;
                b.nd_arr[0]->depth=depth;
            }
        }
        
        return value;
    }

    else{
        //int value = 99999;
        int value = 127;
        for(int i = 0; i < moves.size(); i++){
            int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

            if(ispromote != 0 && promoteval < 4){
                b.Promote(moves[i][1], promoteval, plr);
                promoteval++;
                //if(promoteval <= 3)
                //    i--;
            }
            if(promoteval == 4)
                promoteval = 0;

            plr = -plr;

            value = min(value, minimaxAlphaBetaZobrist(b, plr, depth - 1, alpha, beta));

            plr = -plr;

            b.board = temp_board;
            b.wking_pos = w_king;
            b.bking_pos = b_king;
            b.En_pessant_pos = en_pessant;
            b.half_move = half_move;
            

            if(value <= alpha)
            {
                flag=true;
                break;
            }
            beta = min(value, beta);
        }
        
        b.search_node(key);
        
        if(!flag)
        {
            val=value;
        }
        if(b.nd_arr[0]!=NULL && b.nd_arr[1]!=NULL)
        {
            b.nd_arr[0]->move_num=b.move_num;
            b.nd_arr[0]->Eval=val;
            b.nd_arr[0]->depth=depth;
        }
        if(array_null)
        {
            b.nd[loc]=b.create_node(alpha,beta,val,key,depth,b.move_num);
        }
        else if(no_node)
        {
            b.nd_arr[1]->next=b.create_node(alpha,beta,val,key,depth,b.move_num);
        }
        else
        {
            if(b.nd_arr[0]->Eval>127 && val<=127)
            {
                b.nd_arr[0]->Eval=val;
                b.nd_arr[0]->depth=depth;
            }
        }
        
        return value;
    }
}
int minimaxAlphaBeta(ChessBoard &b, int plr, int depth, int alpha, int beta){
    int val=130;
    bool flag=false;
    /*
    uint64_t key= b.Gen_Zobrist_Key(plr);
    b.search_node(key);
    
 
    bool array_null=false;
    bool no_node=true;
    long loc=0;
    if(b.nd_arr[0]==NULL && b.nd_arr[1]==NULL)
    {
        array_null=true;
        loc=key%(b.c);
    }
    else if(b.nd_arr[0]!=NULL && b.nd_arr[1]==NULL)
    {
        no_node=false;
        if(b.nd_arr[0]->Eval<=127 && b.nd_arr[0]->depth>=depth)
        {
            return(b.nd_arr[0]->Eval);
        }
    }
    */
    
    if(depth == 0){
        countmoves++;
        return getBoardValue(b, plr);
    }
    
    vector<vector<int>> moves;
    moves = b.genMovesForEachPiece(plr);
    vector<int> temp_board = b.board;
    int w_king = b.wking_pos;
    int b_king = b.bking_pos;
    int en_pessant = b.En_pessant_pos;
    int half_move = b.half_move;
    
    int promoteval = 0;
    if(plr == WHITE){
        int value = -127;
        for(int i = 0; i < moves.size(); i++){
            
            int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

            if(ispromote != 0 && promoteval < 4){
                b.Promote(moves[i][1], promoteval, plr);
                promoteval++;
                //if(promoteval <= 3)
                //    i--;
            }
            if(promoteval == 4)
                promoteval = 0;
            
            plr = -plr;

            value = max(value, minimaxAlphaBeta(b, plr, depth - 1, alpha, beta));

            plr = -plr;

            b.board = temp_board;
            b.wking_pos = w_king;
            b.bking_pos = b_king;
            b.En_pessant_pos = en_pessant;
            b.half_move = half_move;
            

            if(value >= beta)
                break;
                
            
            alpha = max(alpha, value);
        }
        /*
        b.search_node(key);
        
        if(!flag)
        {
            val=value;
        }
        if(array_null)
        {
            b.nd[loc]=b.create_node(alpha,beta,val,key,depth);
        }
        else if(no_node)
        {
            b.nd_arr[1]->next=b.create_node(alpha,beta,val,key,depth);
        }
        else
        {
            if(b.nd_arr[0]->Eval>127 && val<=127)
            {
                b.nd_arr[0]->Eval=val;
                b.nd_arr[0]->depth=depth;
            }
        }
        */
        return value;
    }

    else{
        int value = 127;
        for(int i = 0; i < moves.size(); i++){
            
            int ispromote = b.ChangeBoard(moves[i][0], moves[i][1]);

            if(ispromote != 0 && promoteval < 4){
                b.Promote(moves[i][1], promoteval, plr);
                promoteval++;
                //if(promoteval <= 3)
                //    i--;
            }
            if(promoteval == 4)
                promoteval = 0;
            
            plr = -plr;

            value = min(value, minimaxAlphaBeta(b, plr, depth - 1, alpha, beta));

            plr = -plr;

            b.board = temp_board;
            b.wking_pos = w_king;
            b.bking_pos = b_king;
            b.En_pessant_pos = en_pessant;
            b.half_move = half_move;
            

            if(value <= alpha)
            {
                flag=true;
                break;
            }
            beta = min(value, beta);
        }
        /*
        b.search_node(key);
        if(!flag)
        {
            val=value;
        }
        if(array_null)
        {
            b.nd[loc]=b.create_node(alpha,beta,val,key,depth);
        }
        else if(no_node)
        {
            b.nd_arr[1]->next=b.create_node(alpha,beta,val,key,depth);
        }
        else
        {
            if(b.nd_arr[0]->Eval>127 && val<=127)
            {
                b.nd_arr[0]->Eval=val;
                b.nd_arr[0]->depth=depth;
            }
        }
        */
        return value;
    }


}
vector<int> EvaluateBoard(ChessBoard &b, int plr)
{
    int alpha=-9999;
    int beta=9999;
    
    int best_eval=-plr*9999;
    int value=0;
    b.move_num++;
    vector<int> move_eval;
    vector<vector<int>> moves=b.genMovesForEachPiece(plr);
    vector<vector<int>> mvscpy=moves;
    vector<int>best_move;
    best_move.push_back(65);
    best_move.push_back(65);
    best_move.push_back(65);
    if(moves.size()>0)
    {
        best_move[0]=moves[0][0];
        best_move[1]=moves[0][1];
    }
    vector<float> evals;
    vector<int>best_move_final;
    int promoteval = 0;
    for(int k=0;k<5;k++)
    {
        best_move[0]=moves[0][0];
        best_move[1]=moves[0][1];
        move_eval.clear();
        if(plr==WHITE)
        {
            value=-9999;
        }
        if(plr==BLACK)
        {
            value=9999;
        }
        int alpha=-9999;
        int beta=9999;
        best_eval=-plr*9999;
        for(int i=0;i<mvscpy.size();i++)
        {
            vector<int> temp_board = b.board;
            int w_king = b.wking_pos;
            int b_king = b.bking_pos;
            int en_pessant = b.En_pessant_pos;
            int half_move = b.half_move;
            if(plr == WHITE)
            {

                if(b.ChangeBoard(moves[i][0], moves[i][1])!=0)
                {
                    if(moves[i].size()!=3)
                    {
                        moves[i].push_back(0);
                        moves[i+1].push_back(1);
                        moves[i+2].push_back(2);
                        moves[i+3].push_back(3);
                    }
                    b.Promote(moves[i][1],moves[i][2],plr);


                }
                plr = -plr;
                value = minimaxAlphaBetaZobrist(b, plr, k+1, alpha, beta);
                move_eval.push_back(value);
                if(value>best_eval)
                {
                    best_move[0]=moves[i][0];
                    best_move[1]=moves[i][1];
                    if(moves[i].size()==3)
                    {
                        best_move[2]=moves[i][2];
                    }
                    best_eval=value;
                }
                if(value==best_eval)
                {
                    if(moves[i].size()==3 && best_move.size()==2)
                    {
                        best_move[0]=moves[i][0];
                        best_move[1]=moves[i][1];
                        best_move[2]=moves[i][2];
                        best_eval=value;
                    }
                }
                plr = -plr;

                b.board = temp_board;
                b.wking_pos = w_king;
                b.bking_pos = b_king;
                b.En_pessant_pos = en_pessant;
                b.half_move = half_move;
                alpha = max(alpha, value);    
            } 
            else
            {
            
                

                if(b.ChangeBoard(moves[i][0], moves[i][1])!=0)
                {
                    if(moves[i].size()!=3)
                    {
                        moves[i].push_back(0);
                        moves[i+1].push_back(1);
                        moves[i+2].push_back(2);
                        moves[i+3].push_back(3);
                    }
                    b.Promote(moves[i][1],moves[i][2],plr);


                }
                plr = -plr;

                value = minimaxAlphaBetaZobrist(b, plr,  k, alpha, beta);
                move_eval.push_back(value);
                if(value==best_eval)
                {
                    if(moves[i].size()==3 && best_move.size()==2)
                    {
                        best_move[0]=moves[i][0];
                        best_move[1]=moves[i][1];
                        best_move[2]=moves[i][2];
                        best_eval=value;
                    }
                }
                if(value<best_eval)
                {
                    best_move[0]=moves[i][0];
                    best_move[1]=moves[i][1];
                    if(moves[i].size()==3)
                    {
                        best_move[2]=moves[i][2];
                    }
                    best_eval=value;
                }

                plr = -plr;

                b.board = temp_board;
                b.wking_pos = w_king;
                b.bking_pos = b_king;
                b.En_pessant_pos = en_pessant;
                b.half_move = half_move;
                beta = min(value, beta);
            }
        }
        best_move_final=best_move;
        int n=moves.size();
        
        if(plr==WHITE)
        {
            for (int i = 0; i < n-1; i++)
            {
                for (int j = 0; j < n-i-1; j++)
                {
                    
                    if (move_eval[j] < move_eval[j+1])
                    {
                            // swap arr[j+1] and arr[j]
                            int temp = move_eval[j];
                            move_eval[j] = move_eval[j+1];
                            move_eval[j+1] = temp;
                            vector<int> temp1 =moves[j];
                            moves[j] = moves[j+1];
                            moves[j+1] = temp1;
                    }
                }
            }
        }
        else{
            for (int i = 0; i < n-1; i++)
            {
                for (int j = 0; j < n-i-1; j++)
                {
                    if (move_eval[j] > move_eval[j+1])
                    {
                            // swap arr[j+1] and arr[j]
                            int temp = move_eval[j];
                            move_eval[j] = move_eval[j+1];
                            move_eval[j+1] = temp;
                            vector<int> temp1 = moves[j];
                            moves[j] = moves[j+1];
                            moves[j+1] = temp1;
                    }
                }
            }
        }
        
    }
    return(best_move_final);
}