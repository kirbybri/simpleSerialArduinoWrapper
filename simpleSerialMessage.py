import serial
import time
import os.path

class simpleSerialMessage():

  def __init__(self):
    print("Initializing serial interface")

    if os.path.exists("/dev/ttyACM0") == False:
       print("Could not find serial interface")
       self.ser = None
       return

    self.ser = serial.Serial("/dev/ttyACM0", 
       baudrate=9600,
       bytesize=serial.EIGHTBITS,
       parity=serial.PARITY_NONE,
       stopbits=serial.STOPBITS_ONE,
       timeout=3,
       xonxoff=0,
       rtscts=0)

    # Toggle DTR to reset
    self.ser.setDTR(False)
    time.sleep(1)
    self.ser.flushInput()
    self.ser.setDTR(True)
    time.sleep(2)

    self.interfaceUp = 0
    self.startMarker = 60
    self.endMarker = 62
    self.timeoutCount = 1000
    self.byteCount = -1
    self.getReply = 1
    self.responseDelay = 3
    self.checkInterface()

  def __del__(self):
    if self.ser == None:
      return
    self.ser.close

  def checkInterface(self):
    if self.ser == None:
      return
    print("Checking if serial interface is up")
    if self.ser.is_open == 0 :
      print("Interface not working")
      return
    self.flushSerialData()
    self.sendToSerial("<TEST,0,0>")
    time.sleep(self.responseDelay)
    response = self.recvFromSerial()
    if response == None:
      print("Serial interface not working")
      return
    if len(response) == 0:
      print("Serial interface not working")
      return
    self.interfaceUp = 1
    print("Serial interface is working")

  def flushSerialData(self):
    if self.ser == None:
      return
    #print("Clearing serial read buffer")
    #self.ser.reset_output_buffer()
    #self.ser.reset_input_buffer()
    count = 0
    while self.ser.in_waiting != 0 :
      self.ser.read()
      count = count + 1
      if count >= self.timeoutCount:
        break

  def sendToSerial(self,sendStr):
    if self.ser == None:
      return
    self.ser.write( sendStr.encode() )

  def recvFromSerial(self):
    if self.ser == None:
      return
    #while self.ser.inWaiting() != 0 :
    #  test = self.ser.read()
    #  print( str(test) )
    #return

    ck = "" #initialize response string
    x = "z" # any value that is not an end- or startMarker
    self.byteCount = -1 # to allow for the fact that the last increment will be one too many
  
    # wait for the start character
    count = 0
    while  ord(x) != self.startMarker: 
      x = self.ser.read()
      if x == None:
        break
      if len(x) == 0:
        break
      count = count + 1
      if count >= self.timeoutCount:
        break

    if x == None:
      return None
    if len(x) == 0:
      return None

    # save data until the end marker is found
    count = 0
    while ord(x) != self.endMarker:
      if ord(x) != self.startMarker:
        xstr = x.strip().decode('utf-8')
        ck = ck + xstr
        self.byteCount += 1
      x = self.ser.read()
      if x == None:
        break
      if len(x) == 0:
        break
      count = count + 1
      if count >= self.timeoutCount:
        break

    if x == None:
      return None
    if len(x) == 0:
      return None
    return(ck)

  def sendMessage(self, messageStr):
    if self.ser == None:
      return
    print( "Sent from PC -- " + str(messageStr) )
    #self.flushSerialData()
    self.sendToSerial(messageStr)
    time.sleep(1)
    if self.getReply == 0 :
      return
    time.sleep(self.responseDelay)
    if self.ser.inWaiting() != 0 :
      dataRecvd = self.recvFromSerial()
      print( "Reply received  " + str(dataRecvd) )
    else:
      print( "No reply!" )

if __name__ == '__main__':
  myser = simpleSerialMessage()
  #myser.flushSerialData()
  myser.sendMessage("<LED,210,0.2>")
  myser.sendMessage("<TEST,210,0.2>")
