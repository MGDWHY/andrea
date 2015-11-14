/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package rigidbody2dx;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;

import javax.swing.JPanel;
import rigidbody2d.Engine;
import rigidbody2d.RigidBody;

/**
 *
 * @author Andrea
 */
public class RigidBodyPanel extends JPanel {
    
    private Ellipse2D.Float pointMassEllipse;
    
    private Engine engine;
    
    private float mouseX, mouseY;
    
    public RigidBodyPanel() {
        super();
        pointMassEllipse = new Ellipse2D.Float();
        
        pointMassEllipse.width = 10;
        pointMassEllipse.height = 10;
        
        new Thread() {
            public void run() {
                while(true) {
                    repaint();
                    try { Thread.sleep(20); }
                    catch(InterruptedException ex) {}
                }
            }
        }.start();
        
        addMouseMotionListener( new MouseMotionListener() {
            @Override
            public void mouseDragged(MouseEvent me) {}

            @Override
            public void mouseMoved(MouseEvent me) {
                mouseX = me.getX();
                mouseY = me.getY();
            }
        });
    }
    

    public void setEngine(Engine e) { this.engine = e; }
    
    @Override
    public void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;
        
        g2.setPaint(Color.BLACK);
        g2.fillRect(0, 0, getWidth(), getHeight());
        
        
        if(engine == null)
            return;
        
        for(RigidBody b : engine.getRigidBodies()) {
            
            g2.setPaint(Color.RED);
            /*
            for(PointMass p : b.getPointsMass()) {
                pointMassEllipse.x = (float)p.getPosition().getX() - pointMassEllipse.width / 2;
                pointMassEllipse.y = (float)p.getPosition().getY() - pointMassEllipse.height / 2;
                g2.fill(pointMassEllipse);
            }
            
            g2.setPaint(Color.GREEN);
            
            pointMassEllipse.x = (float)b.getCenterOfMass().getX() - pointMassEllipse.width / 2;
            pointMassEllipse.y = (float)b.getCenterOfMass().getY() - pointMassEllipse.height / 2;
            
            g2.fill(pointMassEllipse);
            
            g2.setPaint(Color.YELLOW);
            
            RigidBody.Bounds bounds = b.getBounds();
            
            g2.drawRect((int)bounds.x0, (int)bounds.y0, (int)(bounds.x1 - bounds.x0), (int)(bounds.y1 - bounds.y0));*/
            
            g2.fill(b);
            
            g2.setPaint(Color.YELLOW);
            
            g2.draw(b.getBounds2D());
            
        }
        
        g2.setPaint(Color.WHITE);
        g2.drawString("X: " + mouseX, 20, 20);
        g2.drawString("Y: " + mouseY, 20, 40);
        
    }
    
}
