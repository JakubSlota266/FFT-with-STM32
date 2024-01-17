# Fast Fourier Transform with Nucelo Board
## Assignment project for 'Advanced Design of the Embedded Systems' course
## Elements used:
-> STM32 NUCLEO-F446RE  
-> Audio Codec Board - PROTO  
-> LED Matrix with Max7219 controller 

## Algorithm:
-> Configure and start up communication between elements of the system using I2C and SPI interfaces  
-> Aquire data from microphone input of Proto board  
-> Send data from Proto to Nucelo via I2S interface  
-> Process data using our FFT library  
-> Display output of the program on LED Matrix  

## Authors:
Jakub Słota  
Jakub Pyznar: https://gitlab.com/JakubPyznar  

