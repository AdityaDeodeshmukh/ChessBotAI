import pygame
import numpy as np
import os
from Functions.FEN_parser import fen_code_parser
from Functions.CompileC import CompileClass
import time
from pygame.locals import *
import cppyy
CompileClass()
from cppyy.gbl import ChessBoard
pygame.font.init()
WHITE=(255,255,255)
PLAYER_FONT=pygame.font.SysFont('comicsans',25)
PLAYER="You"
ENGINE="Holly"
player_text=PLAYER_FONT.render(PLAYER,1,WHITE)
engine_text=PLAYER_FONT.render(ENGINE,1,WHITE)
WIDTH,HEIGHT=1280,720
BOARD_SIZE=(640,640)
PEICE_SIZE=(int(BOARD_SIZE[0]/8),int(BOARD_SIZE[1]/8))
START_BOARD=(340,40)
BG2=(50,50,50)
BG=(70,70,70)
WIN=pygame.display.set_mode((WIDTH,HEIGHT))
FPS=120
BOARD=pygame.image.load(os.path.join('Assets','ChessBoard.jpg'))
BOARD=pygame.transform.scale(BOARD,BOARD_SIZE)
B_KING=pygame.image.load(os.path.join('Assets','B_King.png'))
B_KING=pygame.transform.scale(B_KING,PEICE_SIZE)
W_KING=pygame.image.load(os.path.join('Assets','W_King.png'))
W_KING=pygame.transform.scale(W_KING,PEICE_SIZE)
B_QUEEN=pygame.image.load(os.path.join('Assets','B_Queen.png'))
B_QUEEN=pygame.transform.scale(B_QUEEN,PEICE_SIZE)
W_QUEEN=pygame.image.load(os.path.join('Assets','W_Queen.png'))
W_QUEEN=pygame.transform.scale(W_QUEEN,PEICE_SIZE)
B_ROOK=pygame.image.load(os.path.join('Assets','B_Rook.png'))
B_ROOK=pygame.transform.scale(B_ROOK,PEICE_SIZE)
W_ROOK=pygame.image.load(os.path.join('Assets','W_Rook.png'))
W_ROOK=pygame.transform.scale(W_ROOK,PEICE_SIZE)
B_KNIGHT=pygame.image.load(os.path.join('Assets','B_Knight.png'))
B_KNIGHT=pygame.transform.scale(B_KNIGHT,PEICE_SIZE)
W_KNIGHT=pygame.image.load(os.path.join('Assets','W_Knight.png'))
W_KNIGHT=pygame.transform.scale(W_KNIGHT,PEICE_SIZE)
B_BISHOP=pygame.image.load(os.path.join('Assets','B_Bishop.png'))
B_BISHOP=pygame.transform.scale(B_BISHOP,PEICE_SIZE)
W_BISHOP=pygame.image.load(os.path.join('Assets','W_Bishop.png'))
W_BISHOP=pygame.transform.scale(W_BISHOP,PEICE_SIZE)
B_PAWN=pygame.image.load(os.path.join('Assets','B_Pawn.png'))
B_PAWN=pygame.transform.scale(B_PAWN,PEICE_SIZE)
W_PAWN=pygame.image.load(os.path.join('Assets','W_Pawn.png'))
W_PAWN=pygame.transform.scale(W_PAWN,PEICE_SIZE)
UPPER_BOARD=pygame.Rect(0,0,WIDTH,START_BOARD[1])
LOWER_BOARD=pygame.Rect(0,HEIGHT-START_BOARD[1],WIDTH,START_BOARD[1])
player=1
ev=pygame.event.get()
def extractlist(lst):
    return [list(el) for el in lst]

def getsquare(coords,player):
    
    x=coords[0]
    y=coords[1]
    if(x>(START_BOARD[0]+BOARD_SIZE[0])) or (x<START_BOARD[0]) or (y>(START_BOARD[1]+BOARD_SIZE[1])) or (y<START_BOARD[1]):
        return(65,65)
    x_ref=x-START_BOARD[0]
    y_ref=y-START_BOARD[1]
    x1=int(x_ref/PEICE_SIZE[0])
    y1=int(y_ref/PEICE_SIZE[0])
    if player==-1:
        return(7-x1,7-y1)
    return (x1,y1)
def draw_peices(ch_board,locfinal,locinitial,peice,player):
    chess_board=ch_board.copy()
    x=0
    y=0
    sqrIn=(0,0)
    sqrIn=getsquare(locinitial,player)
    if(player==-1):
        chess_board=chess_board.flatten()
        chess_board=chess_board[::-1]
        chess_board=np.reshape(chess_board,(8,8))
    
    if(peice!=0):
        x=sqrIn[0]
        y=sqrIn[1]
        if(peice!=0):
            if(player==-1):
                x=7-x
                y=7-y
        
        
    for i in range(0,8):
        for j in range(0,8):
            offset=(0,0)
            
            if x==j and y==i and peice!=0:
                continue        
            if chess_board[i][j]==1:
                WIN.blit(W_KING,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-1:
                WIN.blit(B_KING,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==2:
                WIN.blit(W_PAWN,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-2:
                WIN.blit(B_PAWN,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==3:
                WIN.blit(W_BISHOP,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-3:
                WIN.blit(B_BISHOP,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==4:
                WIN.blit(W_KNIGHT,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-4:
                WIN.blit(B_KNIGHT,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==5:
                WIN.blit(W_QUEEN,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-5:
                WIN.blit(B_QUEEN,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==6:
                WIN.blit(W_ROOK,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-6:
                WIN.blit(B_ROOK,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
    offset=(locfinal[0]-locinitial[0],locfinal[1]-locinitial[1])
    
    
    
    
    if peice==1:
        WIN.blit(W_KING,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-1:
        WIN.blit(B_KING,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==2:
        WIN.blit(W_PAWN,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-2:
        WIN.blit(B_PAWN,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==3:
        WIN.blit(W_BISHOP,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-3:
        WIN.blit(B_BISHOP,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==4:
        WIN.blit(W_KNIGHT,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-4:
        WIN.blit(B_KNIGHT,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==5:
        WIN.blit(W_QUEEN,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-5:
        WIN.blit(B_QUEEN,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==6:
        WIN.blit(W_ROOK,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-6:
        WIN.blit(B_ROOK,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))


            



def draw_main(chess_board,locfinal,locinitial,peice,player):
    WIN.fill(BG)
    WIN.blit(BOARD,START_BOARD)
    WIN.blit(engine_text,(10,START_BOARD[1]+10))
    WIN.blit(player_text,(10,HEIGHT-START_BOARD[1]-player_text.get_height()-10))
    pygame.draw.rect(WIN,BG2,UPPER_BOARD)
    pygame.draw.rect(WIN,BG2,LOWER_BOARD)
    draw_peices(chess_board,locfinal,locinitial,peice,player)
    pygame.display.update()
def checkpeice():
    pass
def main():
    
    clock=pygame.time.Clock()
    drag=False
    FEN="rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
    chess_board=fen_code_parser(FEN)
    board=ChessBoard(chess_board.flatten())
    board.getEdgeDistance()
    plr=-1
    moveset=extractlist(list(board.genMovesForEachPiece(plr)))
    run=True
    peice=0
    player=1
    chess_board=(list(board.board))
    chess_board=np.reshape(chess_board,(8,8))
    locinitial=(-1,-1)
    
    while run:
        clock.tick(FPS)
        
        
        flag=False
        locfinal=(0,0)
        
        for event in pygame.event.get():
            if(event.type==pygame.QUIT):
                run=False
            if(event.type==pygame.MOUSEBUTTONDOWN):
                if event.button==1 and drag==False:
                    drag=True
                    locinitial=pygame.mouse.get_pos()
                    locfinal=pygame.mouse.get_pos()
                    sqrIn=getsquare(locinitial,player)
                    peice=chess_board[sqrIn[1],sqrIn[0]]
                    


            
            if(event.type==pygame.MOUSEBUTTONUP):
                if event.button==1:
                    drag=False
                    locfinal=pygame.mouse.get_pos()
                    sqrFin=getsquare(locfinal,player)
                    
                    if(peice!=0):
                        
                        if sqrFin[0]!=65 and sqrFin!=65 and sqrIn[0]!=65 and sqrIn[1]!=65:
                            move=[sqrIn[0]+sqrIn[1]*8,sqrFin[0]+sqrFin[1]*8]
                            if move in moveset:
                                board.ChangeBoard(move[0],move[1])
                                chess_board=list(board.board)
                                
                                
                                chess_board=np.reshape(chess_board,(8,8))
                                
                                plr=-plr
                                moveset=extractlist(list(board.genMovesForEachPiece(plr)))
                                print(chess_board)
                                

                        peice=0
                   
        if drag==True:
            locfinal=pygame.mouse.get_pos()
        
        draw_main(chess_board,locfinal,locinitial,peice,player)            
        

    pygame.quit()
if __name__== "__main__":
    main()

