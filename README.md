# Winpower_3FS
this stuff was made to use with ESP-32 and oxygen analyzer winpoer 3FS, bu i have not used any hardware-specific code, i guess, so it should work fine not only on intrnded platform.

# How to use:

Create an Winpower_3FS object:
```
    Winpower_3FS winpower_3FS(); // need to be initialized after

    Winpower_3FS winpower_3FS(&Serial2); //without changing pins
    Winpower_3FS winpower_3FS($Serial2, RX_pin, TX_pin); //change pins 
```

Initialize it, if it is not yet: 
```
    winpower_3FS.init(&Serial2);
    winpower_3FS.init(&Serial2, RX_pin, TX_pin);
```

Send command to 3FS to start it doing smthing:
```
    winpower_3FS.sendCommand(GEAR, MODE, ABC1, SENSIVITY);
```

Update recieved data :
```
    winpower_3FS.updateData(true); // it will wait until enough data recieved and block the code

    error = winpower_3FS.updateData(); //it wil return 2  if it is not enough data yet or 1 if packet is not valid, so it is your responsibility to keep track of the data is received.
```

Get recieved data using getters as a structure or standalone:
```
    Data3FS getData();
    
    uint16_t getOxygenConcentration();
    uint8_t getGear();
    uint8_t getMode();
    uint8_t getFlowRate();
    uint8_t getTemperature();
    uint8_t getRespiratorySign();
    uint8_t getRespiratoryRate();
    uint8_t getAbc();
    uint8_t getSensivity();
```
