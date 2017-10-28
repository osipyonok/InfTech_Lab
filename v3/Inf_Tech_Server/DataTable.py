from DataRow import *
import json
from DataTypes import *
from Ref import *


class DataTable:
#    name = ""
#    rows = []
#    descriptions = []
#    field_names = []
#    is_description_sorted = False

    def __init__(self):
        self.name = ""
        self.rows = list()
        self.descriptions = list()
        self.field_names = list()
        self.is_description_sorted = False

  #  def __init__(self, _name, _desc, _fnames):
  #      self.name = _name
  #      i = 0
  #      for d in _desc:
  #          self.descriptions.append([i, d])
  #          self.field_names.append([i, _fnames[i]])
  #          i = i + 1
  #      self.is_description_sorted = False


    def fromJson(self, obj):
        print(obj)
        if not ("name" in obj.keys() and "description" in obj.keys() and "rows" in obj.keys()):
            raise TypeError

        self.name = str(obj["name"])
        _desc = dict(obj["description"])
        for key in _desc.keys():
            val = str(_desc[key])
            self.descriptions.append([int(key), DataTypes.to_data_types(val)])

        _rows = dict(obj["rows"])
        for e in _rows.keys():
            print(self.name, e)
            self.rows.append(DataRow(_rows[e]))

        _fnames = dict(obj["field_names"])
        for key in _fnames.keys():
            self.field_names.append([int(key), str(_fnames[key])])

        self.is_description_sorted = False

    def sortDescription(self):
        if self.is_description_sorted:
            return
        self.descriptions.sort(key = lambda e: e[0])
        self.is_description_sorted = True

    def getDataType(self, index):
        self.sortDescription()
        l = 0
        r = len(self.descriptions)
        while r - l > 1:
            mid = (r + l) >> 1
            if self.descriptions[mid][0] > index:
                r = mid
            else:
                l = mid

        if self.descriptions[r][0] == index:
            return self.descriptions[r][1]
        if self.descriptions[l][0] == index:
            return self.descriptions[l][1]
        return [False, DataTypes.Int]

    def validate(self, descr, values, was):
        if len(descr) < len(values):
            return False

        result = True
        for e in values:
            index = e[0]
            val = e[1]
            type = self.getDataType(index)
            if type[0] == False:
                return False
            lst = list(was.get())
            lst.append(type[1])
            was.set(lst)
            result = result and DataTypes.is_parsable_as_type(type[1], val)

        return result


    def addRow(self, _data):
        used_datatypes = ref(list())
        if not self.validate(self.descriptions, _data, used_datatypes):
            return False
        self.rows.append(DataRow(used_datatypes.get(), _data))
        return True

    def toJson(self):
        obj = dict()
        obj["name"] = self.name

        obj["rows"] = dict()
        for i in range(0, len(self.rows)):
            obj["rows"][str(i)] = self.rows[i].toJson()

        obj["description"] = dict()
        for e in self.descriptions:
            obj["description"][str(e[0])] = DataTypes.to_string(e[1])

        obj["field_names"] = dict()
        for e in self.field_names:
            obj["field_names"][str(e[0])] = e[1]

        return obj

    def getName(self):
        return self.name

    def getDescriptions(self):
        return self.descriptions

    def getFieldNames(self):
        return self.field_names

    def getRows(self):
        return self.rows

    def updateTable(self, values):
        was = set()
        for e in values:
            self.rows[e[0][0]].update(e[0][1], e[1])
            was.add(e[0][0])
        cur = []
        for i in range(0, len(self.rows)):
            if i in was:
                cur.append(self.rows[i])
        self.rows = cur

    def Project(self, table, _cols):
        res = DataTable()
        res.name = "Projection"
        cols = set()
        for e in _cols:
            cols.add(e)

        table.field_names.sort()
        mp = dict()
        for e in table.field_names:
            mp[e[1]] = e[0]
        need = set()
        for e in _cols:
            if e in mp:
                need.add(mp[e])

        if len(need) == 0:
            return res

        for row in res.rows:
            res.rows.append(DataRow(row, need))
        for e in _cols:
            if e in mp:
                res.field_names.append([mp[e], e])
        table.sortDescription()
        desc = table.getDescriptions()
        for e in res.field_names:
            idx = e[0]
            l = 0
            r = len(desc)
            while r - l > 1:
                mid = (r + l) >> 2
                if desc[mid][0] > idx:
                    r = mid
                else:
                    l = mid
            res.descriptions.append(desc[l])
        return res


