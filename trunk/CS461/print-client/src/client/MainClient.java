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
        try {
            Registry registry = LocateRegistry.getRegistry(args[0]);
            ICom com = (ICom)registry.lookup("com");
            new ClientFrame(com).setVisible(true);
        } catch (Exception e) {        	
            e.printStackTrace();
        }
    }
}
