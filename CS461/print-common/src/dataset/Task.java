package dataset;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

public class Task implements Serializable {
	private static final long serialVersionUID = 1L;

	@Override
	public String toString() {
		return String.format("[%7s]", status) + name;
	}
	
	public int Id;
	public String name;
	public String status;
	public String desc;
	public int projectId;
	public int price;
	public List<User> requestedProgrammers;
	
	public void getResult(ResultSet r) throws SQLException {
		Id = r.getInt("Id");
		name = r.getString("Name");
		status = r.getString("Status");
		desc = r.getString("Description");
		projectId = r.getInt("ProjectId");
		price = r.getInt("Price");
	}
}
