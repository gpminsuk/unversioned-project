package client;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.rmi.RemoteException;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.border.LineBorder;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import dataset.Project;

public class ProgrammersListPage extends JSplitPane {
	private static final long serialVersionUID = 1L;
	
	public ProgrammersListPage() {
		super(JSplitPane.VERTICAL_SPLIT);
		
		GridBagConstraints gbc;
		
		JPanel projectListPanel = new JPanel(new GridBagLayout());
		projectListPanel.setBorder(new LineBorder(Color.blue));
		
		JLabel lblTree = new JLabel("Projects");
		gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.weightx = 1.0;
		projectListPanel.add(lblTree, gbc);
		
		final JTree tree = new JTree();
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 1;
		gbc.weightx = 1.0;
		gbc.weighty = 1.0;
		gbc.fill = GridBagConstraints.BOTH;
		projectListPanel.add(tree, gbc);
		
		addComponentListener(new ComponentListener() {			
			@Override
			public void componentShown(ComponentEvent e) {
				tree.setModel(new TreeModel() {
					List<Project> projects;
					
					@Override
					public void valueForPathChanged(TreePath path, Object newValue) {}
					
					@Override
					public void removeTreeModelListener(TreeModelListener l) {}
					
					@Override
					public boolean isLeaf(Object node) {
						if(node instanceof String && node.equals("Projects")) {
							return false;					
						}
						if(node instanceof Project) {
							return false;
						}
						return true;
					}
					
					@Override
					public Object getRoot() {
						try {
							if(Com.me() != null) {
								projects = Com.get().getProjects(Com.me().id);
							}
						} catch (RemoteException e) {
							e.printStackTrace();
						}
						return "Projects";
					}
					
					@Override
					public int getIndexOfChild(Object parent, Object child) {
						if(parent instanceof String && parent.equals("Projects")) {
							return projects.indexOf(child);							
						}
						if(parent instanceof Project) {
							return ((Project) parent).tasks.indexOf(child);
						}
						return 0;
					}
					
					@Override
					public int getChildCount(Object parent) {
						if(parent instanceof String && parent.equals("Projects")) {
							return projects.size();							
						}
						if(parent instanceof Project) {
							return ((Project) parent).tasks.size();
						}
						return 0;
					}
					
					@Override
					public Object getChild(Object parent, int index) {
						if(parent instanceof String && parent.equals("Projects")) {
							return projects.get(index);
						}
						if(parent instanceof Project) {
							return ((Project) parent).tasks.get(index);
						}
						return null;
					}
					
					@Override
					public void addTreeModelListener(TreeModelListener l) {}
				});
			}
			
			@Override
			public void componentResized(ComponentEvent e) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void componentMoved(ComponentEvent e) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void componentHidden(ComponentEvent e) {
				// TODO Auto-generated method stub
				
			}
		});

		JPanel myInfoPanel = new JPanel();
		myInfoPanel.setBorder(new LineBorder(Color.red));
				
		myInfoPanel.add(new JLabel("MY Name is"));
		
		final JSplitPane upperPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, projectListPanel, myInfoPanel);
		upperPane.setDividerSize(0);
		setLeftComponent(upperPane);
		
		JPanel lowerPane = new JPanel(new FlowLayout());
		
		JButton btnCreateProject = new JButton("Create Project");
		btnCreateProject.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
			}
		});
		lowerPane.add(btnCreateProject);
		lowerPane.add(new JButton("My Requested Jobs"));
		lowerPane.add(new JButton("Search Programmers"));
		
		JButton btnExit = new JButton("Back");
		btnExit.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(getTopLevelAncestor() instanceof JFrame) {
					((ClientFrame)getTopLevelAncestor()).ChangePage("Manager");
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
