
import cppyy

def CompileClass():
    move_file = open("Functions/move_generator.cpp", "r")
    lines_list = move_file.readlines()
    
    move_generator = "" 
    for line in lines_list:
        move_generator += line
        
    
    cppyy.cppdef(move_generator)