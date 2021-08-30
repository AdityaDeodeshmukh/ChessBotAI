
import numpy as np
from numpy.core.numeric import full
def fen_code_parser(fen_code):
    x=0
    half_move=0
    flag=False
    full_move=0
    mult=1
    pieces= {'k' : 1, 'p' : 2, 'b' : 3, 'n' : 4, 'q' : 5, 'r' : 6}
    board_size = 8
    list1=['k','q','K','Q']
    map={'q':(0,0,-1),'k':(0,7,-1),'K':(7,7,1),'Q':(7,0,1)}    
    chess_board = np.full((board_size, board_size), 0)
    
    row = 0
    col = 0
        
    for char in fen_code:
        if(x==0):
            if(char == " "):
                x=x+1
                continue
            
            if char.isdigit():
                col += int(char)
            
            elif char == "/":
                row += 1
                col = 0
            
            else:
                if char.isupper():
                    chess_board[row][col] = pieces[char.lower()]
                else:
                    chess_board[row][col] = -pieces[char]
                col += 1
        if(x==1):
            if(char=="w"):
                plr=1
            if(char=="b"):
                plr=-1
            if(char==" "):
                x=x+1
                continue
        if(x==2):
            if char in list1:
                row=map[char][0]
                col=map[char][1]
                friendly=map[char][2]
                chess_board[row][col]=friendly*7
            if(char== " "):
                x=x+1
                continue
        if(x==3):
            if char.islower():
                col=ord(char)-ord('a')
            if char.isdigit():
                row=int(char)-1
                flag=True
            if(char==" "):
                x=x+1
                if flag:
                    chess_board[7-row][col]=-plr*9
                continue
        if(x==4):
            if char.isdigit():
                half_move=half_move*mult+int(char)
            if char==" ":
                x=x+1
                continue
        if(x==5):
            if char.isdigit():
                full_move=full_move*mult+int(char)
            if char==" ":
                x=x+1
                continue
    return chess_board,plr,half_move,full_move;            



    
