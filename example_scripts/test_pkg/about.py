""" Module managing the Author for all the  modules implemented"""

dict_modules = {"RNWF02":["Irfana", "Irfana@microchip.com"],
                "RNBD451":["Akhil", "Akhil@microchip.com"],
                "SAME54": ["Santhosh", "Santhosh@microchip.com"],
                "IOT": ["Amrita", "Amrita@microchip.com"],
                 "POST": ["Santhosh", "Santhosh@microchip.com"],
                }
""" Enum usage not supported, any feature of Python which requires importing
a library is by default not supported with MicroPython"""

AUTHOR=0
EMAIL=1


def get_author(module_name):
    global dict_modules
    list_author = "Undefined"
    if module_name in dict_modules.keys():
        list_author = dict_modules[module_name][AUTHOR]
    return list_author

def get_email(module_name):
    global dict_modules
    list_author = "Undefined"
    if module_name in dict_modules.keys():
        list_author = dict_modules[module_name][EMAIL]
    return list_author
