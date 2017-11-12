package com.company;

public enum DataTypes {
    Int, Real, Char, String, Range, Txtfile;
}

class DataType{
    public static String to_string(DataTypes type){
        switch (type){
            case Int:
                return "Int";
            case Char:
                return "Char";
            case Real:
                return "Real";
            case String:
                return "String";
            case Range:
                return "Range";
            case Txtfile:
                return "Txtfile";
        }
        return "";
    }

    public static DataTypes to_datatypes(String str){
        for(DataTypes e : DataTypes.values()){
            if(DataType.to_string(e).equals(str)){
                return e;
            }
        }
        return DataTypes.String;
    }

    public static String get_default_value(DataTypes type){
        switch (type){
            case Int:
                return "0";
            case Char:
                return "A";
            case Real:
                return "0.0";
            case String:
                return "";
            case Range:
                return "[0;0;0]";
            case Txtfile:
                return "[;]";
        }
        return "";
    }

    public static boolean is_parsable_as_type(DataTypes type, String val){
        return true;
    }

    private DataType(){}
}
