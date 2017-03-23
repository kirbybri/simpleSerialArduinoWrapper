from simpleSerialMessage import simpleSerialMessage 

if __name__ == '__main__':
  myser = simpleSerialMessage()
  myser.getReply = 0
  #myser.sendMessage("<BWD,1000,2>")
  while 1:
    var = input("Please enter message, FORMAT = <VAR1,VAR2,...>, enter q to exit : ")
    if str(var) == "Q" or str(var) == "q" or str(var) == "Quit" or str(var) == "QUIT" or str(var) == "quit" :
      break
    print( "Message ", str(var) )
    myser.sendMessage( str(var) )
