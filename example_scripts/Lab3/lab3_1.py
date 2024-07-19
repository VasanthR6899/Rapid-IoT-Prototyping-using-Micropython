
"""This Python script is for reading the temperature sensor value and light sensor value on the 
IO1 Xplained pro connected to the EXT1 of the SAME54 XPRO Board"""
#Importing classes(I2C and ADC) from the umachine module
from umachine import I2C
from umachine import ADC
from umachine import UART
WELCOME_MESSAGE="\r\nProgram to test IOT Dashboard and set rules for sending data to cloud \r\n"
EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue\r\n"
TRUE=1
FALSE=0
LINE_FEED='\r\n'
STRING_LINE_FEED="\\r\\n"
TEST_OKAY = "OK"
MAX_SIZE_UART_BUFFER=256

# Below are configuration for WiFi and Network connectivity
SSID="DEMO_APMchp"
PASSWORD="password"
SNTPC_URL="0.in.pool.ntp.org"

# Below are configuration for Security credentials and IOT Thing configuration
AMAZON_ROOT_CA = "AmazonRootCA1"
CA_CERT_1 = "RNWF_149"
CA_CERT_2 = "RNWF_149"
THING_NAME="RNWF_149"
THING_URL="a2uv9ort04b5z3-ats.iot.us-east-2.amazonaws.com"

# MQTT Connection Specific configuration
MQTT_PORT="8883"
MQTT_DELAY="120"

# MQTT PUBLISH and SUBSCRIBE Configuration

DEVICE_TOPIC_PUBLISH = "mchp-iot3/deviceupdate/RNWF_149"  # Device is publishing this topic
DEVICE_TOPIC_SUBSCRIBE = "mchp-iot3/update/RNWF_149" # From AWS Console, this topic needs to be published!!!
MESSAGE_PUBLISH_STRING = "Hello Microchip "

READ_WAIT_DELAY=40 # Max of 40 second delay
PRINT_MQTT_PUBLISH_COUNT=15 # Number of messages to publish

"""Need to add comments to lab modules"""
strIOTString = "\\\"temperature\\\" : {} ,  \\\"light\\\" : {} "



list_WiFITest = [
    "AT+SNTPC=2,1" + LINE_FEED,
    "AT+SNTPC=3," + "\"" + SNTPC_URL + "\"" + LINE_FEED,
    "AT+SNTPC=1,1" + LINE_FEED,
    "AT+WSTAC=1," + "\"" + SSID + "\"" + LINE_FEED,
    "AT+WSTAC=3," +"\"" + PASSWORD + "\"" + LINE_FEED,
    "AT+WSTAC=2,2" + LINE_FEED,
    "AT+WSTA=1" + LINE_FEED]
    

list_MQTTTest = [
    "AT+MQTTC=8,3" +  LINE_FEED,
    "AT+TLSC=2,1," + "\"" + AMAZON_ROOT_CA  + "\"" + LINE_FEED,
    "AT+TLSC=2,2," + "\"" + CA_CERT_1  + "\"" + LINE_FEED,
    "AT+TLSC=2,3," + "\"" + CA_CERT_2  + "\"" + LINE_FEED,
    "AT+MQTTC=7,2" +  LINE_FEED,
    "AT+MQTTC=1," + "\"" + THING_URL  + "\"" + LINE_FEED,
    "AT+MQTTC=3," + "\"" + THING_NAME  + "\"" + LINE_FEED,
    "AT+MQTTC=2," + MQTT_PORT  + LINE_FEED,
    "AT+MQTTC=6," + MQTT_DELAY  + LINE_FEED,
    "AT+MQTTCONN=1"   + LINE_FEED]
    

def main():
    print(WELCOME_MESSAGE)
    wifiModule = cRNWF02()
    result = FALSE
    result = wifiModule.testRST()
    result = wifiModule.testWiFi(list_WiFITest)
    result = result and wifiModule.testMQTT()
    print(EXIT_MESSAGE)
    return

class cRNWF02:
    def __init__(self):
        self.uart=UART(5,timeout=1000)
        return

    def __wait(self,number,stringtowait):
        result = FALSE
        for i in range(number):
            temp = str(self.uart.read(MAX_SIZE_UART_BUFFER), 'UTF-8')
            if(temp !=''):
                print(temp)
            if(stringtowait in temp):
                result = TRUE
                break
            if("ERR" in temp):
                break
            if("FAIL" in temp):
                break
        return result

    def __receive(self):
        received = FALSE 
        try:
           temp = str(self.uart.read(MAX_SIZE_UART_BUFFER), 'UTF-8')
           if("ERR" in  temp):
               received = FALSE
               print("Error received" + temp)
           if("FAIL" in  temp):
               received = FALSE
               print("Error received" + temp)
           else:
               if(TEST_OKAY in temp):
                   received = TRUE
                   print(TEST_OKAY)
                
        except:
            pass
        
        return received

    def testRST(self):
        print("Resetting RNWF02, this may take some time.. please wait")
        self.flush()
        self.uart.write("AT+RST" + LINE_FEED)
        for i in range(6):
            self.flush()
        print("Resetting RNWF02 complete")
        return TRUE

    def testMQTT(self):
        global list_MQTTTest
        for atCommand in list_MQTTTest:
            self.uart.write(atCommand)
            print(atCommand)
            resultWiFi = self.__receive()
        result = self.__wait(30,"CONN:1")
        if (result == FALSE):
            print("MQTT Connection Failed")
            return result
        print("MQTT Connection Success")
        self.uart.write("AT+MQTTSUB="  +  "\"" + DEVICE_TOPIC_SUBSCRIBE + "\"" + "," + "0"  + LINE_FEED )
        for i in range(PRINT_MQTT_PUBLISH_COUNT):
            self.__wait(2,"+MQTTSUB:")
            strNewIOTString = str(strIOTString)
            strNewIOTString = strNewIOTString.format(readTemperature(),readLightValue())
            strNewIOTString = "{" + strNewIOTString +"}"
            self.uart.write("AT+MQTTPUB=0,0,0,"  +  "\"" + DEVICE_TOPIC_PUBLISH + "\"" + "," + "\""  + strNewIOTString + "\""  + LINE_FEED ) 
        return TRUE

    def testWiFi(self,listCommand):
        resultWiFi=FALSE
        for atCommand in listCommand:
            self.uart.write(atCommand)
            print(atCommand)
            resultWiFi = self.__receive()
        resultWiFi = self.__wait(READ_WAIT_DELAY,"TIME")
        if(resultWiFi == FALSE):
            print("Issues in WiFi / Network Connectivity, Please retry")
        else:
            print("Network Connectivity is Successful with " + SSID)
        return resultWiFi

    def flush(self):
        test=self.uart.read(MAX_SIZE_UART_BUFFER)
        return



#Reading Temperature data from the temperature sensor in IO1 Xplained Pro using I2C

def readTemperature():
    i2c=I2C(1)                                             # Initializing an I2C object with I2C on EXT1
    a = i2c.readfrom_mem(0x4F, 0x00,2)                     # storing the result from the temperature sensor to an object 'a'
    b = list(a)                                            # Taking out the values alone from the bytes format and storing it as a list 'b'
    temp = (b[0]<<8) | b[1]                                # Manipulating the decimal values to get the Temperature value 
    tempc = ((temp>>7)//2)
    tempf = (tempc * 9//5) + 32
    return tempf


#Reading Light sensor data from IO1 Xplained Pro using ADC
def readLightValue():
    adc = ADC(1)                                          # Initializing an ADC object with ADC pin available on EXT1
    vol_in_mv = adc.read_uv() //1000                             # Reading the ADC voltage value which is in mV using a python method
    print("\nInput Voltage in mV    -",vol_in_mv,"mV")
    brightness = ((3300 - vol_in_mv) * 100)//3300;         # Calculting the brightness of the light detected by the Light sensor
    print("Brightness percentage  - ",brightness,"%")
    return brightness
    

main()

