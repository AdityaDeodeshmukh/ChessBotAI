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
pygame.init()
pygame.mixer.quit()
pygame.font.init()
WHITE=(255,255,255)
BLACK=(0,0,0)
PLAYER_FONT=pygame.font.SysFont('Inter',25)
QUIT_FONT=pygame.font.SysFont('Inter',30)
PLAYER="Player"
ENGINE="Holly"
player_text=PLAYER_FONT.render(PLAYER,1,BLACK)
engine_text=PLAYER_FONT.render(ENGINE,1,BLACK)
quit_text=QUIT_FONT.render("Quit",1,WHITE)
WIDTH,HEIGHT=1280,720
BOARD_SIZE=(640,640)
PEICE_SIZE=(int(BOARD_SIZE[0]/8),int(BOARD_SIZE[1]/8))
START_BOARD=(320,40)

WIN=pygame.display.set_mode((WIDTH,HEIGHT))
FPS=60
QUIT_START=(1040,635)
QUIT_DIM=(81,45)
PROM_START=(1040,341)
PROM_DIM=(160,160)
BOT_RECT=pygame.Rect(55,40,223,65)
PLAYER_RECT=pygame.Rect(55,615,223,65)
PROMOTE_RECT=pygame.Rect(PROM_START[0],PROM_START[1],PROM_DIM[0],PROM_DIM[1])
QUIT_RECT=pygame.Rect(QUIT_START[0],QUIT_START[1],QUIT_DIM[0],QUIT_DIM[1])
#loading all the assets into pygame
BOARD=pygame.image.load(os.path.join('Assets','ChessBoard.jpg')).convert_alpha()
BOARD=pygame.transform.scale(BOARD,BOARD_SIZE)
PROMOTE_REGION=pygame.image.load(os.path.join('Assets','Promote.png')).convert_alpha()
PROMOTE_REGION=pygame.transform.scale(PROMOTE_REGION,(160,160))
B_KING=pygame.image.load(os.path.join('Assets','B_King.png')).convert_alpha()

W_KING=pygame.image.load(os.path.join('Assets','W_King.png')).convert_alpha()

B_QUEEN=pygame.image.load(os.path.join('Assets','B_Queen.png')).convert_alpha()

W_QUEEN=pygame.image.load(os.path.join('Assets','W_Queen.png')).convert_alpha()

B_ROOK=pygame.image.load(os.path.join('Assets','B_Rook.png')).convert_alpha()

W_ROOK=pygame.image.load(os.path.join('Assets','W_Rook.png')).convert_alpha()

B_KNIGHT=pygame.image.load(os.path.join('Assets','B_Knight.png')).convert_alpha()

W_KNIGHT=pygame.image.load(os.path.join('Assets','W_Knight.png')).convert_alpha()

B_BISHOP=pygame.image.load(os.path.join('Assets','B_Bishop.png')).convert_alpha()

W_BISHOP=pygame.image.load(os.path.join('Assets','W_Bishop.png')).convert_alpha()

B_PAWN=pygame.image.load(os.path.join('Assets','B_Pawn.png')).convert_alpha()

W_PAWN=pygame.image.load(os.path.join('Assets','W_Pawn.png')).convert_alpha()

BACKGROUND=pygame.image.load(os.path.join('Assets','Back.png')).convert_alpha()

BOT_IMG=pygame.image.load(os.path.join('Assets','Bot.png')).convert_alpha()

PLAYER_IMG=pygame.image.load(os.path.join('Assets','Player.png')).convert_alpha()

UPPER_BOARD=pygame.Rect(0,0,WIDTH,START_BOARD[1])
LOWER_BOARD=pygame.Rect(0,HEIGHT-START_BOARD[1],WIDTH,START_BOARD[1])
player=-1
ev=pygame.event.get()
#A utility function to convert a cpp vector to a list
def extractlist(lst):
    return [list(el) for el in lst]
def checkInside(coords,c1,c2):
    if(coords[0] in range(c1[0],c2[0]) and coords[1] in range(c1[1],c2[1])):
        return True
    return False
#A function to get the square at which the mouse is pointing
#coords: tuple of the x and y coordinates
#player: 1 if White and -1 if Black
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
        #if player is playing black then the board is inverted so return 7-x and 7-y
    return (x1,y1)
#A function to draw the peices onto the board
#ch_board: A 2D array representation of the board
#locinitial: The initial location of the mouse while dragging
#locfinal: The final position of mouse while dragging
#peice: The piece that was selected when dragging
#player: 1 if White and -1 if Black
def draw_peices(ch_board,locfinal,locinitial,peice,player):
    chess_board=ch_board.copy()
    x=0
    y=0
    sqrIn=(0,0)
    sqrIn=getsquare(locinitial,player)
    #convert the 2D array to a reversed 2D array if player is playing black
    if(player==-1):
        chess_board=chess_board.flatten()
        chess_board=chess_board[::-1]
        chess_board=np.reshape(chess_board,(8,8))
    #defines the square where dragging started
    if(peice!=0):
        x=sqrIn[0]
        y=sqrIn[1]
        if(peice!=0):
            if(player==-1):
                x=7-x
                y=7-y
        
    #Start drawing    
    for i in range(0,8):
        for j in range(0,8):
            offset=(0,0)
            #if the square corresponds to the square where dragging started then pass(We print that
            # last to avoid the piece going under other pieces)
            if x==j and y==i and peice!=0:
                continue  
            #check which piece it is and print it      
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
            if chess_board[i][j]==6 or chess_board[i][j]==7:
                WIN.blit(W_ROOK,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
            if chess_board[i][j]==-6 or chess_board[i][j]==-7:
                WIN.blit(B_ROOK,(START_BOARD[0]+j*(PEICE_SIZE[0]),START_BOARD[1]+i*(PEICE_SIZE[0])))
    offset=(locfinal[0]-locinitial[0],locfinal[1]-locinitial[1])
    
    
    
    #here we print the piece that was selected when we started dragging
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
    if peice==6 or peice== 7:
        WIN.blit(W_ROOK,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))
    if peice==-6 or peice== -7:
        WIN.blit(B_ROOK,(START_BOARD[0]+x*(PEICE_SIZE[0])+offset[0],START_BOARD[1]+y*(PEICE_SIZE[0])+offset[1]))


            


#The main function to draw the board
#chess_board: 2D array representation of the board
#locinitial: The initial location of the mouse while dragging
#locfinal: The final position of mouse while dragging
#peice: The piece that was selected when dragging
#player: 1 if White and -1 if Black
def draw_promote(prom):
    if(prom==1):
        WIN.blit(W_QUEEN,(PROM_START))
        WIN.blit(W_KNIGHT,(PROM_START[0],PROM_START[1]+80))
        WIN.blit(W_BISHOP,(PROM_START[0]+80,PROM_START[1]))
        WIN.blit(W_ROOK,(PROM_START[0]+80,PROM_START[1]+80))
    if(prom==-1):
        WIN.blit(B_QUEEN,(PROM_START))
        WIN.blit(B_KNIGHT,(PROM_START[0],PROM_START[1]+80))
        WIN.blit(B_BISHOP,(PROM_START[0]+80,PROM_START[1]))
        WIN.blit(B_ROOK,(PROM_START[0]+80,PROM_START[1]+80))
def draw_main(chess_board,locfinal,locinitial,peice,player,prom):
    WIN.blit(BACKGROUND,(0,0))
    WIN.blit(BOARD,START_BOARD)
    #pygame.draw.rect(WIN,BG2,UPPER_BOARD)
    #pygame.draw.rect(WIN,BG2,LOWER_BOARD)
    pygame.draw.rect(WIN,WHITE,BOT_RECT,0,8)
    pygame.draw.rect(WIN,WHITE,PLAYER_RECT,0,8)
    #pygame.draw.rect(WIN,(116,150,85),PROMOTE_RECT)
    pygame.draw.rect(WIN,(235, 87, 87),QUIT_RECT,0,8)
    WIN.blit(PROMOTE_REGION,PROM_START)
    WIN.blit(engine_text,(112,63))
    WIN.blit(player_text,(112,638))
    WIN.blit(quit_text,(1058,649))
    WIN.blit(BOT_IMG,(71,56))
    WIN.blit(PLAYER_IMG,(71,631))
    draw_peices(chess_board,locfinal,locinitial,peice,player)
    if prom!=0:
        draw_promote(prom)
    pygame.display.update()
def checkpeice():
    pass
#The main body of the game
def main():
    clock=pygame.time.Clock()
    prom=0
    prom_sqr=(65,65)
    drag=False
    FEN="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    chess_board,plr,half_move,full_move=fen_code_parser(FEN)
  
    #creates the board object
    board=ChessBoard(chess_board.flatten(),half_move)
    board.getEdgeDistance()

    moveset=extractlist(list(board.genMovesForEachPiece(plr)))
    run=True
    peice=0
    player=1
    chess_board=(list(board.board))
    chess_board=np.reshape(chess_board,(8,8))
    locinitial=(-1,-1)
    #main loop of the game
    while run:
        clock.tick(FPS)
        
        
        flag=False
        locfinal=(0,0)
        
        for event in pygame.event.get():
            if(event.type==pygame.QUIT):
                run=False
            #If dragging of mouse has started
            if(event.type==pygame.MOUSEBUTTONDOWN and prom==0):
                if event.button==1 and drag==False:
                    peice=0
                    drag=True
                    locinitial=pygame.mouse.get_pos()
                    locfinal=pygame.mouse.get_pos()
                    sqrIn=getsquare(locinitial,player)
                    if sqrIn[1] in range(0,8) and sqrIn[0] in range(0,8):
                        peice=chess_board[sqrIn[1],sqrIn[0]]
            


            #If dragging of mouse has stopped
            if(event.type==pygame.MOUSEBUTTONUP and prom==0):
                if event.button==1:
                    drag=False
                    locfinal=pygame.mouse.get_pos()
                    sqrFin=getsquare(locfinal,player)
                    
                    if(peice!=0):
                        
                        if sqrFin[0]!=65 and sqrFin!=65 and sqrIn[0]!=65 and sqrIn[1]!=65:
                            move=[sqrIn[0]+sqrIn[1]*8,sqrFin[0]+sqrFin[1]*8]
                            if move in moveset:
                                prom=board.ChangeBoard(move[0],move[1])
                                
                                if(prom!=0):
                                    prom_sqr=move[1]
                                chess_board=list(board.board)
                                chess_board=np.reshape(chess_board,(8,8))
                                print(board.IsCheck(-plr))
                                if(prom==0):
                                    plr=-plr
                                    moveset=extractlist(list(board.genMovesForEachPiece(plr)))
                                    
                        peice=0
            if(event.type==pygame.MOUSEBUTTONDOWN and prom!=0):
                if event.button==1 and drag==False:
                    drag=True
                    locinitial=pygame.mouse.get_pos()
                    locfinal=pygame.mouse.get_pos()
            if(event.type==pygame.MOUSEBUTTONUP and prom!=0):
                if event.button==1:
                    locfinal=pygame.mouse.get_pos()
                    drag=False
                    if(checkInside(locinitial,PROM_START,(PROM_START[0]+80,PROM_START[1]+80)) and checkInside(locfinal,PROM_START,(PROM_START[0]+80,PROM_START[1]+80))):
                        board.Promote(prom_sqr,0,prom)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        moveset=extractlist(list(board.genMovesForEachPiece(plr)))
                        prom=0
                        prom_sqr=(65,65)
                        
                    elif(checkInside(locinitial,(PROM_START[0]+80,PROM_START[1]),(PROM_START[0]+160,PROM_START[1]+80)) and checkInside(locfinal,(PROM_START[0]+80,PROM_START[1]),(PROM_START[0]+160,PROM_START[1]+80))):
                        board.Promote(prom_sqr,2,prom)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        moveset=extractlist(list(board.genMovesForEachPiece(plr)))
                        prom=0
                        prom_sqr=(65,65)
                    elif(checkInside(locinitial,(PROM_START[0],PROM_START[1]+80),(PROM_START[0]+80,PROM_START[1]+160)) and checkInside(locfinal,(PROM_START[0],PROM_START[1]+80),(PROM_START[0]+80,PROM_START[1]+160))):
                        board.Promote(prom_sqr,1,prom)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        moveset=extractlist(list(board.genMovesForEachPiece(plr)))
                        prom=0
                        prom_sqr=(65,65)
                    elif(checkInside(locinitial,(PROM_START[0]+80,PROM_START[1]+80),(PROM_START[0]+160,PROM_START[1]+160)) and checkInside(locfinal,(PROM_START[0]+80,PROM_START[1]+80),(PROM_START[0]+160,PROM_START[1]+160))):
                        board.Promote(prom_sqr,3,prom)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        moveset=extractlist(list(board.genMovesForEachPiece(plr)))
                        prom=0
                        prom_sqr=(65,65)
                    
                    
                   
        if drag==True:
            locfinal=pygame.mouse.get_pos()
        
        draw_main(chess_board,locfinal,locinitial,peice,player,prom)            
        

    pygame.quit()
if __name__== "__main__":
    main()

