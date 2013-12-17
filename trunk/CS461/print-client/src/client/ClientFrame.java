package client;

import java.awt.CardLayout;
import java.awt.Dimension;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class ClientFrame extends JFrame {
	private static final long serialVersionUID = 1L;	
	
	private CardLayout cardLayout = new CardLayout();
	private JPanel cards;
	
	public void ChangePage(String page) {
		cardLayout.show(cards, page);
	}
		
	public ClientFrame() {
		super();
		
		setMinimumSize(new Dimension(800, 600));
		
		addWindowListener(new WindowListener() {			
			@Override
			public void windowOpened(WindowEvent e) {}
			
			@Override
			public void windowIconified(WindowEvent e) {}
			
			@Override
			public void windowDeiconified(WindowEvent e) {}
			
			@Override
			public void windowDeactivated(WindowEvent e) {}
			
			@Override
			public void windowClosing(WindowEvent e) {
				dispose();				
			}
			
			@Override
			public void windowClosed(WindowEvent e) {}
			
			@Override
			public void windowActivated(WindowEvent e) {}
		});
		
		cards = new JPanel(cardLayout);
		
		cards.add(new LoginPage(), "Login");
		cards.add(new SignupPage(), "Signup");
		cards.add(new ProgrammerPage(), "Programmer");
		cards.add(new ManagerPage(), "Manager");
		cards.add(new CreateProjectPage(), "CreateProject");
		cards.add(new CreateTaskPage(), "CreateTask");
		cards.add(new ProgrammersListPage(), "ProgrammersList");
		cards.add(new RequestedTasksPage(), "RequestedTasks");
		
		cards.add(new TaskDetailPage(), "TaskDetail");
		
		cardLayout.show(cards, "Login");
				
		getContentPane().add(cards);
		
		pack();
	}
}
