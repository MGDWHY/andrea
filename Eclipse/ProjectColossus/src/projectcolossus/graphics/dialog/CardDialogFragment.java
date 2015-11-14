package projectcolossus.graphics.dialog;

import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.heirs.ScoutCard;
import projectcolossus.graphics.view.CardBigView;
import andrea.bucaletti.projectcolossus.R;
import android.app.Dialog;
import android.app.DialogFragment;
import android.os.Bundle;

public class CardDialogFragment extends DialogFragment {
	
	protected Card card;
	
	public CardDialogFragment() {super();}
	
	public void setCard(Card card) {this.card = card; }
	
	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
		super.onCreateDialog(savedInstanceState);
		Dialog dialog = new Dialog(getActivity(), R.style.notitledialog);
		CardBigView cbv = new CardBigView(getActivity().getApplicationContext(), getActivity(), card);
		dialog.setContentView(cbv);
		dialog.setTitle(null);
		cbv.getLayoutParams().width = (int)(cbv.width);
		cbv.getLayoutParams().height = (int)(cbv.height);
		return dialog;
	}
}
