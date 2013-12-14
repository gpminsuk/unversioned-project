package client;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.rmi.RemoteException;

import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;

import dataset.User;

public class SignupPage extends JPanel {
	private static final long serialVersionUID = 1L;

	private CardLayout detailCardLayout = new CardLayout();
	private JPanel detailCards;
	private int signupTry = 0;

	public SignupPage() {
		super(new GridBagLayout());

		setBackground(Color.white);

		GridBagConstraints gbc;

		JLabel type = new JLabel("TYPE : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		add(type, gbc);

		final JRadioButton radioProgrammer = new JRadioButton("Programmer");
		radioProgrammer.setBackground(Color.white);
		radioProgrammer.setSelected(true);
		radioProgrammer.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				if (e.getStateChange() == ItemEvent.SELECTED) {
					detailCardLayout.show(detailCards, "Programmer");
				}
			}
		});
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		add(radioProgrammer, gbc);

		final JRadioButton radioManager = new JRadioButton("Manager");
		radioManager.setBackground(Color.white);
		radioManager.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				if (e.getStateChange() == ItemEvent.SELECTED) {
					detailCardLayout.show(detailCards, "Manager");
				}
			}
		});
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		add(radioManager, gbc);

		ButtonGroup group = new ButtonGroup();
		group.add(radioManager);
		group.add(radioProgrammer);

		JLabel id = new JLabel("ID : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		add(id, gbc);

		final JTextField idinput = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		add(idinput, gbc);

		JLabel password = new JLabel("PASSWORD : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		add(password, gbc);

		final JTextField passwordinput = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		add(passwordinput, gbc);

		detailCards = new JPanel(detailCardLayout);

		JPanel managerDetail = new JPanel(new GridBagLayout());
		managerDetail.setBackground(Color.white);

		JLabel company = new JLabel("COMPANY : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.weighty = 1.0;
		managerDetail.add(company, gbc);

		final JTextField companyinput = new JTextField(20);
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		managerDetail.add(companyinput, gbc);

		JPanel programmerDetail = new JPanel(new GridBagLayout());
		programmerDetail.setBackground(Color.white);

		JLabel lang = new JLabel("LANGUAGE : ");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.weightx = 1.0;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		programmerDetail.add(lang, gbc);

		final JCheckBox langc = new JCheckBox("C/C++");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		programmerDetail.add(langc, gbc);

		final JCheckBox langJava = new JCheckBox("JAVA");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		programmerDetail.add(langJava, gbc);

		final JCheckBox langPython = new JCheckBox("Python");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		programmerDetail.add(langPython, gbc);

		detailCards.add(managerDetail, "Manager");
		detailCards.add(programmerDetail, "Programmer");

		detailCardLayout.show(detailCards, "Programmer");

		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		add(detailCards, gbc);

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
				info.setText("Try to register your id");
				try {
					User u = new User();
					
					u.id = idinput.getText();
					u.password = passwordinput.getText();
					if(radioManager.isSelected()) {
						u.type = "Manager";
					}
					else {
						u.type = "Programmer";
					}
					u.lang = 0;
					if(langc.isSelected()) {
						u.lang |= 0x000001;
					}
					if(langJava.isSelected()) {
						u.lang |= 0x000002;
					}
					if(langPython.isSelected()) {
						u.lang |= 0x000004;
					}

					if (Com.get().signup(u)) {
						((ClientFrame)getTopLevelAncestor()).ChangePage("Login");
					} else {
						info.setText("Singup Failed (" + (++signupTry) + ")");
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
				((ClientFrame)getTopLevelAncestor()).ChangePage("Login");
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
