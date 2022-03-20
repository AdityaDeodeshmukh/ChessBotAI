
import pygame
import numpy as np
import os
from Functions.FEN_parser import fen_code_parser
from Functions.CompileC import CompileClass
import time
from pygame.locals import *
import cppyy
from pygame import mixer
CompileClass()
from cppyy.gbl import ChessBoard
from cppyy.gbl import minimaxAlphaBetaZobrist
from cppyy.gbl import EvaluateBoard
#end=time.time()
#print(end-strt)
pygame.init()
mixer.init()
pygame.font.init()
WHITE=(255,255,255)
BLACK=(0,0,0)
PLAYER_FONT=pygame.font.SysFont('Inter',25)
QUIT_FONT=pygame.font.SysFont('Inter',30)
DEV_FONT=pygame.font.SysFont('Termina',41)
NAME_FONT=pygame.font.SysFont('Inter',20)
PLAYER="Player"
ENGINE="Holly"
player_text=PLAYER_FONT.render(PLAYER,1,BLACK)
engine_text=PLAYER_FONT.render(ENGINE,1,BLACK)
quit_text=QUIT_FONT.render("Quit",1,WHITE)
dev_text=DEV_FONT.render("Developers",1,WHITE)
des_text=DEV_FONT.render("Designer",1,WHITE)
dev3_name=NAME_FONT.render("Aditya Deodeshmukh",1,WHITE)
dev2_name=NAME_FONT.render("Atharva Shekatkar",1,WHITE)
dev1_name=NAME_FONT.render("Nitish Kumar Bansal",1,WHITE)
WIDTH,HEIGHT=1280,720
BOARD_SIZE=(640,640)
PEICE_SIZE=(int(BOARD_SIZE[0]/8),int(BOARD_SIZE[1]/8))
START_BOARD=(320,40)
CAPTURE_SOUND=mixer.Sound(os.path.join('Assets','Capture.wav'))
MOVE_SOUND=mixer.Sound(os.path.join('Assets','Move.wav'))
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

MOVE_SHADOW=pygame.image.load(os.path.join('Assets','Move_Chess.png')).convert_alpha()

CAPTURE_SHADOW=pygame.image.load(os.path.join('Assets','Capture_Chess.png')).convert_alpha()
DEV1_IMG=pygame.image.load(os.path.join('Assets','Dev1.png')).convert_alpha()
DEV1_IMG=pygame.transform.scale(DEV1_IMG,(40,40))
DEV2_IMG=pygame.image.load(os.path.join('Assets','Dev2.png')).convert_alpha()
DEV2_IMG=pygame.transform.scale(DEV2_IMG,(40,40))
DEV3_IMG=pygame.image.load(os.path.join('Assets','Dev3.png')).convert_alpha()
DEV3_IMG=pygame.transform.scale(DEV3_IMG,(40,40))
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
def draw_main(chess_board,locfinal,locinitial,peice,player,prom,moveset):
    WIN.blit(BACKGROUND,(0,0))
    WIN.blit(BOARD,START_BOARD)
    if(peice!=0):
        draw_moves(chess_board,locfinal,locinitial,peice,player,moveset)
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
    WIN.blit(dev_text,(1042,56))
    WIN.blit(des_text,(1062,221))
    WIN.blit(DEV3_IMG,(1037,109))
    WIN.blit(DEV2_IMG,(1037,165))
    WIN.blit(DEV1_IMG,(1048,266))
    WIN.blit(dev3_name,(1093,119))
    WIN.blit(dev2_name,(1093,180))
    WIN.blit(dev1_name,(1096,280))
    WIN.blit(BOT_IMG,(71,56))
    WIN.blit(PLAYER_IMG,(71,631))
    draw_peices(chess_board,locfinal,locinitial,peice,player)
    if prom!=0:
        draw_promote(prom)
    pygame.display.update()
def draw_moves(chess_board,locfinal,locinitial,peice,player,moveset):
    sqrIn=getsquare(locinitial,player)
    x=sqrIn[0]
    y=sqrIn[1]
    for i in moveset:
        if i[0]==x+y*8:
            end=i[1]
            row=end//8
            col=end%8
            if(chess_board[row][col]!=0 and abs(chess_board[row][col])!=9):
                if(player==-1):
                    row=7-row
                    col=7-col
                WIN.blit(CAPTURE_SHADOW,(START_BOARD[0]+col*(PEICE_SIZE[0]),START_BOARD[1]+row*(PEICE_SIZE[0])))
                continue
            if(abs(chess_board[row][col])==9 and abs(peice)==2):
                if(player==-1):
                    row=7-row
                    col=7-col
                WIN.blit(CAPTURE_SHADOW,(START_BOARD[0]+col*(PEICE_SIZE[0]),START_BOARD[1]+row*(PEICE_SIZE[0])))
                continue

            if(player==-1):
                row=7-row
                col=7-col
            WIN.blit(MOVE_SHADOW,(START_BOARD[0]+col*(PEICE_SIZE[0]),START_BOARD[1]+row*(PEICE_SIZE[0])))

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
    player=1
    plr=player
    moveset=extractlist(list(board.genMovesForEachPiece(plr)))
    run=True
    peice=0
    
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
                                
                                capture=False
                                if(board.board[move[1]]!=0):
                                    capture=True
                                    if(abs(board.board[move[1]])==9 and abs(board.board[move[0]])!=2):
                                        capture=False
                                prom=board.ChangeBoard(move[0],move[1])
                                if(prom!=0):
                                    prom_sqr=move[1]
                                chess_board=list(board.board)
                                chess_board=np.reshape(chess_board,(8,8))
                                if(capture):
                                    CAPTURE_SOUND.play()
                                else:
                                    MOVE_SOUND.play()
                                if(prom==0):
                                    plr=-plr
                                    draw_main(chess_board,locfinal,locinitial,0,player,prom,moveset)
                                    moveset=extractlist(list(board.genMovesForEachPiece(-player)))
                                    if(len(moveset)==0):
                                        if(board.IsCheck(-player)==0):
                                            print("Stalemate")
                                        else:
                                            print("Checkmate")
                                        break
                                    print(moveset)
                                    best_move=list(EvaluateBoard(board,-player))
                                    chess_board=list(board.board)
                                    chess_board=np.reshape(chess_board,(8,8))
                                    pr=board.ChangeBoard(best_move[0],best_move[1])
                                    print(best_move)
                                    if(pr!=0):
                                        board.Promote(best_move[1],best_move[2],plr)
                                    chess_board=list(board.board)
                                    chess_board=np.reshape(chess_board,(8,8))
                                    
                                    moveset=extractlist(list(board.genMovesForEachPiece(player)))
                                    print(moveset)
                                    if(len(moveset)==0):
                                        if(board.IsCheck(player)==0):
                                            print("Stalemate")
                                        else:
                                            print("Checkmate")
                                    plr=-plr
                                    peice=0
                                    continue;
                                    

                                    
                                    
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
                        MOVE_SOUND.play()
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        draw_main(chess_board,locfinal,locinitial,0,player,prom,moveset)
                        best_move=list(EvaluateBoard(board,-player))
                        print(best_move)
                        moves=extractlist(list(board.genMovesForEachPiece(-player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(-player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        print(moves)
                        print(best_move)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        pr=board.ChangeBoard(best_move[0],best_move[1])
                        if(pr!=0):
                            board.Promote(best_move[1],best_move[2],plr)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        moveset=extractlist(list(board.genMovesForEachPiece(player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        plr=-plr
                        peice=0
                        prom=0;
                        prom_sqr=(65,65)
                        continue;
                        
                    elif(checkInside(locinitial,(PROM_START[0]+80,PROM_START[1]),(PROM_START[0]+160,PROM_START[1]+80)) and checkInside(locfinal,(PROM_START[0]+80,PROM_START[1]),(PROM_START[0]+160,PROM_START[1]+80))):
                        board.Promote(prom_sqr,2,prom)
                        MOVE_SOUND.play()
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        moves=extractlist(list(board.genMovesForEachPiece(-player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(-player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        print(moves)
                        print(best_move)
                        draw_main(chess_board,locfinal,locinitial,0,player,prom,moveset)
                        best_move=list(EvaluateBoard(board,-player))
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        pr=board.ChangeBoard(best_move[0],best_move[1])
                        if(pr!=0):
                            board.Promote(best_move[1],best_move[2],plr)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        moveset=extractlist(list(board.genMovesForEachPiece(player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        plr=-plr
                        peice=0
                        prom=0;
                        prom_sqr=(65,65)
                    elif(checkInside(locinitial,(PROM_START[0],PROM_START[1]+80),(PROM_START[0]+80,PROM_START[1]+160)) and checkInside(locfinal,(PROM_START[0],PROM_START[1]+80),(PROM_START[0]+80,PROM_START[1]+160))):
                        board.Promote(prom_sqr,1,prom)
                        MOVE_SOUND.play()
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        
                        draw_main(chess_board,locfinal,locinitial,0,player,prom,moveset)
                        moves=extractlist(list(board.genMovesForEachPiece(-player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(-player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        print(moves)
                        best_move=list(EvaluateBoard(board,-player))
                        print(best_move)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        pr=board.ChangeBoard(best_move[0],best_move[1])
                        if(pr!=0):
                            board.Promote(best_move[1],best_move[2],plr)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        moveset=extractlist(list(board.genMovesForEachPiece(player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        plr=-plr
                        peice=0
                        prom=0;
                        prom_sqr=(65,65)
                    elif(checkInside(locinitial,(PROM_START[0]+80,PROM_START[1]+80),(PROM_START[0]+160,PROM_START[1]+160)) and checkInside(locfinal,(PROM_START[0]+80,PROM_START[1]+80),(PROM_START[0]+160,PROM_START[1]+160))):
                        board.Promote(prom_sqr,3,prom)
                        MOVE_SOUND.play()
                        
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        plr=-plr
                        draw_main(chess_board,locfinal,locinitial,0,player,prom,moveset)
                        best_move=list(EvaluateBoard(board,-player))
                        moves=extractlist(list(board.genMovesForEachPiece(-player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(-player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        print(moves)
                        print(best_move)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        pr=board.ChangeBoard(best_move[0],best_move[1])
                        if(pr!=0):
                            board.Promote(best_move[1],best_move[2],plr)
                        chess_board=list(board.board)
                        chess_board=np.reshape(chess_board,(8,8))
                        draw_main(chess_board,locfinal,locinitial,0,player,prom,moveset)
                        moveset=extractlist(list(board.genMovesForEachPiece(player)))
                        if(len(moveset)==0):
                            if(board.IsCheck(player)==0):
                                print("Stalemate")
                            else:
                                print("Checkmate")
                            break
                        plr=-plr
                        peice=0
                        prom=0;
                        prom_sqr=(65,65)
                    
                    
                   
        if drag==True:
            locfinal=pygame.mouse.get_pos()
        
        draw_main(chess_board,locfinal,locinitial,peice,player,prom,moveset)            
        

    pygame.quit()
if __name__== "__main__":
    main()

