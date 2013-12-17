package client;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.rmi.RemoteException;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import dataset.Note;

public class TaskDetailPage extends JSplitPane {
	private static final long serialVersionUID = 1L;

	public TaskDetailPage() {
		super(JSplitPane.VERTICAL_SPLIT);

		JPanel panel = new JPanel();
		JPanel lowerPane = new JPanel(new FlowLayout());
		
		panel.add(new JLabel("Task Name"));
		
		final JTextField taskName = new JTextField();
		panel.add(taskName);
		
		final JTextArea notesArea = new JTextArea(5,30);
		panel.add(notesArea);

		notesArea.addComponentListener(new ComponentListener() {
			
			@Override
			public void componentShown(ComponentEvent e) {
				String str = "";
				List<Note> notes = null;
				try {
					notes = Com.get().getNotes(Com.inst.selectedTask.Id);
				} catch (RemoteException e2) {
					// TODO Auto-generated catch block
					e2.printStackTrace();
				}
				for(Note n : notes) {
					str += n.content + "\n";
				}
				notesArea.setText(str);
				
				taskName.setText(Com.inst.selectedTask.name);
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

		final JButton btnFinish = new JButton("Finish Request");
		btnFinish.setEnabled(false);
		btnFinish.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if(Com.me().type.equals("Programmer")) {
					try {
						Com.get().requestTaskFinish(Com.inst.selectedTask.Id);
						Com.inst.selectedTask = null;
						((ClientFrame) getTopLevelAncestor()).ChangePage(Com.me().type);
					} catch (RemoteException e1) {
						e1.printStackTrace();
					}
				}
				else {
					try {
						Com.get().approveTaskFinish(Com.inst.selectedTask.Id, Com.me().id);
						Com.inst.selectedTask = null;
						((ClientFrame) getTopLevelAncestor()).ChangePage(Com.me().type);
					} catch (RemoteException e1) {
						e1.printStackTrace();
					}
				}
			}
		});
		lowerPane.add(btnFinish);		
		
		JButton btnNote = new JButton("Add Note");
		btnNote.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (getTopLevelAncestor() instanceof JFrame) {
					JTextField text = new JTextField();
					JOptionPane new_pane = new JOptionPane(text);
					JDialog new_dialog = new_pane.createDialog(new JFrame(), "Input Content");
					new_dialog.setVisible(true);
					new_dialog.toFront();
					try {
						if(Com.get().addNote(Com.inst.selectedTask.Id, Com.me().id, text.getText())) {
							notesArea.setText(notesArea.getText() + text.getText() + "\n");
						}
					} catch (RemoteException e1) {
						e1.printStackTrace();
					}
				}
			}
		});
		lowerPane.add(btnNote);

		JButton btnExit = new JButton("Back");
		btnExit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (getTopLevelAncestor() instanceof JFrame) {
					((ClientFrame) getTopLevelAncestor()).ChangePage(Com.me().type);
				}
			}
		});
		lowerPane.add(btnExit);

		setLeftComponent(panel);
		setRightComponent(lowerPane);

		setDividerSize(0);

		addComponentListener(new ComponentListener() {

			@Override
			public void componentShown(ComponentEvent e) {
				if(Com.me().type.equals("Programmer")) {
					btnFinish.setText("Request Finished");
					btnFinish.setEnabled(true);
				}
				else {
					btnFinish.setText("Approve and Pay");
					if(Com.inst.selectedTask.status.equals("Finished")) {
						btnFinish.setEnabled(true);
					}
					else {
						btnFinish.setEnabled(false);
					}
				}
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
	}
}
