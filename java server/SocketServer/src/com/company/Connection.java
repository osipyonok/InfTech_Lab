package com.company;
import com.oracle.tools.packager.IOUtils;
import org.json.JSONArray;
import org.json.JSONObject;


import java.io.*;
import java.net.*;
import java.util.*;

public class Connection implements Runnable {
    private Set<String> used_tables;
    private Socket socket;
    private DataBase db = null;
    private String path = null;

    private static final String dbs_dir = "db/";


    public Connection(Set<String> used_tables, Socket socket){
        this.used_tables = used_tables;
        this.socket = socket;
    }

    private JSONObject getDataBases(){
        JSONObject obj = new JSONObject();
        File dir = new File(dbs_dir);
        File[] listing = dir.listFiles();
        if(listing != null)
            for(File file : listing){
                if(file.isFile() && file.getName().contains(".dblab")){
                    try {
                        JSONObject tmp = new JSONObject(DataBaseIO.readJson(file.getAbsolutePath()));
                        String name = tmp.getString("name");
                        boolean b = used_tables.contains(name);
                        obj.put(name , b);
                    }catch (Exception exc){}
                }
            }
        return obj;
    }

    private JSONObject openDataBase(String name , String login , String pass){
        JSONObject obj = new JSONObject();
        File dir = new File(dbs_dir);
        File[] listing = dir.listFiles();
        path = null;
        if(listing != null)
            for(File file : listing){
                if(file.isFile() && file.getName().contains(".dblab")){
                    try {
                        JSONObject tmp = new JSONObject(DataBaseIO.readJson(file.getAbsolutePath()));
                        String cur_name = tmp.getString("name");
                        if(cur_name.equals(name)){
                           path = file.getAbsolutePath();
                           break;
                        }
                    }catch (Exception exc){}
                }
            }

        try {
            if (path != null) {
                boolean res = ! used_tables.contains(name);
                if(res){
                    this.db = new DataBase(new JSONObject(DataBaseIO.readJson(path)));
                    if(this.db.authentication(login, pass)){
                        res = true;
                        JSONArray arr = new JSONArray();
                        for(String e : this.db.getUsed_names()){
                            arr.put(e);
                        }
                        obj.put("Tables" , arr);
                        obj.put("Name", name);
                        obj.put("Login", login);
                        obj.put("Password", pass);
                    }else{
                        res = false;
                        this.db = null;
                    }
                }
                obj.put("Result", res);
            } else {
                obj.put("Result", false);
            }
        }catch (Exception exc){}
        return obj;
    }


    private JSONObject showTable(String name){
        JSONObject obj = new JSONObject();
        try{
            Set<String> tnames = this.db.getUsed_names();
            boolean res = tnames.contains(name);
            obj.put("Result", res);
            if(res){
                obj.put("Table", this.db.getDataTable(name).toJson());
            }
        }catch (Exception exc){}
        return obj;
    }

    private JSONObject setMainData(String name , String login , String pass){
        JSONObject obj = new JSONObject();
        try{
            boolean ck = true;
            if(name.length() > 0){
                ck &= db.setName(name);
                ck &= db.setLogin(login);
                ck &= db.setPassword(pass);
            }else ck = false;

            obj.put("Result", ck);
            if(ck){
                obj.put("Name" , name);
                obj.put("Login" , login);
                obj.put("Password" , pass);
            }
        }catch (Exception exc){}
        return obj;
    }

    private JSONObject saveDataBase(){
        JSONObject obj = new JSONObject();
        try{
            DataBaseIO.saveJson(path , this.db.toJson());
            obj.put("Result" , true);
        }catch (Exception exc){}
        return obj;
    }

    private JSONObject closeDataBase(){
        JSONObject obj = new JSONObject();
        try{
            this.used_tables.remove(this.db.getName());
            this.db = null;
            this.path = null;
            obj.put("Result" , true);
        }catch (Exception exc){}
        return obj;
    }

    private String run_command(String command){
        String response = "";
        try {
            JSONObject obj = new JSONObject(command);
            if(obj.has("Command")){
                JSONObject tmp = new JSONObject();
                command = obj.getString("Command");
                if(command.equals("getDataBases")){
                    tmp.put("getDataBases", getDataBases());
                }else if(command.equals("openDataBase")){
                    String name = obj.getJSONObject("Params").getString("Name");
                    String login = obj.getJSONObject("Params").getString("Login");
                    String pass = obj.getJSONObject("Params").getString("Password");
                    tmp.put("openDataBase" , openDataBase(name , login , pass));
                }else if(command.equals("showTable")){
                    String name = obj.getJSONObject("Params").getString("Name");
                    tmp.put("showTable" ,  showTable(name));
                }else if(command.equals("setMainData")){
                    String name = obj.getJSONObject("Params").getString("Name");
                    String login = obj.getJSONObject("Params").getString("Login");
                    String pass = obj.getJSONObject("Params").getString("Password");
                    tmp.put("setMainData" , setMainData(name , login , pass));
                }else if(command.equals("saveDataBase")){
                    tmp.put("saveDataBase" , saveDataBase());
                }else if(command.equals("closeDataBase")){
                    tmp.put("closeDataBase" , closeDataBase());
                }

                response = tmp.toString();
            }
        }catch (Exception exc){}
        return response;
    }

    @Override
    public void run() {
        try{
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(
                    new OutputStreamWriter(socket.getOutputStream()));

            while (true){
                String command = "";
                int cnt = 0;
                do {
                    String cur = in.readLine();
                    command += cur;
                    for (char c : cur.toCharArray())
                        if (c == '{') ++cnt;
                        else if (c == '}') --cnt;
                }while (cnt != 0);
                System.out.println("Get: " + command + " from " + socket.getPort());
                String response = run_command(command);
                out.print(response + "\n");
                out.flush();
                System.out.println("Response: " + response + " to " + socket.getPort());
            }
        }catch (Exception e){}
    }
}
