package dataset;

import java.io.Serializable;
import java.util.List;

public class Project implements Serializable {
	private static final long serialVersionUID = 1L;
	
	@Override
	public String toString() {
		return name;
	}
	
	public int Id;
	public String name;
	public String managerId;
	public List<Task> tasks;
	
}
