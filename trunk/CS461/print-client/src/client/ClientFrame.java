package client;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTextField;

import common.ICom;

public class ClientFrame extends JFrame {
	private static final long serialVersionUID = 1L;
	
	public final ICom com;
	
	private CardLayout cardLayout = new CardLayout();
	private JPanel cards;
	
	public JPanel setupSignupPanel() {
		return new SignupPage(cardLayout, cards);
	}
	
	public JPanel setupLoginPanel() {
		return new LoginPage(cardLayout, cards);
	}
	
	public JComponent setupJobDetailsPage() {
		return new JobDetailPage();
	}
	
	public JComponent setupJobsPage() {
		return new JobDetailPage();
	}
	
	public ClientFrame(ICom comm) {
		super();
		this.com = comm;
		
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
		
		cards.add(setupLoginPanel(), "Login");
		cards.add(setupSignupPanel(), "Signup");
		cards.add(setupJobsPage(), "Jobs");
		
		cardLayout.show(cards, "Login");
				
		getContentPane().add(cards);
		
		pack();
	}
}
