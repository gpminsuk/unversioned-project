package client;

import java.awt.Color;
import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.rmi.RemoteException;

import javax.imageio.ImageIO;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.LineBorder;

public class LoginPage extends JPanel {
	private static final long serialVersionUID = 1L;
	private int loginTries = 0;

	public LoginPage() {
		super();
		
		setBackground(Color.white);
		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		setBorder(new LineBorder(Color.red));

		add(Box.createVerticalStrut(100));
		
		JPanel loginPanel = new JPanel(new GridBagLayout());
		loginPanel.setBackground(Color.white);

		GridBagConstraints gbc;
		
		BufferedImage myPicture = null;
		try {
			myPicture = ImageIO.read(new File("./res/logo.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		JLabel lblLogo = new JLabel(new ImageIcon(myPicture));
		lblLogo.setAlignmentX(Component.CENTER_ALIGNMENT);
		add(lblLogo);
		
		JLabel id = new JLabel("ID : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 0;
		gbc.gridy = 0;
		loginPanel.add(id);
		
		final JTextField idinput = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 1;
		gbc.gridy = 0;
		loginPanel.add(idinput);
		
		JLabel password = new JLabel("PASSWORD : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 0;
		gbc.gridy = 1;
		loginPanel.add(password, gbc);
		
		final JTextField idpassword = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 1;
		gbc.gridy = 1;
		loginPanel.add(idpassword, gbc);

		final JLabel info = new JLabel("Login...");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.gridx = 0;
		gbc.gridy = 2;
		gbc.gridwidth = 2;
		loginPanel.add(info, gbc);
		
		JPanel btnPane = new JPanel();
		btnPane.setBackground(Color.white);
		
		JButton btnLogin = new JButton("Login");
		btnLogin.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					Com.inst.me = Com.get().login(idinput.getText(), idpassword.getText());
					if(Com.me() == null) {
						info.setText("Login Failed (" + (++loginTries) + ")");
					}
					else {
						((ClientFrame)getTopLevelAncestor()).ChangePage(Com.me().type);
					}
				} catch (RemoteException ex) {
					ex.printStackTrace();
				}
			}
		});
		btnPane.add(btnLogin);
		
		JButton btnSignup = new JButton("Signup");
		btnSignup.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				((ClientFrame)getTopLevelAncestor()).ChangePage("Signup");
			}
		});
		btnPane.add(btnSignup);
		
		JButton btnQuit = new JButton("Quit");	
		btnQuit.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(getTopLevelAncestor() instanceof JFrame) {
					JFrame frame = (JFrame) getTopLevelAncestor();
					frame.setVisible(false);
					frame.dispose();	
				}				
			}
		});
		btnPane.add(btnQuit);
		
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.insets = new Insets(40, 0, 0, 0);
		gbc.gridx = 0;
		gbc.gridwidth = 2;
		gbc.gridy = 3;
		loginPanel.add(btnPane, gbc);
		
		add(loginPanel);		
	}
}
