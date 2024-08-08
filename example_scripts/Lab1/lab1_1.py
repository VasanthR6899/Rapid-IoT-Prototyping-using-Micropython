"""This Python script is for showcasing basic Micropython functionalities"""

EXIT_MESSAGE="\r\nExiting the application. Press Enter to continue"

#Basic addition is done in this method                         
def add_two_numbers(num1,num2):    
    print(num1+num2)                                                                        #Adding two numbers
    return

#Definition of method which converts byte data to string
def convert_bytes_to_string():    
    bytes_data = b'Hello, Python\r\n Microchip\r\n OK!'
    print("\r\nUsing str method we have converted the following bytes data to string\r\n")
    print(bytes_data)
    string_data = str(bytes_data, 'UTF-8')                                                  #Using str() method which converts the bytes data to string
    print("\r\n" + string_data)
    
#Definition of method which forms a list from the digits of a number    
def form_list_from_digits(num):
    num_list =  [int(digit) for digit in str(num)]                                          #Using for loop to form a list
    return num_list

def add_two_big_numbers(num1, num2):
  """
  Adds two big numbers represented as lists of digits.

  Args:
    num1: A list of digits representing the first number.
    num2: A list of digits representing the second number.

  Returns:
    A list of digits representing the sum of the two numbers.
  """

  # Make sure the numbers are the same length.
  if len(num1) < len(num2):
    num1 = [0] * (len(num2) - len(num1)) + num1
  elif len(num2) < len(num1):
    num2 = [0] * (len(num1) - len(num2)) + num2

  # Add the two numbers, digit by digit.
  carry = 0
  result = []
  for i in range(len(num1) - 1, -1, -1):
    digit_sum = num1[i] + num2[i] + carry
    carry = digit_sum // 10
    result.append(digit_sum % 10)

  # Add the carry digit, if necessary.
  if carry > 0:
    result.append(carry)

  # Reverse the result list, so that the digits are in the correct order.
  result.reverse()

  return result

# Write the method for subtraction here
"""

"""

def main():
    print("\r\nAdding two numbers 4 + 5 = ")
    add_two_numbers(4,5)

    print("\r\nConsider two big numbers of same size, for example 123456789 and 987654321. Let's add this in Micropython")
    num1_list = form_list_from_digits("123456789")
    num2_list = form_list_from_digits("987654321")
    
    result = add_two_big_numbers(num1_list, num2_list)
    print("\r\nAddition Result = " + str(result))

    convert_bytes_to_string()
    
    #To Do : Write a method to subtract two numbers
    
    # call the method here to perform subtraction and print the result

    print(EXIT_MESSAGE)

main()