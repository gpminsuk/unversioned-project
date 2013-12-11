package client;

import java.awt.Dimension;
import java.rmi.RemoteException;

import javax.swing.JFrame;

import common.ICom;

public class ClientFrame extends JFrame {
	private static final long serialVersionUID = 1L;
	
	ICom com;
	
	public ClientFrame(ICom com) {
		super();
		this.com = com;
		
		setSize(new Dimension(800, 600));
		
		try {
			System.out.println(com.login("kms0627", "0627"));
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}
}
