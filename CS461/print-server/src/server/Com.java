package server;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import common.ICom;

import dataset.Note;
import dataset.Project;
import dataset.Task;
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
	        selectUser = con.prepareStatement(selectString);

	        selectUser.setString(1, id);
	        selectUser.setString(2, pwd);
	        
	        ResultSet result = selectUser.executeQuery();
	        result.last();
	        if(result.getRow() > 0) {
	        	result.first();
	        	user = new User();
	        	user.getResult(result);
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

	@Override
	public boolean signup(User u) throws RemoteException {
		PreparedStatement insertUser = null;

	    String insertString = "insert into users (Id, Password, Type, Lang) values (?, ?, ?, ?)";
	    try {
		    Connection con = DB.getConnection();
	        insertUser = con.prepareStatement(insertString);

	        insertUser.setString(1, u.id);
	        insertUser.setString(2, u.password);
	        insertUser.setString(3, u.type);
	        insertUser.setInt(4, u.lang);
	        
	        return insertUser.execute();
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(insertUser != null) {
	    		try {
					insertUser.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
	    return false;
	}

	@Override
	public boolean createProject(Project p) throws RemoteException {
		PreparedStatement insertProject = null;

	    String insertString = "insert into projects (Name, ManagerId) values (?, ?)";
	    try {
		    Connection con = DB.getConnection();
	        insertProject = con.prepareStatement(insertString);

	        insertProject.setString(1, p.name);
	        insertProject.setString(2, p.managerId);
	        
	        int result = insertProject.executeUpdate(); 
	        return result == 1;
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(insertProject != null) {
	    		try {
					insertProject.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
	    return false;
	}

	@Override
	public boolean createTask(Task j) throws RemoteException {
		PreparedStatement insertProject = null;

	    String insertString = "insert into tasks (Name, Status, Description, ProjectId) values (?, ?, ?, ?)";
	    try {
		    Connection con = DB.getConnection();
	        insertProject = con.prepareStatement(insertString);

	        insertProject.setString(1, j.name);
	        insertProject.setString(2, "Open");
	        insertProject.setString(3, j.desc);
	        insertProject.setInt(4, j.projectId);
	        
	        int result = insertProject.executeUpdate();
	        return result == 1;
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(insertProject != null) {
	    		try {
					insertProject.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
	    return false;
	}
	
	public List<Task> getAssignedTasksInProject(int id, String userId) {
		List<Task> ret = new ArrayList<Task>();

		PreparedStatement selectTasks = null;

	    String selectString = "select * from tasks where ProjectId = ? and Id in (select TaskId from task_programmer where UserId = ?)";
	    try {
		    Connection con = DB.getConnection();
	        selectTasks = con.prepareStatement(selectString);

	        selectTasks.setInt(1, id);
	        selectTasks.setString(2, userId);
	        
	        ResultSet result = selectTasks.executeQuery();
	        while(result.next()) {
	        	Task t = new Task();
	        	t.getResult(result);
	        	ret.add(t);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectTasks != null) {
	    		try {
					selectTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}
	
	public List<Task> getTasksInProject(int id) {
		List<Task> ret = new ArrayList<Task>();

		PreparedStatement selectTasks = null;

	    String selectString = "select * from tasks where ProjectId = ?";
	    try {
		    Connection con = DB.getConnection();
	        selectTasks = con.prepareStatement(selectString);

	        selectTasks.setInt(1, id);
	        
	        ResultSet result = selectTasks.executeQuery();
	        while(result.next()) {
	        	Task t = new Task();
	        	t.getResult(result);
	        	ret.add(t);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectTasks != null) {
	    		try {
					selectTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}
	
	@Override
	public List<Project> getWorkingProjects(String id) throws RemoteException {
		List<Project> ret = new ArrayList<Project>();

		PreparedStatement selectProjects = null;

	    String selectString = "select * from projects where Id in (select ProjectId from tasks where Id in (select TaskId from task_programmer where UserId = ?));";
	    try {
		    Connection con = DB.getConnection();
	        selectProjects = con.prepareStatement(selectString);

	        selectProjects.setString(1, id);
	        
	        ResultSet result = selectProjects.executeQuery();
	        while(result.next()) {
	        	Project p = new Project();
	        	p.Id = result.getInt("Id");
	        	p.name = result.getString("Name");
	        	p.managerId = result.getString("ManagerId");
	        	p.tasks = getAssignedTasksInProject(p.Id, id);
	        	ret.add(p);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectProjects != null) {
	    		try {
					selectProjects.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}
	
	@Override
	public List<Project> getProjects(String id) throws RemoteException {
		List<Project> ret = new ArrayList<Project>();

		PreparedStatement selectProjects = null;

	    String selectString = "select * from projects where ManagerId = ?";
	    try {
		    Connection con = DB.getConnection();
	        selectProjects = con.prepareStatement(selectString);

	        selectProjects.setString(1, id);
	        
	        ResultSet result = selectProjects.executeQuery();
	        while(result.next()) {
	        	Project p = new Project();
	        	p.Id = result.getInt("Id");
	        	p.name = result.getString("Name");
	        	p.managerId = result.getString("ManagerId");
	        	p.tasks = getTasksInProject(p.Id);
	        	ret.add(p);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectProjects != null) {
	    		try {
					selectProjects.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}

	@Override
	public List<User> getRequestProgrammers(int taskId) throws RemoteException {
		List<User> ret = new ArrayList<User>();

		PreparedStatement selectProgrammers = null;

	    String selectString = "select *, if(? in (select TaskId from task_requested_by_user), 'Requested', 'None') as Status from users where Type = 'Programmer'";
	    try {
		    Connection con = DB.getConnection();
	        selectProgrammers = con.prepareStatement(selectString);
	        
	        selectProgrammers.setInt(1, taskId);
	        
	        ResultSet result = selectProgrammers.executeQuery();
	        while(result.next()) {
	        	User u = new User();
	        	u.getResult(result);
	        	u.status = result.getString("Status");
	        	ret.add(u);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectProgrammers != null) {
	    		try {
					selectProgrammers.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}

	public List<User> getTaskRequestedUsers(int TaskId) {
		List<User> ret = new ArrayList<User>();

		PreparedStatement selectUsers = null;

	    String selectString = "select * from users where Id in (select UserId from task_requested_by_user where TaskId = ?)";
	    try {
		    Connection con = DB.getConnection();
	        selectUsers = con.prepareStatement(selectString);
	        
	        selectUsers.setInt(1, TaskId);
	        
	        ResultSet result = selectUsers.executeQuery();
	        while(result.next()) {	        	
	        	User u = new User();
	        	u.getResult(result);
	        	ret.add(u);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectUsers != null) {
	    		try {
					selectUsers.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}

	@Override
	public List<Task> getProgrammerRequestedTasks(String id) throws RemoteException {
		List<Task> ret = new ArrayList<Task>();

		PreparedStatement selectRequestedTasks = null;

	    String selectString = "select * from tasks where Id = (select TaskId from task_requested_by_user " +
	    		"where UserId = ?)";
	    try {
		    Connection con = DB.getConnection();
	        selectRequestedTasks = con.prepareStatement(selectString);
	        
	        selectRequestedTasks.setString(1, id);
	        
	        ResultSet result = selectRequestedTasks.executeQuery();
	        while(result.next()) {	        	
	        	Task u = new Task();
	        	u.getResult(result);
	        	ret.add(u);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectRequestedTasks != null) {
	    		try {
					selectRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}

	@Override
	public List<Task> getManagerRequestedTasks(String id) throws RemoteException {
		List<Task> ret = new ArrayList<Task>();

		PreparedStatement selectRequestedTasks = null;

	    String selectString = "select * from tasks where Id = (select TaskId from task_requested_by_user " +
	    		"where (select Type from users where Id = UserId) = 'Manager') and" +
	    		"(select ManagerId from projects where Id = ProjectId) = ?";
	    try {
		    Connection con = DB.getConnection();
	        selectRequestedTasks = con.prepareStatement(selectString);
	        
	        selectRequestedTasks.setString(1, id);
	        
	        ResultSet result = selectRequestedTasks.executeQuery();
	        while(result.next()) {	        	
	        	Task u = new Task();
	        	u.getResult(result);
	        	u.requestedProgrammers = getTaskRequestedUsers(result.getInt("Id"));
	        	ret.add(u);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectRequestedTasks != null) {
	    		try {
					selectRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}

	@Override
	public boolean acceptTask(int taskId, String userId) {
		PreparedStatement updateRequestedTasks = null;
		PreparedStatement insertRequestedTasks = null;
		PreparedStatement deleteRequestedTasks = null;

		String updateString = "update tasks set Status = 'Assigned' where Id = ?";
		String insertString = "insert into task_programmer (TaskId, UserId) values(?, ?)";
	    String deleteString = "delete from task_requested_by_user where TaskId = ?";
	    try {
		    Connection con = DB.getConnection();
		    
		    updateRequestedTasks = con.prepareStatement(updateString);
		    updateRequestedTasks.setInt(1, taskId);
		    
		    int result = updateRequestedTasks.executeUpdate();
		    if(result != 1) {
		    	return false;
		    }
		    
		    insertRequestedTasks = con.prepareStatement(insertString);
		    insertRequestedTasks.setInt(1, taskId);
		    insertRequestedTasks.setString(2, userId);
		    
		    result = insertRequestedTasks.executeUpdate();
		    if(result != 1) {
		    	return false;
		    }
		    
	        deleteRequestedTasks = con.prepareStatement(deleteString);
	        
	        deleteRequestedTasks.setInt(1, taskId);
	        
	        result = deleteRequestedTasks.executeUpdate();
	        return result == 1;
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(updateRequestedTasks != null) {
	    		try {
	    			updateRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    	if(insertRequestedTasks != null) {
	    		try {
	    			insertRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    	if(deleteRequestedTasks != null) {
	    		try {
					deleteRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return false;
	}

	@Override
	public boolean requestTask(int taskId, String userId) throws RemoteException {
		PreparedStatement insertRequestedTasks = null;

		String insertString = "insert into task_requested_by_user (TaskId, UserId) values(?, ?)";
	    try {
		    Connection con = DB.getConnection();
		    
		    insertRequestedTasks = con.prepareStatement(insertString);
		    insertRequestedTasks.setInt(1, taskId);
		    insertRequestedTasks.setString(2, userId);
		    
		    int result = insertRequestedTasks.executeUpdate();
		    return result == 1;
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(insertRequestedTasks != null) {
	    		try {
	    			insertRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return false;
	}

	@Override
	public boolean cancelTask(int taskId, String userId) throws RemoteException {
		PreparedStatement deleteRequestedTasks = null;

	    String deleteString = "delete from task_requested_by_user where TaskId = ? and UserId = ?";
	    try {
		    Connection con = DB.getConnection();
		    
		    deleteRequestedTasks = con.prepareStatement(deleteString);
		    deleteRequestedTasks.setInt(1, taskId);
		    deleteRequestedTasks.setString(2, userId);
		    
		    int result = deleteRequestedTasks.executeUpdate();
		    return result == 1;
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(deleteRequestedTasks != null) {
	    		try {
	    			deleteRequestedTasks.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return false;
	}

	@Override
	public boolean addNote(int taskId, String userId, String note) throws RemoteException {
		PreparedStatement addNotequery = null;
		
		String addNoteString = "insert into notes (idnotes, Contents, Uploadedtime) values (?, ?, ?)";

		try {
		    Connection con = DB.getConnection();
	        addNotequery = con.prepareStatement(addNoteString);

	        addNotequery.setInt(1, taskId);
	        addNotequery.setString(2, userId);
	        addNotequery.setString(3, note);
	        
	        return addNotequery.execute();
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(addNotequery != null) {
	    		try {
					addNotequery.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
	    return false;
	}

	@Override
	public List<Note> getNotes(int idnotes) throws RemoteException {
		List<Note> ret = new ArrayList<Note>();

		PreparedStatement selectNotes = null;

	    String selectString = "select * from s where idnotes = ?";
	    try {
		    Connection con = DB.getConnection();
	        selectNotes = con.prepareStatement(selectString);

	        selectNotes.setInt(1, idnotes);
	        
	        ResultSet result = selectNotes.executeQuery();
	        while(result.next()) {
	        	Note n = new Note();
	        	n.id = result.getInt("idnotes");
	        	n.taskId = result.getInt("tasks_Id");
	        	n.content = result.getString("Contents");
	        	n.time = result.getString("Uploadedtime");
	        	ret.add(n);
	        }
	    }
	    catch(Exception e) {
	    	e.printStackTrace();	    	
	    }
	    finally {
	    	if(selectNotes != null) {
	    		try {
					selectNotes.close();
				} catch (SQLException e) {
					e.printStackTrace();
				}
	    	}
	    }
		return ret;
	}
}
