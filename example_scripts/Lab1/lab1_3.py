"""This Python script is for reading the temperature sensor value and light sensor value on the 
IO1 Xplained pro connected to the EXT1 of the SAME54 XPRO Board"""
#Importing classes(I2C and ADC) from the umachine module
from umachine import I2C
from umachine import ADC

WELCOME_MESSAGE="\r\nProgram to Read Temperature in Farenheit and Brightness in %\r\n"
EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue\r\n"
INPUT_VOLTAGE=3300
RANGE_VDARK_LIMIT_LOW = 0
RANGE_VDARK_LIMIT_HIGH = 25
RANGE_DARK_LIMIT_LOW = 26
RANGE_DARK_LIMIT_HIGH = 50
RANGE_BRIGHT_LIMIT_LOW = 51
RANGE_BRIGHT_LIMIT_HIGH = 75
RANGE_VBRIGHT_LIMIT_LOW = 76
RANGE_VBRIGHT_LIMIT_HIGH = 100
#Reading Temperature data from the temperature sensor in IO1 Xplained Pro using I2C

def readTemperature():
    i2c=I2C(1)                                             # Initializing an I2C object with I2C on EXT1
    a = i2c.readfrom_mem(0x4F, 0x00,2)                     # storing the result from the temperature sensor to an object 'a'
    b = list(a)                                            # Taking out the values alone from the bytes format and storing it as a list 'b'
    temp = (b[0]<<8) | b[1]                                # Manipulating the decimal values to get the Temperature value 
    tempc = ((temp>>7)//2)
    tempf=0
    tempf = (tempc * 9//5) + 32;
   
    return tempf

#Reading Light sensor data from IO1 Xplained Pro using ADC

def readLightBrightness():
    adc = ADC(1)                                          # Initializing an ADC object with ADC pin available on EXT1
    vol_in_mv = adc.read_uv() // 1000                             # Reading the ADC voltage value which is in mV using a python method
    print("\nInput Voltage in mV    -",vol_in_mv,"mV")
    brightness = ((INPUT_VOLTAGE - vol_in_mv) * 100)//INPUT_VOLTAGE         # Calculting the brightness of the light detected by the Light sensor
    print("Brightness percentage  - ",brightness,"%")
    return brightness

# Write the method here for coverting temperature from Fahrenheit to Celsius

"""



"""                                             
def main():
    print(WELCOME_MESSAGE)
    tempf = readTemperature()
    print("Temperature value in Fahrenheit -", tempf, "F" )
    brightness = readLightBrightness()
   
    if brightness in range (RANGE_VDARK_LIMIT_LOW, RANGE_VDARK_LIMIT_HIGH):             # Categorising the brightness levels based on %
        print("Very dark")
    elif brightness in range (RANGE_DARK_LIMIT_LOW, RANGE_DARK_LIMIT_HIGH):
        print("Dark")
    elif brightness in range (RANGE_BRIGHT_LIMIT_LOW, RANGE_BRIGHT_LIMIT_HIGH):
        print("Bright")
    elif brightness in range (RANGE_VBRIGHT_LIMIT_LOW, RANGE_VBRIGHT_LIMIT_HIGH):
        print("Very Bright")
    
    
    #To Do : Write a method to covert temperature from Fahrenheit to Celsius
                                                                                       
    #Call the method here for temperature conversion and print the value with unit

    print(EXIT_MESSAGE)
    
main()
