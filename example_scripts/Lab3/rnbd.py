from umachine import I2C, ADC, UART
import utime

# Constants
TRUE = 1
FALSE = 0
MAX_SIZE_UART_BUFFER = 256
TEST_OKAY = "AOK"


class RNBD451:
    def __init__(self):
        # Initialize UART with a specified timeout
        self.uart = UART(5, timeout=1000)
        self.i2c = I2C(1)  # Initializing an I2C object with I2C on EXT1
        self.app_state = None  # Initialize application state
        self.state_callbacks = {
            "rnbd_init": self.on_init,
            "rnbd_command": self.on_command,
            "awaiting_connection": self.on_awaiting_connection,
            "rnbd_connected": self.on_connected
        }

        print("Initializing RNBD451...")
        self.set_app_state("rnbd_init")  # Start in the initialization state

        # Test reset and enter command mode
        if self.test_rst():
            self.flush()
            if self.enter_command_mode():
                print("Successfully entered command mode.")
                utime.sleep(2)
                self.set_app_state("rnbd_command")
            else:
                print("Failed to enter command mode.")
        else:
            print("Device reset failed.")

    def set_app_state(self, new_state):
        """Set and transition to a new application state."""
        if new_state != self.app_state:
            self.app_state = new_state
            if new_state in self.state_callbacks:
                self.state_callbacks[new_state]()  # Call the state-specific callback

    # State callback methods
    def on_init(self):
        print("rnbd_init")

    def on_command(self):
        print("rnbd_command")
        self.uart.write('A\r')
        utime.sleep(2)
        is_ok = self.receive()
        print(is_ok)
        if is_ok:
            self.set_app_state("awaiting_connection")
        else:
            return

    def on_awaiting_connection(self):
        print("awaiting_connection")
        # Wait till timer expires, if connected with the timer, move to connection state or return
        temp = self.wait_for_response(10, "%CONNECT")
        if temp:
            self.set_app_state("rnbd_connected")
        else:
            return

    def on_connected(self):
        print("rnbd_connected")
        while TRUE:
            utime.sleep(2)
            temp = str(self.readTemperature())  # Corrected to call class method
            self.uart.write(temp)
            test = self.uart.read(MAX_SIZE_UART_BUFFER)
            # Check if the response contains "%DISCONNECT"
            if "%DISCONNECT" in test:
                print("Disconnected")
                return

    # RNBD451-specific methods
    def test_rst(self):
        """Reset the RNBD451 device."""
        print("Resetting")
        self.flush()
        self.uart.write("r,1\r")
        for _ in range(6):
            self.flush()
        print("Reset complete")
        return TRUE

    def enter_command_mode(self):
        """Enter command mode on the RNBD451."""
        self.uart.write("$$$")
        return self.wait_for_response(5, "CMD>")

    def flush(self):
        """Flush the UART buffer by reading it."""
        self.uart.read(MAX_SIZE_UART_BUFFER)

    def receive(self):
        """Receive and process data from UART."""
        temp = self.uart.read(MAX_SIZE_UART_BUFFER)
        if temp:
            if "ERR" in temp or "FAIL" in temp:
                return FALSE
            elif TEST_OKAY in temp:
                print(TEST_OKAY)
                return TRUE
        return FALSE

    def wait_for_response(self, timeout, expected_response):
        for _ in range(timeout):
            temp = self.uart.read(MAX_SIZE_UART_BUFFER)
            if temp:
                if expected_response in temp:
                    return TRUE
                if "ERR" in temp or "FAIL" in temp:
                    return FALSE
        return FALSE

    def readTemperature(self):
        """Read temperature from I2C sensor."""
        a = self.i2c.readfrom_mem(0x4F, 0x00, 2)  # Storing the result from the temperature sensor
        b = list(a)  # Convert byte data to a list
        temp = (b[0] << 8) | b[1]  # Combine the two bytes into one temperature value
        tempc = ((temp >> 7) // 2)  # Adjust based on sensor-specific calculations
        tempf = (tempc * 9 // 5) + 32  # Convert to Fahrenheit
        return tempf


def main():
    """Main function to run the RNBD451 example."""
    rn = RNBD451()  # Initialize the RNBD451 class
    print("exit")


main()
