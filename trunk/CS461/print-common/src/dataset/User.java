package dataset;

import java.io.Serializable;

public class User implements Serializable {
	private static final long serialVersionUID = 1L;

	@Override
	public String toString() {
		return String.format("[%s]", type) + id;
	}
	
	public String id;
	public String password;
	public String type;
	public int lang;
}
