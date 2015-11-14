package projectcolossus.graphics.dialog;

import java.util.ArrayList;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;

public class YesNoDialogFragment extends DialogFragment implements DialogInterface.OnClickListener {
	
	public static final int DIALOG_NO = 0;
	public static final int DIALOG_YES = 1;
	
	protected ArrayList<Listener> listeners;
	
	protected String message;
	
	public YesNoDialogFragment() {
		listeners = new ArrayList<Listener>();
	}
	
	
	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
		AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
		
		builder.setMessage(message);
		
		builder.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				fireOnDialogResult(DIALOG_YES);
			}
		});
		
		builder.setNegativeButton("No", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				fireOnDialogResult(DIALOG_NO);
			}
		});
		
		return builder.create();
		
	}

	@Override
	public void onClick(DialogInterface dialog, int which) {
		Log.d("Test3", which + "");
	}	
	
	public void setMessage(String message) { this.message = message; }
	public String getMessage() { return this.message; }
	
	public boolean addListener(Listener l) { return listeners.add(l);}
	public boolean removeListener(Listener l) { return listeners.remove(l); }
	
	protected void fireOnDialogResult(int result) {
		for(Listener l : listeners)
			l.onDialogResult(this, result);
	}
	
	public static interface Listener {
		public void onDialogResult(DialogFragment dialogFragment, int value);
	}


}
