package client;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.border.LineBorder;

public class ProgrammerPage extends JSplitPane {
	private static final long serialVersionUID = 1L;
	
	public ProgrammerPage() {
		super(JSplitPane.VERTICAL_SPLIT);
		
		GridBagConstraints gbc;
		
		JPanel jobListPanel = new JPanel(new GridBagLayout());
		jobListPanel.setBorder(new LineBorder(Color.blue));
		
		JLabel lblTree = new JLabel("Jobs");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.weightx = 1.0;
		jobListPanel.add(lblTree, gbc);
		
		JTree tree = new JTree();
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 1;
		gbc.weightx = 1.0;
		gbc.weighty = 1.0;
		gbc.fill = GridBagConstraints.BOTH;
		jobListPanel.add(tree, gbc);
				
		JPanel myInfoPanel = new JPanel();
		myInfoPanel.setBorder(new LineBorder(Color.red));
				
		myInfoPanel.add(new JLabel("MY Name is"));

		final JSplitPane upperPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, jobListPanel, myInfoPanel);
		upperPane.setDividerSize(0);
		setLeftComponent(upperPane);
		
		JPanel lowerPane = new JPanel(new FlowLayout());
		
		lowerPane.add(new JButton("Search Jobs"));
		lowerPane.add(new JButton("My Requested Jobs"));
		
		JButton btnExit = new JButton("Exit");
		btnExit.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(getTopLevelAncestor() instanceof JFrame) {
					JFrame f = (JFrame) getTopLevelAncestor();
					f.setVisible(false);
					f.dispose();
				}
			}
		});		
		lowerPane.add(btnExit);
		
		setRightComponent(lowerPane);
		setDividerSize(0);

		addComponentListener(new ComponentListener() {
			
			@Override
			public void componentShown(ComponentEvent e) {}
			
			@Override
			public void componentResized(ComponentEvent e) {
				setDividerLocation(getHeight() - 40);
			}
			
			@Override
			public void componentMoved(ComponentEvent e) {}
			
			@Override
			public void componentHidden(ComponentEvent e) {}
		});
		
		upperPane.addComponentListener(new ComponentListener() {
			
			@Override
			public void componentShown(ComponentEvent e) {}
			
			@Override
			public void componentResized(ComponentEvent e) {
				upperPane.setDividerLocation(getWidth() - 150);
			}
			
			@Override
			public void componentMoved(ComponentEvent e) {}
			
			@Override
			public void componentHidden(ComponentEvent e) {}
		});
	}

}
