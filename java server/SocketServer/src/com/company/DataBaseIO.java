package com.company;


import org.json.JSONObject;

import java.io.*;
import java.net.*;
import java.util.*;

public class DataBaseIO {
    public static String readJson(String path){
        String content = "";
        try {
            FileInputStream stream = new FileInputStream(path);
            byte[] arr = stream.readAllBytes();
            content = new String(arr);
        }catch (Exception exc){}
        return content;
    }

    public static void saveJson(String path , JSONObject obj){
        try{
            FileOutputStream stream = new FileOutputStream(path);
            byte[] arr = new byte[obj.toString().length()];
            int i = 0;
            for(char c : obj.toString().toCharArray())arr[i++] = (byte)c;
            stream.write(arr);
            stream.flush();
        }catch (Exception exc){}
    }
}
