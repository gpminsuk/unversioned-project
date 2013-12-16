package server;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class DB {
	static String userName = "root";
	static String password = "Dpflspf1!";
	static String dbName = "project461";
	
	static Connection con = null;
	
	public static Connection getConnection() throws SQLException {
		if(con == null) {
			con = DriverManager.getConnection("jdbc:mysql://localhost:3306/project461", userName, password);
			System.out.println("Connected to database");
		}
		return con;
	}
}
