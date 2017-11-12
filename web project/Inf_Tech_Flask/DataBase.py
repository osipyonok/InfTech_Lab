import json
from Ref import *
from DataTable import *

class DataBase:


#    def __init__(self, _name, _login, _password):
#        self.name = _name
#        self.login = _login
#        self.password = _password
#        self.is_auth = True



    def __init__(self, jsonObject):
        self.name = ""
        self.login = ""
        self.password = ""

        self.is_auth = False
        self.used_names = set()
        self.tables = []
        self.active_table = -1
     #   print(jsonObject)
        obj = jsonObject
        if not ("name" in obj.keys() and "login" in obj.keys() and "pass" in obj.keys() and "tables" in obj.keys()):
            raise FileNotFoundError
        self.name = obj["name"]
        self.login = obj["login"]
        self.password = obj["pass"]
        tbls = dict(obj["tables"])
        for e in tbls.keys():
            tmp = tbls[e]
            self.used_names.add(str(tmp["name"]))
            tbl = DataTable()
            tbl.fromJson(tmp)
            self.tables.append(tbl)
        pass

    def toJson(self):
        obj = dict()
        obj['name'] = self.name
        obj['login'] = self.login
        obj['pass'] = self.password

        tbls = dict()
        for i in range(0, len(self.tables)):
            tbls[str(i)] = self.tables[i].toJson()
        obj['tables'] = tbls
        return obj


        return obj


    def authentication(self, _login, _pass):
        self.is_auth = self.is_auth or (self.login == _login and self.password == _pass)
        return self.is_auth

    def getDataTable(self, tableName):
        for e in self.tables:
            if e.getName() == tableName:
                return e

    def getTableNamesSet(self):
        return self.used_names

    def getName(self):
        return self.name
