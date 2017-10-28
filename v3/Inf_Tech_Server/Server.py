import socket
from threading import Thread
from Ref import ref
from DataBaseConnection import DataBaseConnection
import json


class ClientThread(Thread):
    def __init__(self, ip, port, used_db_ref, conn):
        Thread.__init__(self)
        self.ip = ip
        self.port = port
        self.used_tables = used_db_ref
        self.connection = DataBaseConnection(self.used_tables)
        self.conn = conn

    def run(self):
        while True:
            try:
                data = self.conn.recv(1024)
                try:
                    obj = dict(json.loads(data))
                except:
                    obj = dict()
                print("Server received data:", data, "from", self.getName())
                response = data
                if "Command" in obj.keys():
                    command = obj["Command"]
                    if command == "getDataBases":
                        response = self.connection.getDataBases()
                    elif command == "openDataBase":
                        params = dict(obj["Params"])
                        name = params["Name"]
                        login = params["Login"]
                        password = params["Password"]
                        response = str.encode(self.connection.openDB(name, login, password))
                    elif command == "showTable":
                        params = dict(obj["Params"])
                        name = params["Name"]
                        response = str.encode(self.connection.showTable(name))
                    elif command == "closeDataBase":
                        self.connection.close()
                print("Send", response, "to", self.getName())
                self.conn.send(response)
            except ConnectionResetError:
                self.connection.close()
                break
        self.conn.close()




TCP_IP = '0.0.0.0'
TCP_PORT = 1234
BUFFER_SIZE = 1024

tcpServer = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpServer.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
tcpServer.bind((TCP_IP, TCP_PORT))
threads = []

used_tables = ref(set())

tcpServer.listen(4)
while True:
    (conn, (ip, port)) = tcpServer.accept()
    newthread = ClientThread(ip, port, used_tables, conn)
    newthread.start()
    threads.append(newthread)

for t in threads:
    t.join()