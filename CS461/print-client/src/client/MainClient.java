package client;

import java.rmi.RMISecurityManager;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import common.ICom;


public class MainClient {

    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java " + MainClient.class + " <registry name>");
            return;
        }
        System.setSecurityManager(new RMISecurityManager());
    	Com.inst = new Com(args);                
        new ClientFrame().setVisible(true);
    }
}
