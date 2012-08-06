import serial
import struct
import time

commands = {'motor1fwd':0,'motor1bwd':1,'vmin':2,'vmax':3,'motor2fwd':4,'motor2bwd':5,'motor1drive':6,'motor2drive':7,'timeout':14,'baud':15}
baudcodes = {2400:1, 9600:2, 19200:3, 38400:4}

def set_baud(address, baudrate):
	packet = make_packet(address, commands['baud'], baudcodes[baudrate])
	ser.write(packet)

def drive(address, motor, speed):
	if motor==1:
		command = 0
	elif motor==2:
		command = 4
	else:
		return false
	
	if speed<0:
		command = command + 1
		speed = -speed
	
	if speed >127:
		speed = 127
	
	packet = make_packet(address, command, speed)
	ser.write(packet)

def make_packet(address,command,data):
	return struct.pack('BBBB', address, command, data, (127&(address+command+data)))

addr = 130
baud = 9600
ser = serial.Serial('/dev/ttyUSB0',baud)
#set_baud(130,baud)
print "Sending commands at 9600 baud\n"
print "Motor 1 forward\n"
drive(addr, 1, 127)
time.sleep(5)
print "Motor 1 reverse\n"
drive(addr, 1, -127)
time.sleep(5)
print "Motor 2 forward\n"
drive(addr, 1, 0)
drive(addr, 2, 127)
time.sleep(5)
print "Motor 2 reverse\n"
drive(addr, 2, -127)
time.sleep(5)
print "Test complete.\n"
drive(addr, 2, 0)
