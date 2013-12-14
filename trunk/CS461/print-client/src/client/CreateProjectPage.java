package client;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.rmi.RemoteException;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import dataset.Project;

public class CreateProjectPage extends JPanel {
	private static final long serialVersionUID = 1L;

	private int createTry = 0;

	public CreateProjectPage() {
		super(new GridBagLayout());

		setBackground(Color.white);

		GridBagConstraints gbc;

		JLabel name = new JLabel("Name : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		add(name, gbc);

		final JTextField nameinput = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		add(nameinput, gbc);

		final JLabel info = new JLabel("Fill out the form...");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbc.insets = new Insets(10, 0, 0, 0);
		add(info, gbc);

		JPanel btnPane = new JPanel();
		btnPane.setBackground(Color.white);

		JButton btnRegister = new JButton("Register");
		btnRegister.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					Project p = new Project();
					
					p.name = nameinput.getText();
					p.managerId = Com.me().id;
					
					if (Com.get().createProject(p)) {
						((ClientFrame)getTopLevelAncestor()).ChangePage("Manager");
					} else {
						info.setText("Project Creation Failed (" + (++createTry) + ")");
					}
				} catch (RemoteException ex) {
					ex.printStackTrace();
				}

			}
		});
		btnPane.add(btnRegister);

		JButton btnCancel = new JButton("Cancel");
		btnCancel.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				((ClientFrame)getTopLevelAncestor()).ChangePage("Manager");
			}
		});
		btnPane.add(btnCancel);

		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.insets = new Insets(20, 0, 0, 0);
		gbc.gridwidth = 2;
		add(btnPane, gbc);
	}
}
