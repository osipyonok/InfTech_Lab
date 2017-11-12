package com.company;
import java.lang.reflect.Array;
import java.util.*;
import org.json.*;

import javax.swing.text.html.HTMLDocument;


public class DataRow {
    class Value{
        DataTypes type;
        String value = "";

        Value(DataTypes _t , String _v){
            type = _t;
            value = _v;
        }

        Value(JSONObject obj){
            try{
                type = DataType.to_datatypes(obj.getString("type"));
                value = obj.getString("value");
            }catch (Exception e){}
        }

        JSONObject toJson(){
            JSONObject obj = new JSONObject();
            try{
                obj.put("type", DataType.to_string(type));
                obj.put("value", value);
            }catch (Exception e){}
            return obj;
        }
    }

    boolean is_sorted = false;
    ArrayList<Map<Integer , Value>> data = new ArrayList<>();

    public DataRow(ArrayList<DataTypes> types , ArrayList<Map<Integer , String>> _data){
        for(int i = 0 ; i < _data.size() ; ++i){
            Map<Integer , Value> tmp = new HashMap<>();
            int key = _data.get(i).keySet().iterator().next();
            String val = _data.get(i).get(key);
            tmp.put(key , new Value(types.get(i) , val));
            data.add(tmp);
        }
        is_sorted = false;
    }

    public DataRow(JSONObject obj){
        Iterator it = obj.keys();
        while (it.hasNext()) {
            String key = (String) it.next();
            Map<Integer, Value> tmp = new HashMap<>();
            try {
                tmp.put(Integer.parseInt(key), new Value(obj.getJSONObject(key)));
                data.add(tmp);
            }catch (Exception e){}
        }
        is_sorted = false;
    }

    public DataRow(DataRow other , Set<Integer> projection){
        for(Map<Integer , Value> mp : other.data){
            int key = mp.keySet().iterator().next();
            if(projection.contains(key)){
                this.data.add(mp);
            }
        }
        is_sorted = false;
    }

    public JSONObject toJson(){
        JSONObject obj = new JSONObject();
        for(Map<Integer , Value> mp : data){
            int key = mp.keySet().iterator().next();
            try {
                obj.put("" + key, mp.get(key).toJson());
            }catch (Exception e){}
        }
        return obj;
    }

    public ArrayList<Map<Integer , Value>> getData(){
        return (ArrayList<Map<Integer , Value>>)data.clone();
    }

    public void update(int pos , String val){
        for(int i = 0 ; i < data.size() ; ++i){
            if(data.get(i).containsKey(pos)){
                Value v = data.get(i).get(pos);
                v.value = val;
                data.get(i).put(pos , v);
            }
        }
    }

    public static Comparator<Map<Integer , Value>> comparator(){
        Comparator comp = new Comparator() {
            @Override
            public int compare(Object o1, Object o2) {
                Integer key1 = ((Map<Integer , Value>)o1).keySet().iterator().next();
                Integer key2 = ((Map<Integer , Value>)o2).keySet().iterator().next();
                return key1.compareTo(key2);
            }
        };
        return comp;
    }

    private void sortData(){
        if(is_sorted)return;

        Collections.sort(data , comparator());
        is_sorted = true;
    }

}
