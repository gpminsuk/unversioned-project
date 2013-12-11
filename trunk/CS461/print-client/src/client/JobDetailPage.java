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

public class JobDetailPage extends JSplitPane {
	private static final long serialVersionUID = 1L;
	
	public JobDetailPage() {
		super(JSplitPane.VERTICAL_SPLIT);
		
		JSplitPane bpane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, null, null);
		
		JSplitPane pane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, null, null);		
		pane.setDividerLocation(getWidth() - 150);
		pane.setDividerSize(0);
		
		JPanel lowerPane = new JPanel(new FlowLayout());
		
		lowerPane.add(new JButton("Report Completed"));
		
		JButton btnExit = new JButton("Back");
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
		
		setLeftComponent(pane);
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
	}

}
