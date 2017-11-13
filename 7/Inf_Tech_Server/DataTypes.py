from enum import Enum
import re

class DataTypes(Enum):
    Int = "Int"
    Real = "Real"
    Char = "Char"
    String = "String"
    Range = "Range"
    Txtfile = "Txtfile"

    def to_string(val):
        return val.name

    def to_data_types(val):
        if val == "Int":
            return DataTypes.Int
        elif val == "Real":
            return DataTypes.Real
        elif val == "Char":
            return DataTypes.Char
        elif val == "String":
            return DataTypes.String
        elif val == "Range":
            return DataTypes.Range
        elif val == "Txtfile":
            return DataTypes.Txtfile
        return None

    def get_default_value(type):
        if type == DataTypes.Int:
            return "0"
        elif type == DataTypes.Real:
            return "0.0"
        elif type == DataTypes.Char:
            return "A"
        elif type == DataTypes.String:
            return ""
        elif type == DataTypes.Range:
            return "[0;0;0]"
        elif type == DataTypes.Txtfile:
            return "[;]"
        return  ""

    def is_parsable_as_type(type, value):
        val = str(value)
        if val == DataTypes.get_default_value(type):
            return True

        if type == DataTypes.Int:
            try:
                int(val)
                return True
            except ValueError:
                return False
        elif type == DataTypes.Real:
            try:
                float(val)
                return True
            except ValueError:
                return False
        elif type == DataTypes.Char:
            rex = re.compile(".")
            return rex.fullmatch(val)
        elif type == DataTypes.String:
            return True
        elif type == DataTypes.Range:
            str = '''\[(([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)\;([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?(\;([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)?\]'''
            rex = re.compile(str)
            return rex.fullmatch(val)
        elif type == DataTypes.Txtfile:
            str = '''(\[[\w,\s-]+\.[A-Za-z]+\;(.|\n|\t)+\])|(\[\;\])'''
            rex = re.compile(str)
            return rex.fullmatch(val)
        return False