package com.company;
import java.util.*;
import org.json.*;

import javax.lang.model.type.ArrayType;


public class DataTable {
    String name;
    ArrayList<DataRow> rows = new ArrayList<>();
    ArrayList<Map<Integer , DataTypes>> description = new ArrayList<>();
    ArrayList<Map<Integer , String>> field_names = new ArrayList<>();

    boolean is_description_sorted = false;

    public DataTable(String _name , ArrayList<DataTypes> _desc , ArrayList<String> _fnames){
        name = _name;
        int i = 0;
        for(DataTypes type : _desc){
            Map<Integer , DataTypes> tmp1 = new HashMap<>();
            Map<Integer , String> tmp2 = new HashMap<>();
            tmp1.put(i , type);
            tmp2.put(i , _fnames.get(i));
            ++i;
        }
        is_description_sorted = false;
    }

    public DataTable(JSONObject obj){
        try{
            name = obj.getString("name");
            JSONObject jdesc = obj.getJSONObject("description");
            Iterator it1 = jdesc.keys();
            while (it1.hasNext()){
                String key = (String)it1.next();
                Map<Integer , DataTypes> tmp = new HashMap<>();
                tmp.put(Integer.parseInt(key) , DataType.to_datatypes(jdesc.getString(key)));
                description.add(tmp);
            }

            JSONObject fnames = obj.getJSONObject("field_names");
            Iterator it2 = fnames.keys();
            while (it2.hasNext()){
                String key = (String)it2.next();
                Map<Integer , String> tmp = new HashMap<>();
                tmp.put(Integer.parseInt(key) , fnames.getString(key));
                field_names.add(tmp);
            }

            JSONObject rows = obj.getJSONObject("rows");
            Iterator it3 = rows.keys();
            while (it3.hasNext()){
                String key = (String)it3.next();
                this.rows.add(new DataRow(rows.getJSONObject(key)));
            }

            is_description_sorted = false;
        }catch (Exception e){}
    }

    public static Comparator<Map<Integer , DataTypes>> comparator(){
        Comparator comp = new Comparator() {
            @Override
            public int compare(Object o1, Object o2) {
                Integer key1 = ((Map<Integer , DataTypes>)o1).keySet().iterator().next();
                Integer key2 = ((Map<Integer , DataTypes>)o2).keySet().iterator().next();
                return key1.compareTo(key2);
            }
        };
        return comp;
    }

    private void sortDescription(){
        if(is_description_sorted)return;
        Collections.sort(description , comparator());
        is_description_sorted = true;
    }

    public DataTypes getDataType(int index){
        sortDescription();

        int l = 0 , r = description.size();
        while(r - l > 1){
            int mid = (r + l) / 2;
            Map<Integer , DataTypes> mid_o = description.get(mid);
            int mid_v = mid_o.keySet().iterator().next();
            if(mid_v > index){
                r = mid;
            }else{
                l = mid;
            }
        }
        return description.get(l).get(description.get(l).keySet().iterator().next());
    }

    public void addRow(ArrayList<Map<Integer , String>> data){
        ArrayList<DataTypes> types = new ArrayList<>();
        for(Map<Integer , DataTypes> e : this.description){
            types.addAll(e.values());
        }
        this.rows.add(new DataRow(types , data));
    }

    public JSONObject toJson(){
        JSONObject obj = new JSONObject();
        try{
            obj.put("name", name);
            JSONObject tmp1 = new JSONObject();
            for(int i =  0 ; i < rows.size() ; ++i){
                tmp1.put("" + i, rows.get(i).toJson());
            }
            obj.put("rows" , tmp1);

            JSONObject tmp2 = new JSONObject();
            for(Map<Integer , DataTypes> e : description){
                tmp2.put("" + e.keySet().iterator().next(), DataType.to_string(e.get(e.keySet().iterator().next())));
            }
            obj.put("description" , tmp2);

            JSONObject tmp3 = new JSONObject();
            for(Map<Integer , String> e : field_names){
                tmp3.put("" + e.keySet().iterator().next(), e.get(e.keySet().iterator().next()));
            }
            obj.put("field_names", tmp3);


        }catch (Exception except){}
        return obj;
    }

    public String getName() {
        return name;
    }

    public ArrayList<DataRow> getRows() {
        return rows;
    }

    public ArrayList<Map<Integer, DataTypes>> getDescription() {
        return description;
    }

    public ArrayList<Map<Integer, String>> getField_names() {
        return field_names;
    }

    private DataTable(){}

    public void updateTable(Map<Map<Integer, Integer> , String> values){
        Set<Integer> was = new HashSet<>();
        for(Map<Integer, Integer> e : values.keySet()){
            int fi = e.keySet().iterator().next();
            int se = e.get(fi);
            DataRow row = rows.get(fi);
            row.update(se , values.get(e));
            rows.add(fi , row);
            was.add(fi);
        }
        ArrayList<DataRow> cur = new ArrayList<>();
        for(int i = 0 ; i < rows.size() ; ++i){
            if(was.contains(i)){
                cur.add(rows.get(i));
            }
        }
        this.rows = cur;
    }

    public static Comparator<Map<Integer , String>> comparator2(){
        Comparator comp = new Comparator() {
            @Override
            public int compare(Object o1, Object o2) {
                Integer key1 = ((Map<Integer , String>)o1).keySet().iterator().next();
                Integer key2 = ((Map<Integer , String>)o2).keySet().iterator().next();
                return key1.compareTo(key2);
            }
        };
        return comp;
    }

    public DataTable project(DataTable table , ArrayList<String> _cols){
        DataTable res = new DataTable();
        res.name = "Projection";
        Set<String> cols = new HashSet<>();
        cols.addAll(_cols);
        Collections.sort(field_names , comparator2());
        Map<String , Integer> mp = new HashMap<>();
        for(Map<Integer , String> e : field_names){
            int key = e.keySet().iterator().next();
            mp.put(e.get(key) , key);
        }
        Set<Integer> need = new HashSet<>();
        for(String str : _cols){
            if(mp.containsKey(str)){
                Map<Integer , String> tmp = new HashMap<>();
                tmp.put(mp.get(str) , str);
            }
        }
        if(need.size() == 0)return res;

        for(DataRow row : res.rows){
            res.rows.add(new DataRow(row , need));
        }

        for(String str : _cols){
            if(mp.containsKey(str)){
                Map<Integer , String> tmp = new HashMap<>();
                tmp.put(mp.get(str) , str);
                res.field_names.add(tmp);
            }
        }
        table.sortDescription();
        ArrayList<Map<Integer , DataTypes>> des = table.getDescription();
        for(Map<Integer , String> e : res.field_names){
            int idx = e.keySet().iterator().next();
            int l = 0 , r = des.size();
            while (r - l > 1){
                int mid = (r + l) / 2;
                Map<Integer , DataTypes> mid_o = des.get(mid);
                int mid_v = mid_o.keySet().iterator().next();
                if(mid_v > idx){
                    r = mid;
                }else {
                    l = mid;
                }
            }
            res.description.add(des.get(l));
        }
        return res;
    }


}
