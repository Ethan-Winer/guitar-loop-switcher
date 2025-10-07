# DIY Guitar Loop Switcher

## What is a loop switcher?
    A loop switcher is a device that can automatically bypass or enable multiple guitar effect 
	pedals at once. In this case, there are 6 available relays for 6 pedals. Additionally, there are 
	6 momentary switches on top of the box to toggle each individual relay in a preset. I have 
	used it live and it does work but there is still a lot I want to work on with it. The end goal is 
	to implement all the features listed bellow, make a pcb for it, and get one of my engineering
	friends to help me make a 3D printed enclosure.

## Things I want to add / change	
	- Controll preset changes with MIDI controller instead of a voltage reading
		* Also including MIDI hardware passthrough to reduce latency
	
    - Rewrite the code to be object oriented and more readable (I wrote it last minute in a
	  rush because I had to get ready to leave for college the next day)
	
    - Better use of EEPROM, currently one boolean value is stored in a full byte
	
    - 7 segment displays to indicate the current profile and preset withing the profile

## Some pictures of the project in its current state
The current setup, loop switcher is the wood box with the red led buttons. Controller is the wood box with letters:
![A guitar pedalboard with the DIY loop switcher](https://github.com/Ethan-Winer/guitar-loop-switcher/blob/main/full-setup.jpg?raw=true)

This is the wiring for the loop switcher. The footswitch is just 6 momentary switches and a daisy chain of diodes:
![The internal wiring for the DIY loop switcher](https://github.com/Ethan-Winer/guitar-loop-switcher/blob/main/loop-switcher-wiring.jpg?raw=true)