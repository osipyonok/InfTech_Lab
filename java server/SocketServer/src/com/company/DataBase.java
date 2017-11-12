package com.company;
import java.util.*;
import org.json.*;

public class DataBase {
    private String name, login, pass;
    private boolean is_auth = false;
    Set<String> used_names = new HashSet<>();
    ArrayList<DataTable> tables = new ArrayList<>();
    int active_table = -1;

    public DataBase(String name , String login , String pass){
        this.name = name;
        this.login = login;
        this.pass = pass;
        is_auth = true;
    }

    public DataBase(JSONObject obj){
        try {
            name = obj.getString("name");
            login = obj.getString("login");
            pass = obj.getString("pass");

            JSONObject tbls = obj.getJSONObject("tables");
            Iterator it = tbls.keys();
            while(it.hasNext()){
                String key = (String)it.next();
                JSONObject tbl = tbls.getJSONObject(key);
                used_names.add(tbl.getString("name"));
                tables.add(new DataTable(tbl));
            }
        }catch (Exception e){}

    }


    public DataTable getDataTable(String name){
        for(DataTable table : tables)
            if(table.getName().equals(name)){
                return table;
            }

        return tables.get(0);
    }

    public boolean authentication(String _login , String _pass){
        return is_auth = (is_auth || (login.equals(_login) && pass.equals(_pass)));
    }

    JSONObject toJson(){
        JSONObject obj = new JSONObject();
        if(is_auth == false)return obj;

        try{
            obj.put("name", name);
            obj.put("login", login);
            obj.put("pass", pass);

            JSONObject tabl = new JSONObject();
            for(int i = 0 ; i < tables.size() ; ++i){
                tabl.put("" + i, tables.get(i).toJson());
            }
            obj.put("tables", tabl);
        }catch (Exception exc){}
        return obj;
    }

    public Set<String> getUsed_names() {
        return used_names;
    }

    public String getName() {
        return name;
    }

    public String getLogin() {
        return login;
    }

    public String getPass() {
        return pass;
    }

    public boolean setName(String string){
        if(is_auth == false)return false;
        name = string;
        return true;
    }

    public boolean setLogin(String string){
        if(is_auth == false)return false;
        login = string;
        return true;
    }

    public boolean setPassword(String string){
        if(is_auth == false)return false;
        pass = string;
        return true;
    }
}
