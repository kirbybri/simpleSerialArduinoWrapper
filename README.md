# simpleSerialArduinoWrapper
Simple pyserial-based data interface wrapper to Arduino

-use python3, ideally in virtualenv
virtualenv -p /usr/bin/python3 venv
source venv/bin/activate

-install pyserial (if not already done)
pip install pyserial

-test interface after arduino has been programmed
python simpleSerialMessage.py
