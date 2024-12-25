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
