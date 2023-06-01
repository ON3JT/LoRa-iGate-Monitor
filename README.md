# Introduction

I had had a Lora igate active for some time, but felt that the data from other igates was shown faster or more on sites such as aprs.fi, aprsdirect.de, ...  
But I didn't really have an idea where to start. Until a certain day in July 2022.

Our holiday was over and on the way back to Belgium from Italy we stopped at a motorway car park in Germany. When I went to see where the tracker had been picked up, I saw an igate with a url in it. This made me curious. Just clicked on the link and it was just what I was looking for.

The link : https://aprs.prgm.org/lora-aprs/db0lj.php

Back home, I emailed the site owner asking how he had created this and if I could use some of the code. Unfortunately, the code was not usable but I now had a direction I could work towards.

## Screenshots

![App Screenshot](https://via.placeholder.com/468x300?text=App+Screenshot+Here)

## Changes in is-cfg.json
You have to activate the syslog function on your igate by modifying this entry :

```JSON
 	"syslog": {
		"active": true,              activate this
		"server": "192.168.1.56",    ip adress from my ESP32
		"port": 4210                 port where the ESP32 listens to
	}
```
I know that the port was 514, but 4210 was from an Arduino UDP example and I continued using this :-).

# Resources
## Disclaimer
All this code is for demonstration purposes only. It should not be considered safe and bug-free. I cannot be held liable for any damage or loss using this code. 
It can be improved and you can do so at your own discretion.

******
