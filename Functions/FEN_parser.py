
import numpy as np
def fen_code_parser(fen_code: str) -> np.ndarray:
    pieces= {'k' : 1, 'p' : 2, 'b' : 3, 'n' : 4, 'q' : 5, 'r' : 6}
    board_size = 8
    
    chess_board = np.full((board_size, board_size), 0)
    
    row = 0
    col = 0
    
    for char in fen_code:
        if(char == " "):
            break
        
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
    
    return chess_board