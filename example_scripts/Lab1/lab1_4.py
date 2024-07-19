""" This lab helps user understand the features of python"""
""" We are importing three modules and show case different features"""
""" Post is a base class implementation of a specific functionality and this lab uses that feature
of the base Post class"""
from test_pkg import post
from test_pkg import about
from test_pkg import log
from test import test1,test2

WELCOME_MESSAGE="\r\nProgram to showcase the features of classes and test module\r\n"
EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue\r\n"

def main():
    print(WELCOME_MESSAGE)

    p = post.Post()
    p.add_post("Using Device")
    Log = log.Log(print,"Lab1")

    # About is an independent module, which shows author responsible
    author = about.get_author("RNWF02")
    email = about.get_email("IOT")


    Log.printlog(p.get_post())
    Log.printlog("Testing log message")
    Log.printlog(author)  
    Log.printlog(email)
    
    test1() # calls C code from Python which enables UART debug log and LED toggle info
    #test2() # calls C code from Python which disables UART debug log and LED toggle info
    print(EXIT_MESSAGE)
    
main()