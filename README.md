
# Introduction

I had had a Lora igate active for some time, but felt that the data from other igates was shown faster or more on sites such as aprs.fi, aprsdirect.de, ...  
But I didn't really have an idea where to start. Until a certain day in July 2022.

Our holiday was over and on the way back to Belgium from Italy we stopped at a motorway car park in Germany. When I went to see where the tracker had been picked up, I saw an igate with a url in it. This made me curious. Just clicked on the link and it was just what I was looking for.

The link : https://aprs.prgm.org/lora-aprs/db0lj.php

Back home, I emailed the site owner asking how he had created this and if I could use some of the code. Unfortunately, the code was not usable but I now had a direction I could work towards.


# String in detail
I just monitor the data sent from the host (computer) : 
- TX = FE FE 00 E0 1C 00 01 FD
- RX = FE FE 00 E0 1C 00 00 FD

# Serial library
One of my goals was to eliminate the use of an older RS232 port. The CI-V port is 5V TTL compatible. You can build the level converter with
the famous MAX232 and a USB <> RS232 adaptor. But the Arduino has already the hardware. By creating a second serial port in code you can eliminate both MAX232 and USB adaptor.

But I ran into problems when I tried to echo both devices to each other. Apparantly the standard Software Serial library is interrupt driven or something like that.
I think i got timing issues. I used AltSoftSerial to have a working software serial port with better timing. 

**Attention** : Read the site, the ports this library uses are locked on D8 (rxd) and D9 (txd) for the Arduino Nano.

# Interface


# Other uses
This code is also tested with HRD v5.24.0.36 and my Icom IC756. Remote control works as normal. TX is not available (blame HRD :-)).
Omnirig, hmm, I made a custom ini file, seems to work with the client programm, but fails in real use (JS8call for example).


# Resources


******
