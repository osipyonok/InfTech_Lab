import sqlite3
from threading import Thread
from Ref import ref
from DataBaseConnection import DataBaseConnection
import json
import socket
import datetime
from pymongo import MongoClient


CONNSTR = "mongodb://admin:admin@ds257245.mlab.com:57245/queries"
DBNAME = "queries"
OBJECT_QUERY = "Query"
OBJECT_RESPONSE = "Response"


def get_db(COLNAME = OBJECT_QUERY):
    client = MongoClient(CONNSTR)
    db = client[DBNAME]
    col = db[COLNAME]
    return col

class ClientThread(Thread):
    def __init__(self, descriptor, used_tables):
        Thread.__init__(self)
        self.descriptor = descriptor
        self.used_tables = used_tables
        self.connection = DataBaseConnection(self.used_tables)

    def run(self):
        while True:
            try:
                db = get_db()
                db_response = get_db(OBJECT_RESPONSE)
                queries = list(db.find({'author': self.descriptor}).sort('time', 1))
                for query in queries:
                    #print(self.descriptor, query)
                    db.delete_one({'_id': query['_id']})
                    try:
                        obj = dict(json.loads(query['query']))
                    except:
                        obj = dict()
                    print("Server received data:", query['query'], "from", self.descriptor)
                    response = None
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
                    if response is not None:
                        response = response.decode('utf-8')
                        print("Send", response, "to", self.descriptor)
                        data = {
                            'to': self.descriptor,
                            'response': response,
                            'time': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                        }
                        db_response.insert_one(data)
                    
            except ConnectionResetError:
                self.connection.close()
                break
        #self.conn.close()





used_tables = ref(set())
print("Server IP is", socket.gethostbyname(socket.gethostname()))

while True:
    db = get_db()
    connRequests = list(db.find({'query': 'signup'}).sort('time', 1))
    for request in connRequests:
        cur = dict(request)
        print(cur['author'])
        db.delete_one({'_id': cur['_id']})
        newthread = ClientThread(cur['author'], used_tables)
        newthread.start()
        #threads.append(newthread)

    #(conn, (ip, port)) = tcpServer.accept()
    #newthread = ClientThread(ip, port, used_tables, conn)
    #newthread.start()
    #threads.append(newthread)
#    for t in threads:
#        t.join()

