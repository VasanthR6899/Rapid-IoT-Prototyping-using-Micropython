"""This Python script is for blinking the LED0 on the SAME54 XPRO Board
using a timer and the user button (SW0) is used to stop the blinking"""

#Importing classes(Pin and Timer) from the umachine module
from umachine import Pin                           
from umachine import Timer  

WELCOME_MESSAGE="\r\nProgram to toggle the onboard LED0\r\n"
EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue\r\n"

SWITCH_PRESSED=0
SWITCH_RELEASE=1
# Initializing the count for LED toggling
cnt = 0    

#Initializing the pin objects - led for LED, switch for User button using Pin method 
led = Pin(18) 
switch = Pin(31)  

#Defining the timer callback to toggle the LED with 70% duty cycle and time period of 1 second based on count value
#timer callback is called every 100ms and the cnt value is checked accordingly for turning ON/OFF the LED

def mycallback(t):
    global cnt                          
    if (cnt<7) and led.value():                   # led.value() is checked here to avoid unnecessary writing to the pin         
        led.off()                                 # led is turned ON for the first 700ms
    elif (cnt>7) and not(led.value()):            # led.value() is checked here to avoid unnecessary writing to the pin
        led.on()                                  # led is turned OFF for the next 300ms
    cnt = (cnt + 1)%11                            # count is re-initialized to 0 since 1 second is over on count of 10 (10*100ms = 1s)
        
        
#Definition of main 
def main():
    print(WELCOME_MESSAGE)
    tim = Timer(0)                                #Initializing the timer object
    print("Press the push button (SW0) on the board to stop the LED toggle\r\n")
    # Initialize led with default value of 0 which will turn on the LED
    led.off()
    tim.init(period=100000, callback=mycallback)  #Initializing the timer with period value of 100ms(Since timer tick is 1us) and the modified callback method   
    while True:                                   
        if switch.value() == SWITCH_PRESSED:      #Checking if the user button is pressed
            print("\r\nPush button is pressed to stop the LED Toggle.. ")
            tim.deinit()                          #De-initializing the timer, so that we could use the timer for any other purpose           
            break                                  
    print(EXIT_MESSAGE)
   

main()



