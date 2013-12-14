package common;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

import dataset.Project;
import dataset.Task;
import dataset.User;

public interface ICom extends Remote {
	public String ping() throws RemoteException;
	
	public User login(String id, String pwd) throws RemoteException;
	public boolean signup(User u) throws RemoteException;
	
	public List<Project> getProjects(String id) throws RemoteException;
	public List<User> getProgrammers() throws RemoteException;
	public List<Task> getManagerRequestedTasks(String id) throws RemoteException;
	
	public boolean createProject(Project p) throws RemoteException;
	public boolean createTask(Task j) throws RemoteException;
	
	public boolean acceptTask(int taskId, String userId) throws RemoteException;
}