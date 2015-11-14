/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.net;

/**
 *
 * @author Andrea
 */
public class Protocol {
    
    /* Generic messages */
    
    public static final int M_ACK = 200;
    public static final int M_NACK = 201;
    public static final int M_ERR = 202;    
    public static final int M_GS_CMD = 203; /* Game server command */
    
    /* Game commands */
    
    public static final int M_GS_CONNECT = 300;
    public static final int M_GS_RECONNECT = 301;
    public static final int M_GS_END_TURN = 302;
    public static final int M_GS_GET_MAP = 303;
    public static final int M_GS_WAIT_FOR_PLAYERS = 304;
    public static final int M_GS_RUN_GAME = 305;
    public static final int M_GS_IS_MY_TURN = 306;
    
    
    
}
