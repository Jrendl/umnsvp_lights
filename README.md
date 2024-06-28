# UMNSVP Lights Control Firmware

This contains the Code for the lights control firmware where I left it when I left the team.


# F1.50 Lights # 

## Description ##
This directory is to provide a simple implentation of the F1.50 Lights board.

## Pin Mapping- corresponds to channels on the lights board ##

Mappings can be changed via the Application Constructor in the ```app.cc``` file.

*Front Board:
**Headlights - Ch0
**High Beams - Ch1
**Left Turn - Ch2
**Right Turn - Ch3
**Camera - Ch5

**Ch4 is unused

*Back Board
**Brake Lights - Ch0
**Left Turn - Ch1
**Right Turn - Ch2
**Blinky Light - Ch3
**Camera - Ch5

**Ch4 is unused

## Light Class ##

This class provides wrapper functions for easey use of the light channels on the F1.50 Lights board. It can be initialized as follows:

```Light light_0 = Light(PORT_LIGHT_0, PIN_LIGHT_0);```

Calling ```light_0.init();``` will set up the necessary hardware interfaces for correct interaction. 
