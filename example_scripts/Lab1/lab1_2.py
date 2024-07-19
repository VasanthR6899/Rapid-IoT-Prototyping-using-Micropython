"""This Python script is for blinking the LED0 on the SAME54 XPRO Board
using a timer and the user button (SW0) is used to stop the blinking"""

#Importing classes(Pin and Timer) from the umachine module
from umachine import Pin                           
from umachine import Timer  

WELCOME_MESSAGE="\r\nProgram to toggle the onboard LED0 every 2 seconds\r\n"
EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue\r\n"

SWITCH_PRESSED=0
SWITCH_RELEASE=1
# Initializing the count for LED toggling
cnt = 0    

#Initializing the pins - led for LED, switchfor User button using Pin method -> These are initialized to objects.
led = Pin(18) 
switch = Pin(31)  

#Defining the timer callback to toggle the LED based on count value


def mycallback(t):
    global cnt
                                
    if cnt % 2 != 0:        
        led.on()
    else:        
        led.off()
    cnt = cnt + 1
        
#Definition of main 
def main():
    print(WELCOME_MESSAGE)
    tim = Timer(0)                                 #Initializing the timer object for TC0 
    print("Press the push button (SW0) on the board to stop the LED toggle\r\n")
    # Initialize led with default value of ON
    led.off()
    tim.init(period=2000000, callback=mycallback)  #Initializing the timer with period value and callback method.(Note: Timer(TC0) initialized in application have a timer tick period of 1us)  
    while True:                                   
        if switch.value() == SWITCH_PRESSED:       #Checking if the user button is pressed
            print("\r\nPush button is pressed to stop the LED Toggle.. ")
            tim.deinit()                           #De-initializing the timer, so that we could use the timer for any other purpose           
            break                                  
    print(EXIT_MESSAGE)


main()

#To Do : Change the timer configuration and callback method to perform LED toggle with duty cycle of 70% and time period of 1 second. Refer to the lab manual (section) for details.


