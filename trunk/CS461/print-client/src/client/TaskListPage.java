package client;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
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

import dataset.Task;
import dataset.User;

public class TaskListPage extends JSplitPane {
	private static final long serialVersionUID = 1L;
	
	public TaskListPage() {
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
					List<Task> tasks;
					
					@Override
					public void valueForPathChanged(TreePath path, Object newValue) {}
					
					@Override
					public void removeTreeModelListener(TreeModelListener l) {}
					
					@Override
					public boolean isLeaf(Object node) {
						if(node instanceof String && node.equals("Tasks")) {
							return false;					
						}
						return true;
					}
					
					@Override
					public Object getRoot() {
						try {
							if(Com.me() != null) {
								tasks = Com.get().getAllOpenTasks();
							}
						} catch (RemoteException e) {
							e.printStackTrace();
						}
						return "Tasks";
					}
					
					@Override
					public int getIndexOfChild(Object parent, Object child) {
						if(parent instanceof String && parent.equals("Tasks")) {
							return tasks.indexOf(child);							
						}
						return 0;
					}
					
					@Override
					public int getChildCount(Object parent) {
						if(parent instanceof String && parent.equals("Tasks")) {
							return tasks.size();							
						}
						return 0;
					}
					
					@Override
					public Object getChild(Object parent, int index) {
						if(parent instanceof String && parent.equals("Tasks")) {
							return tasks.get(index);
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
		
		final JButton btnRequestTask = new JButton("Request");
		btnRequestTask.setEnabled(false);
		btnRequestTask.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(tree.getSelectionPath() != null) {
					User u = (User) tree.getSelectionPath().getLastPathComponent();
					try {
						if(u.status.equals("None")) {
							if(Com.get().requestTask(Com.inst.selectedTask.Id, u.id)) {
								Com.inst.selectedTask = null;
								((ClientFrame)getTopLevelAncestor()).ChangePage("Programmer");
							}	
						}	
						else {
							if(Com.get().cancelTask(Com.inst.selectedTask.Id, u.id)) {
								Com.inst.selectedTask = null;
								((ClientFrame)getTopLevelAncestor()).ChangePage("Programmer");
							}
						}
					} catch (RemoteException e1) {
						e1.printStackTrace();
					}
				}
				
			}
		});
		lowerPane.add(btnRequestTask);
		
		JButton btnExit = new JButton("Back");
		btnExit.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(getTopLevelAncestor() instanceof JFrame) {
					((ClientFrame)getTopLevelAncestor()).ChangePage("Programmer");
				}
			}
		});
		
		lowerPane.add(btnExit);

		tree.setModel(null);
		tree.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent me) {
			    TreePath tp = tree.getPathForLocation(me.getX(), me.getY());
				if(tp != null) {
					Object o = tp.getLastPathComponent();
					if(o instanceof User) {
						if(((User) o).status.equals("None")) {
							btnRequestTask.setText("Request");
						}
						else {
							btnRequestTask.setText("Cancel");
						}
						btnRequestTask.setEnabled(true);
					}
					else {
						btnRequestTask.setEnabled(false);
					}
				}				
			}
		});
		
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
