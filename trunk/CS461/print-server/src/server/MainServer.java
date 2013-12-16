package server;

import java.net.URL;
import java.net.URLClassLoader;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.sql.SQLException;

public class MainServer {

	public static void main(String[] args) {
		ClassLoader s = ClassLoader.getSystemClassLoader();
		URL[] urls = ((URLClassLoader)s).getURLs();
		for(URL u : urls) {
			System.out.println(u.getFile());
		}
		
		
		
		try {
			DB.getConnection();
		} catch (SQLException e1) {
			e1.printStackTrace();
		}
		try {
			LocateRegistry.createRegistry(1099);
			Registry registry = LocateRegistry.getRegistry();
			registry.rebind("com", new Com());
		} catch (RemoteException e) {
			System.err.println("Something wrong happended on the remote end");
			e.printStackTrace();
			System.exit(-1);
		}
		System.out.println("The print server is ready");
	}
}
