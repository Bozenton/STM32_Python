from socket import timeout
import serial
import serial.tools.list_ports
import struct
import threading
import time
import numpy as np

"""
------------------------------------- 数据包格式 -------------------------------------
字节数      数据        说明
1           0xFF       包头
1           0x         字节长度（数据部分） 0~254
1           0x         该字节用于表示数据类型
n           ...        data部分
1           0x         校验和，对数据部分累加取低八位
"""

class SendMsg():
    def __init__(self, baud) -> None:
        self.baud = baud
        self.start_time = time.time()
        while True:
            port_list = list(serial.tools.list_ports.comports())
            if len(port_list) == 1:
                self.portx = port_list[0].device
                break
            elif len(port_list) > 1:
                temp_i = 1
                print(port_list)
                for item in port_list:
                    print(temp_i, ' - ', item.device)
                self.portx = port_list[int(input('Please enter the num of the port: '))-1].device
                break
            else:
                input('未发现串口，请重新检测')
        self.time_out = 2
        self.serial = serial.Serial(self.portx, self.baud, timeout=self.time_out)
        print(self.portx, "Open!")

        self.RecvThread_thread = threading.Thread(target=self.RecvThread, args=())
        self.RecvThread_thread.daemon = True
        self.RecvThread_thread.start()

    def RecvThread(self):
        while True:
            time.sleep(0.01)
            if self.serial.in_waiting:
                RecvData = self.serial.read(self.serial.in_waiting)
                if len(RecvData) > 0:
                    print(RecvData)
    
    def send(self, msg_type : bytes, data : list):
        HEAD = b'\xff'

        length = len(data)
        check_sum = np.sum(data)
        check_point = check_sum & 0xff

        send_data = HEAD
        send_data += struct.pack('=BB', length, msg_type)
        for item in data:
            send_data += struct.pack('=B', item)
        send_data += struct.pack('=B', check_point)
        print(send_data)

        self.serial.write(send_data)

if __name__ == "__main__":
    c = SendMsg(115200)
    while True:
        c.send(1, [1,2,3])
        time.sleep(1)