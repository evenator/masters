import serial
import struct


commands = {'motor1fwd':0,'motor1bwd':1,'vmin':2,'vmax':3,'motor2fwd':4,'motor2bwd':5,'motor1drive':6,'motor2drive':7,'timeout':14,'baud':15}
baudcodes = {2400:1, 9600:2, 19200:3, 38400:4}

baud = 2400
ser = serial.Serial('/dev/ttyUSB0',baud)

def auto_baud(address, baudrate):
	packet = make_packet(address, commands['baud'], baudcodes[baudrate])
	ser.write(packet)

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
set_baud(addr, baud)
drive(addr, 1, 10)
