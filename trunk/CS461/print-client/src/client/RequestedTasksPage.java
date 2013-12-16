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

public class RequestedTasksPage extends JSplitPane {
	private static final long serialVersionUID = 1L;

	public RequestedTasksPage() {
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
					public void valueForPathChanged(TreePath path,
							Object newValue) {
					}

					@Override
					public void removeTreeModelListener(TreeModelListener l) {
					}

					@Override
					public boolean isLeaf(Object node) {
						if (node instanceof String && node.equals("Tasks")) {
							return false;
						}
						return true;
					}

					@Override
					public Object getRoot() {
						try {
							if (Com.me() != null) {
								if(Com.me().type.equals("Programmer")) {
									tasks = Com.get().getProgrammerRequestedTasks(Com.me().id);									
								}
								else {
									tasks = Com.get().getManagerRequestedTasks(Com.me().id);									
								}
							}
						} catch (RemoteException e) {
							e.printStackTrace();
						}
						return "Tasks";
					}

					@Override
					public int getIndexOfChild(Object parent, Object child) {
						if (parent instanceof String
								&& parent.equals("Tasks")) {
							return tasks.indexOf(child);
						}
						if (parent instanceof Task) {
							return ((Task) parent).requestedProgrammers.indexOf(child);
						}
						return 0;
					}

					@Override
					public int getChildCount(Object parent) {
						if (parent instanceof String
								&& parent.equals("Tasks")) {
							return tasks.size();
						}
						if(Com.me().type.equals("Manager")) {
							if (parent instanceof Task) {
								return ((Task) parent).requestedProgrammers.size();
							}
						}
						return 0;
					}

					@Override
					public Object getChild(Object parent, int index) {
						if (parent instanceof String
								&& parent.equals("Tasks")) {
							return tasks.get(index);
						}
						if(Com.me().type.equals("Manager")) {
							if (parent instanceof Task) {
								return ((Task) parent).requestedProgrammers.get(index);
							}
						}
						return null;
					}

					@Override
					public void addTreeModelListener(TreeModelListener l) {
					}
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

		final JSplitPane upperPane = new JSplitPane(
				JSplitPane.HORIZONTAL_SPLIT, projectListPanel, myInfoPanel);
		upperPane.setDividerSize(0);
		setLeftComponent(upperPane);

		JPanel lowerPane = new JPanel(new FlowLayout());

		final JButton btnAccept = new JButton("Accept");
		btnAccept.setEnabled(false);
		btnAccept.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if(tree.getSelectionPath() != null) {
					User u = null;
					Task t = null;
					if(Com.me().type.equals("Manager")) {
						u = (User) tree.getSelectionPath().getLastPathComponent();
						t = (Task) tree.getSelectionPath().getParentPath().getLastPathComponent();						
					}
					else {
						u = (User) Com.me();
						t = (Task) tree.getSelectionPath().getLastPathComponent();
					}
					
					try {
						if(Com.get().acceptTask(t.Id, u.id)) {
							((ClientFrame) getTopLevelAncestor()).ChangePage(Com.me().type);	
						}
					} catch (RemoteException e1) {
						e1.printStackTrace();
					}
				}
			}
		});
		lowerPane.add(btnAccept);
		tree.setModel(null);
		tree.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent me) {
				if(me.getClickCount() == 1) {
					TreePath tp = tree.getPathForLocation(me.getX(), me.getY());
					if(tp != null) {
						Object o = tp.getLastPathComponent();

						if(Com.me().type.equals("Manager")) {
							if(o instanceof User) {
								btnAccept.setEnabled(true);
							}
							else {
								btnAccept.setEnabled(false);
							}
						}
						else {
							if(o instanceof Task) {
								btnAccept.setEnabled(true);
							}
							else {
								btnAccept.setEnabled(false);
							}
						}
					}
	            }
	            else if(me.getClickCount() == 2) {
	            	TreePath tp = tree.getPathForLocation(me.getX(), me.getY());
					if(tp != null) {
						Object o = tp.getLastPathComponent();
						if(o instanceof Task) {
							Com.inst.selectedTask = (Task) o;
							((ClientFrame) getTopLevelAncestor())
							.ChangePage("TaskDetail");							
						}						
					}
	            }    				
			}
		});
		
		JButton btnExit = new JButton("Back");
		btnExit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (getTopLevelAncestor() instanceof JFrame) {
					((ClientFrame) getTopLevelAncestor())
					.ChangePage(Com.me().type);
				}
			}
		});

		lowerPane.add(btnExit);

		setRightComponent(lowerPane);
		setDividerSize(0);

		addComponentListener(new ComponentListener() {

			@Override
			public void componentShown(ComponentEvent e) {
			}

			@Override
			public void componentResized(ComponentEvent e) {
				setDividerLocation(getHeight() - 40);
			}

			@Override
			public void componentMoved(ComponentEvent e) {
			}

			@Override
			public void componentHidden(ComponentEvent e) {
			}
		});

		upperPane.addComponentListener(new ComponentListener() {

			@Override
			public void componentShown(ComponentEvent e) {
			}

			@Override
			public void componentResized(ComponentEvent e) {
				upperPane.setDividerLocation(getWidth() - 150);
			}

			@Override
			public void componentMoved(ComponentEvent e) {
			}

			@Override
			public void componentHidden(ComponentEvent e) {
			}
		});
	}

}
