package dataset;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;

public class User implements Serializable {
	private static final long serialVersionUID = 1L;

	@Override
	public String toString() {
		return String.format("[%s] - [%s] ", type, status) + id;
	}
	
	public String id;
	public String password;
	public String type;
	public int lang;
	
	public String status = "";

	public void getResult(ResultSet r) throws SQLException {
		id = r.getString("Id");
		type = r.getString("Type");
		lang = r.getInt("Lang");
	}
}
