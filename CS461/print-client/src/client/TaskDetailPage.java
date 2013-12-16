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
		
		JTextArea notesArea = new JTextArea(5,30);
		panel.add(notesArea);

		JPanel lowerPane = new JPanel(new FlowLayout());
		List<Note> notes = null;
		try {
			notes = Com.get().getNotes(Com.inst.selectedTask.Id);
		} catch (RemoteException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		for(Note n : notes) {
			notesArea.setText(notesArea.getText() + "\n" + n.content);
		}

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
						Com.get().addNote(Com.inst.selectedTask.Id, Com.me().id, text.getText());
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
