package client;

import java.rmi.RMISecurityManager;


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
