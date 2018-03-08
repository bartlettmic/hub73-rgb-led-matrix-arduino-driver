# hub73-rgb-led-matrix-arduino-driver
A library for driving a HUB73 (very uncommon) RGB LED matrix (32x32) using an Arduino.
---
Approach:

Bit banging timing:

* 1024pixels * 60Hz = 61440 Refreshes / Sec -> 1 refresh = 1 / 61440 seconds -> 16.28 micro-seconds -> 0.01628ms ~= 2% of a millisecond
* each pixel gets 16+2/3 ms -> denote as one "period"
* -> capable of 62.5ns per op


Timer output	Arduino output
OC0A	        6	
OC0B	        5	
OC1A	        9	
OC1B	        10	
OC2A	        11	
OC2B	        3	            -> use 3, 6, 9? 

Bit banging:
* Pros: can just use PortD
* Cons: Manual PWM

PWM pins:
* Pros: Don't have to calculate manual PWM
* Cons: Can't just use Port D