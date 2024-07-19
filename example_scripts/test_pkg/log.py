""" This class show cases the power of modules
    For example, if user passes print, it prints on console
    if users passes a corresponding message, it can send this message over MQTT
    as well.
"""

class Log:

    def __init__(self, func,module):
        self._func = func
        self._module = module

    def printlog(self, message):
        # This implementation forces the user to use the Post class as a list of string only
        self._func(str(self._module) +  ": " +  str (message))

