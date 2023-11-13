import serial 


path2_signal = "L1"

path3_signal = "R0"

path4_signal = "R1"


ser = serial.Serial()


def send_path1():
    path1_signal = "L0"
    ser.write(path1_signal)

def send_path2():
    ser.write(path2_signal) 

def send_path3():
    ser.write(path3_signal)

def send_path4():
    ser.write(path4_signal)