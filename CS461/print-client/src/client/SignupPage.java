package client;

import java.awt.CardLayout;
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

public class SignupPage extends JPanel {
	private static final long serialVersionUID = 1L;

	public SignupPage(final CardLayout pageLayout, final JPanel pages) {
		super(new GridBagLayout());
		
		setBackground(Color.white);

		GridBagConstraints gbc;
		
		JLabel id = new JLabel("ID : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 0;
		gbc.gridy = 0;
		add(id);
		
		final JTextField idinput = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 1;
		gbc.gridy = 0;
		add(idinput);
		
		JLabel password = new JLabel("PASSWORD : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 0;
		gbc.gridy = 1;
		add(password, gbc);
		
		final JTextField idpassword = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 1;
		gbc.gridy = 1;
		add(idpassword, gbc);
		
		final JLabel info = new JLabel("Signup...");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.gridx = 0;
		gbc.gridy = 2;
		gbc.gridwidth = 2;
		add(info, gbc);
		
		JPanel btnPane = new JPanel();
		btnPane.setBackground(Color.white);
		
		JButton btnRegister = new JButton("Register");
		btnRegister.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				info.setText("Try to register your id");
				/*
				try {
					User u = new User();
					
					if(com.signup(u)) {
						cardLayout.show(cards, "Login");
					}
					else {
						
					}
				} catch (RemoteException ex) {
					ex.printStackTrace();
				}*/
			}
		});
		btnPane.add(btnRegister);
		
		JButton btnCancel = new JButton("Cancel");
		btnCancel.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				pageLayout.show(pages, "Login");
			}
		});
		btnPane.add(btnCancel);
		
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.insets = new Insets(40, 0, 0, 0);
		gbc.gridx = 0;
		gbc.gridwidth = 2;
		gbc.gridy = 3;
		add(btnPane, gbc);
	}
}
