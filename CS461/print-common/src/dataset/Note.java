package dataset;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;

public class Note implements Serializable {
	private static final long serialVersionUID = 1L;

	@Override
	public String toString() {
		return String.format("[%s] ", userName) + content;
	}
	
	public int id;
	public String content;
	public String userName;
	public String time;
	public int taskId;

	public void getResult(ResultSet r) throws SQLException {
		
	}
}
