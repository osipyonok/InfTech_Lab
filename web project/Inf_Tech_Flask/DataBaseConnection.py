from Ref import ref
import os
import json
from DataBase import DataBase
import sqlite3

DATABASE_MAIN_DIRECTORY = r"db/"


def readJson(file):
    with open(file) as jsonFile:
        data = json.load(jsonFile)
        return data


def getDB():
    conn = sqlite3.connect('database.db')
    conn.execute('CREATE TABLE IF NOT EXISTS `used_tables`(`table` TEXT)')
    cursor = conn.cursor()
    return (conn, cursor)


def get_used_tables():
    (conn, cursor) = getDB()
    cursor.execute('SELECT `table` FROM `used_tables` WHERE 1')
    rows = cursor.fetchall()
    e = set()
    for r in rows:
        e.add(r[0])
    return e


def add_used_table(table):
    (conn, cursor) = getDB()
    cursor.execute('INSERT INTO `used_tables` VALUES(\'' + table + '\')')
    conn.commit()


def discard_used_table(table):
    (conn, cursor) = getDB()
    cursor.execute('DELETE FROM `used_tables` WHERE `table` = \'' + table + '\'')
    conn.commit()


class DataBaseConnection:

    def __init__(self, d=None):
        self.db = None
        self.path = ""
        if d is not None:
            self.db = d['db']
            self.path = d['path']


    def toJson(self):
        obj = dict()
        obj['path'] = self.path
        if self.db is not None:
            obj['db'] = self.db.toJson()


    def fromJson(self, obj):
        self.path = obj['path']
        self.db = None
        if 'db' in obj.keys():
            self.db = DataBase(obj['db'])

    def getDataBases(self):
        obj = dict()
        obj["getDataBases"] = dict()
        (conn, cursor) = getDB()
        cursor.execute('SELECT `table` FROM `used_tables` WHERE 1')
        rows = cursor.fetchall()
        e = get_used_tables()
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
                if target_name.strip() == name.strip():
                    return DATABASE_MAIN_DIRECTORY + filename
        return ""

    def openDB(self, name, login, password):
        obj = dict()
        tot = dict()
        dbPath = self.getDataBasePath(name)
        used_tables = get_used_tables()
        print('Used tables', used_tables)
        print('Path', dbPath)
        if not dbPath:
            obj["Result"] = False
        elif name in used_tables:
            obj["Result"] = False
        else:
            print("Here")
            self.db = DataBase(readJson(dbPath))
            if self.db.authentication(login, password):
                add_used_table(name.strip())
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
        print('Self db', self.db.toJson())
        return json.dumps(tot)

    def showTable(self, name):
        obj = dict()
        tot = dict()
        ck = True
        if isinstance(self.db, dict):
            self.db = DataBase(self.db)
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
            obj["Table"] = self.db.getDataTable(name).toJson()
        tot["showTable"] = obj
        return json.dumps(tot)

    def close(self):
        if self.db is not None:
            discard_used_table(self.db.getName())
