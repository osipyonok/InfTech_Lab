package com.company;
import java.io.*;
import java.net.*;
import java.util.*;

public class Main {

    public static void main(String[] args) {
	    int port = 1234;
	    Set<String> used_tables = new HashSet<>();
	    try{
	        ServerSocket server = new ServerSocket(port);
            System.out.println("Waiting for a client...");

            for(;;){
                Socket socket = server.accept();
                System.out.println("Client connected");
                new Thread(new Connection(used_tables, socket)).start();
            }

        }catch(Exception e){
	        e.printStackTrace();
        }
    }
}
