from Ref import ref
import os
import json
from DataBase import DataBase
from DataTable import DataTable

DATABASE_MAIN_DIRECTORY = r"db/"

def readJson(file):
    with open(file) as jsonFile:
        data = json.load(jsonFile)
        return data

class DataBaseConnection:

    def __init__(self, used_db_ref):
        self.db = None
        self.path = ""
        self.used_tables = used_db_ref

    def getDataBases(self):
        obj = dict()
        obj["getDataBases"] = dict()
        e = self.used_tables.get()
        for file in os.listdir(DATABASE_MAIN_DIRECTORY):
            filename = os.fsdecode(file)
            if filename.endswith(".dblab"):
                cur = readJson(DATABASE_MAIN_DIRECTORY + filename)
                name = cur["name"]
                obj["getDataBases"][name] = name in e
        return str.encode(json.dumps(obj))

    def getDataBasePath(self, target_name):
        for file in os.listdir(DATABASE_MAIN_DIRECTORY):
            filename = os.fsdecode(file)
            if filename.endswith(".dblab"):
                cur = readJson(DATABASE_MAIN_DIRECTORY + filename)
                name = cur["name"]
                if target_name == name:
                    return DATABASE_MAIN_DIRECTORY + filename
        return ""

    def openDB(self, name, login, password):
        obj = dict()
        tot = dict()
        dbPath = self.getDataBasePath(name)
        if not dbPath:
            obj["Result"] = False
        elif name in set(self.used_tables.get()):
            obj["Result"] = False
        else:
            self.db = DataBase(readJson(dbPath))
            if self.db.authentication(login, password):
                e = set(self.used_tables.get())
                e.add(name)
                self.used_tables.set(e)
                self.path = dbPath
                obj["Result"] = True
                tables = []
                for o in self.db.getTableNamesSet():
                    tables.append(o)
                obj["Tables"] = tables
                obj["Name"] = name
                obj["Login"] = login
                obj["Password"] = password
            else:
                self.db = None
                self.path = ""
                obj["Result"] = False
        tot["openDataBase"] = obj
        return json.dumps(tot)

    def showTable(self, name):
        obj = dict()
        tot = dict()
        ck = True
        try:
            tableNames = self.db.getTableNamesSet()
            ck = name in tableNames
            if not ck:
                pass
        except:
            ck = False
        obj["Result"] = ck
        if ck:
            tbl = self.db.getDataTable(name)
            print("Name", tbl.getName())
            obj["Table"] = self.db.getDataTable(name).toJson()
        tot["showTable"] = obj
  #      print("SHOW TABLE", tot)
        return json.dumps(tot)

    def close(self):
        if not self.db is None:
            e = self.used_tables.get()
            e.discard(self.db.getName())
            self.used_tables.set(e)

    def getPossibleIntersections(self, tbl):
        obj = dict()
        tot = dict()
        cur_table = self.db.getDataTable(tbl)
        arr = []
        for tn in self.db.getTableNamesSet():
            cdb = self.db.getDataTable(tn)
            if cdb.__eq__(cur_table) and tbl != cdb.getName():
                arr.append(cdb.getName())
        obj["Result"] = True
        obj["Tables"] = arr
        tot["getPossibleIntegsection"] = obj
        return json.dumps(tot)

    def intersect(self, tables):
        obj = dict()
        tot = dict()
        tables = list(tables)
        obj["Result"] = True
        obj["Table"] = self.db.intersection(tables).toJson()
        tot["makeIntersection"] = obj
        return json.dumps(tot)