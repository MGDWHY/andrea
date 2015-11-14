/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.cfb;

/**
 *
 * @author Andrea
 */
public class CFBCipher {
    
    private byte[] register;
    
    public CFBCipher(byte[] iv) {
        this.register = iv;
    }
    
    public byte[] Cipher(byte[] dataIn) {
        byte[] dataOut = new byte[dataIn.length];
        
        for(int i = 0; i < dataIn.length; i++) {
            dataOut[i] = (byte)(dataIn[i] ^ register[register.length - 1]);
            Shift(dataOut[i]);
        }
        
        return dataOut;
    }

    public byte[] Decipher(byte[] dataIn) {
        byte[] dataOut = new byte[dataIn.length];
        
        for(int i = 0; i < dataIn.length; i++) {
            dataOut[i] = (byte)(dataIn[i] ^ register[register.length - 1]);
            Shift(dataIn[i]);
        }
        
        return dataOut;
    }    
    
    private void Shift(byte newByte) {
        register[0] = newByte;
        for(int i = 0; i < register.length - 1; i++)
            register[i+1] = register[i];
    }
}
