package server;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import common.ICom;
import dataset.User;

public class Com extends UnicastRemoteObject implements ICom {
	private static final long serialVersionUID = 1L;

	public Com() throws RemoteException {
	}

	@Override
	public String ping() throws RemoteException {
		return "Hi, I'm alive";
	}

	@Override
	public User login(String id, String pwd) throws RemoteException {
		PreparedStatement selectUser = null;

	    String selectString = "select * from users where Id = ? and Password = ?";
	    User user = null;
	    try {
		    Connection con = DB.getConnection();
	        con.setAutoCommit(false);
	        selectUser = con.prepareStatement(selectString);

	        selectUser.setString(1, id);
	        selectUser.setString(2, pwd);
	        
	        ResultSet result = selectUser.executeQuery();
	        result.last();
	        if(result.getRow() > 0) {
	        	result.first();
	        	user = new User();
	        	user.id = result.getString("Id");
	        	user.password = result.getString("Password");
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectUser != null) {
	    		try {
					selectUser.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
	    return user;
	}
}
