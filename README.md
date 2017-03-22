# simpleSerialArduinoWrapper
Simple pyserial-based python class to send messages to Arduino via serial port

-use python3, ideally in virtual environment:

virtualenv -p /usr/bin/python3 venv

source venv/bin/activate

-install pyserial (if not already done):

pip install pyserial

-check out package
git clone https://github.com/kirbybri/simpleSerialArduinoWrapper

-upload simpleSerialArduino.ino to Arduino

-test interface after arduino has been programmed:

python simpleSerialMessage.py
