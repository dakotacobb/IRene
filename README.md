# IRene
Robot that is controlled by an IR remote


## Group Members
Dakota Cobb

## Video Demo
https://www.youtube.com/watch?v=Y6xmmqbI8vc&ab_channel=DakotaCobb

## Idea
To create a fully functioning robot that can move around with any remote and any IR sensor that someone can get their hands on. The remote can be used to do anything a mbed can do with the right hardware. I added an LCD screen, LED and accelerometer to mine, as well as a dual H-bridge for controlling 2 DC motors. 

## Instructions
To set up you will need a IR remote and an IR sensor. IR sensors usually have 3 pins, power, ground, and a data line. Hook this data line to a digital I/O of your choice (I used p15) If you run the main program then it will automatically print the last two hexadecimal numbers to your PC. I used the last two since the last bytes are the ones that change. The ones at the beggining rarely do between button to button. You can do this with TeraTerm on Windows or screen /dev/tty.usbmodem* on mac. If everything is set up correctly you can press a button on the remote and it will print those to hexadecimal numbers to your screen
