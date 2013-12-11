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
        ICom com = null;
        try {
            Registry registry = LocateRegistry.getRegistry(args[0]);
            com = (ICom)registry.lookup("com");
        } catch (Exception e) {        	
            e.printStackTrace();
        }
        
        new ClientFrame(com).setVisible(true);
    }
}
