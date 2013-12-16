package client;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import common.ICom;

import dataset.Project;
import dataset.Task;
import dataset.User;

public class Com {
	public static Com inst;
	public ICom com = null;
	public User me = null;
	public Project selectedProject = null;
	public Task selectedTask = null;
	
	public Com(String[] args) {
		Registry registry;
		try {
			registry = LocateRegistry.getRegistry(args[0]);
	        com = (ICom)registry.lookup("com");
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (NotBoundException e) {
			e.printStackTrace();
		}
	}
	
	public static ICom get() {
		return inst.com;
	}
	
	public static User me() {
		return inst.me;
	}
}
