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
        self.app_state = None  # Initialize application state
        self.state_callbacks = {
            "rnbd_init": self.on_init,
            "rnbd_command": self.on_command,
            "awaiting_connection": self.on_awaiting_connection,
            "rnbd_connected": self.on_connected,
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
            #print("connected")
        else:
            return

    def on_connected(self):
        print("rnbd_connected")

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

    def __wait(self, number, string_to_wait):
        result = FALSE
        for _ in range(number):
            temp = self.uart.read(MAX_SIZE_UART_BUFFER)
            if temp:
                if string_to_wait in temp:
                    result = TRUE
                    break
                if "ERR" in temp or "FAIL" in temp:
                    break
        return result

    def wait_for_response(self, timeout, expected_response):
        for _ in range(timeout):
            temp = self.uart.read(MAX_SIZE_UART_BUFFER)
            if temp:
                if expected_response in temp:
                    return TRUE
                if "ERR" in temp or "FAIL" in temp:
                    return FALSE
        return FALSE


def main():
    print("**************************************************")
    print("                   RNBD451 Demo")
    print("**************************************************\r\n")

    rn = RNBD451()  # Initialize the RNBD451 class


main()
