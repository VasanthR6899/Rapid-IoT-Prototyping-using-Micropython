
"""This Python script is for configuring the RNWF11 WiFi module as WiFi station using UART and connect it to a WiFi access point""" 

#Importing classes(I2C and ADC) from the umachine module
from umachine import UART

WELCOME_MESSAGE="\r\nProgram to Query RNWF11 and connect to an AP \r\n"
EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue\r\n"
TRUE=1
FALSE=0
LINE_FEED='\r\n'
STRING_LINE_FEED="\\r\\n"
TEST_OKAY = "OK"
MAX_SIZE_UART_BUFFER=256
SSID="DEMO_APMchp"          #WiFi SSID
PASSWORD="password"         #WiFi Password
SNTPC_URL="0.in.pool.ntp.org"
READ_WAIT_DELAY=40

list_WiFITest = [
    "AT+SNTPC=2,1" + LINE_FEED,                                 #NTP configuration mode
    "AT+SNTPC=3," + "\"" + SNTPC_URL + "\"" + LINE_FEED,        #NTP server configuration
    "AT+SNTPC=1,1" + LINE_FEED,                                 #NTP enable
    "AT+WSTAC=1," + "\"" + SSID + "\"" + LINE_FEED,             #Set WiFi SSID
    "AT+WSTAC=3," +"\"" + PASSWORD + "\"" + LINE_FEED,          #Set WiFi Password
    "AT+WSTAC=2,2" + LINE_FEED,                                 #Set WiFi security type (WPA2)
    "AT+WSTA=1" + LINE_FEED]                                    #Enable WiFi station configuration
    

def main():
    print(WELCOME_MESSAGE)
    wifiModule = cRNWF11()
    result = FALSE
    result = wifiModule.testRST()
    result = result and wifiModule.testGMM()
    result = result and wifiModule.testWiFi(list_WiFITest)
    print(EXIT_MESSAGE)

    return

class cRNWF11:
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
        print("Resetting RNWF11, this may take some time.. please wait")
        self.flush()
        self.uart.write("AT+RST" + LINE_FEED)
        for i in range(6):
            self.flush()
        print("Resetting RNWF11 complete")
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
    
    def testGMM(self):
        self.uart.write("AT+GMM" + LINE_FEED)
        resultGMM=self.__receive()
        if(resultGMM == FALSE):
            print("Device did not return right data")
        return resultGMM
 

main()

