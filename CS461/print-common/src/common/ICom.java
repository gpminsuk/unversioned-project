package common;

import java.rmi.Remote;
import java.rmi.RemoteException;

import dataset.User;

public interface ICom extends Remote {
	public String ping() throws RemoteException;
	
	public User login(String id, String pwd) throws RemoteException;
}