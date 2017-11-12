from DataTypes import *
import json

class Value:
    def __init__(self, _t, _v):
        self.type = _t
        self.value = _v

    def __init__(self, obj):
#       obj = dict(json.loads(obj))
        self.type = DataTypes.to_data_types(obj["type"])
        self.value = str(obj["value"])

    def toJson(self):
        dic = dict()
        dic["type"] = DataTypes.to_string(self.type)
        dic["value"] = self.value
        return dic

class DataRow:

    def __init__(self, types, _data):
        for i in range(0, len(_data)):
            self.data.append([_data[i][0], Value(types[i], _data[i][1])])
        self.is_sorted = False

    def __init__(self, other, projection):
        for e in other.data:
            if e[0] in projection:
                self.data.append(e)
        self.is_sorted = False

    def __init__(self, obj):
        self.is_sorted = False
        self.data = list()
        obj = dict(obj)
        for e in obj.keys():
            key = int(e)
            self.data.append([key, Value(obj[e])])
        self.is_sorted = False

    def toJson(self):
        dic = dict()
        for e in self.data:
            dic[str(e[0])] = e[1].toJson()
        return dic

    def getData(self):
        return self.data

    def update(self, pos, val):
        for i in range(0, len(self.data)):
            if self.data[i][0] == pos:
                self.data[i][1] = val

