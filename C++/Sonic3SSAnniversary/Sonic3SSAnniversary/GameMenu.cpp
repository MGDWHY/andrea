#include "GameMenu.h"

namespace GameMenu {

	GameMenuParentItem * mnuRoot;

	GameMenuItem * GetMenuById(int id, GameMenuItem * root) {

		if(root == NULL)
			root = mnuRoot;

		for(int i = 0; i < root->GetChildCount(); i++) {

			GameMenuItem * current = root->GetChildren()[i];

			if(current->GetID() == id)
				return current;

			if(!current->IsLeaf()) {
				GameMenuItem * recursive = GetMenuById(id, current);
				if(recursive != NULL)
					return recursive;
			}
		}

		return NULL;
	}

	GameMenuItem * BuildGameMenu(GameMenuActionListener * listener, const GameConfig& gameConfig, const vector<string>& stageFiles) {
		
		vector<GameMenuSelectItemOption> onOffOpt, stagesOpt, volumeOpt, qualityOpt;

		int * temp = new int[1];

		*temp = Global::SWITCH_OFF; onOffOpt.push_back(GameMenuSelectItemOption("Off", sizeof(int), temp));
		*temp = Global::SWITCH_ON; onOffOpt.push_back(GameMenuSelectItemOption("On", sizeof(int), temp));

		for(int i = 0; i < stageFiles.size(); i++) {
			char buffer[40];
			sprintf(buffer, "%d", i + 1);
			stagesOpt.push_back(GameMenuSelectItemOption(string(buffer), sizeof(int), &i));
		}

		for(int i = 1; i <= 100; i++) {
			char buf[4];
			sprintf(buf, "%03d", i);
			volumeOpt.push_back(GameMenuSelectItemOption(string(buf), sizeof(int), &i));
		}


		*temp = Global::QUALITY_LOW; qualityOpt.push_back(GameMenuSelectItemOption("Low", sizeof(int), temp));
		*temp = Global::QUALITY_MEDIUM; qualityOpt.push_back(GameMenuSelectItemOption("Medium", sizeof(int), temp));
		*temp = Global::QUALITY_HIGH; qualityOpt.push_back(GameMenuSelectItemOption("High", sizeof(int), temp));

		mnuRoot = new GameMenuParentItem(MENU_ROOT, "");

		/* Main menu */
		GameMenuParentItem * mnuNewGame = new GameMenuParentItem(MENU_NEWGAME, "New game");
		GameMenuParentItem * mnuCustomStages = new GameMenuParentItem(MENU_CUSTOMSTAGES, "Custom stages");
		GameMenuParentItem * mnuOptions = new GameMenuParentItem(MENU_OPTIONS, "Options");
		GameMenuLeafItem * mnuExit = new GameMenuLeafItem(MENU_EXIT, "Exit");

		/* Mew game menu */
		GameMenuCurrentLevel * mnuNewGameStart = new GameMenuCurrentLevel(MENU_NEWGAME_START,  "Start Game");
		GameMenuCodeInputItem * mnuNewGameInsertCode = new GameMenuCodeInputItem(MENU_NEWGAME_INSERTCODE);
		GameMenuLeafItem * mnuNewGameBack = new GameMenuLeafItem(MENU_NEWGAME_BACK, "Back to Main menu");

		/* Custom stages menu */
		GameMenuSelectItem * mnuCustomSelectStage = new GameMenuSelectItem(MENU_CUSTOM_SELECTSTAGE, "Select Stage", stagesOpt);
		GameMenuLeafItem * mnuCustomStartGame = new GameMenuLeafItem(MENU_CUSTOM_STARTGAME, "Start Game");
		GameMenuLeafItem * mnuCustomBack = new GameMenuLeafItem(MENU_CUSTOM_BACK, "Back to Main Menu");

		/* Options menu */
		GameMenuParentItem * mnuOptionsVideo = new GameMenuParentItem(MENU_OPTIONS_VIDEO, "Video Settings");
		GameMenuParentItem * mnuOptionsAudio = new GameMenuParentItem(MENU_OPTIONS_AUDIO, "Audio Settings");
		GameMenuLeafItem * mnuOptionsBack = new GameMenuLeafItem(MENU_OPTIONS_BACK, "Back to Main menu");

		/* Video menu */
		GameMenuSelectItem * mnuVideoStereo3D = new GameMenuSelectItem(MENU_VIDEO_STEREO3D, "Stereoscopic 3D", onOffOpt);
		GameMenuSelectItem * mnuVideoShadowsEnabled = new GameMenuSelectItem(MENU_VIDEO_SHADOWSENABLED, "Shadows", onOffOpt);
		GameMenuSelectItem * mnuVideoShadowQuality = new GameMenuSelectItem(MENU_VIDEO_SHADOWQUALITY, "Shadow Quality", qualityOpt);
		GameMenuLeafItem * mnuVideoBack = new GameMenuLeafItem(MENU_OPTIONS_BACK, "Back to Options");

		/* Audio menu */
		GameMenuSelectItem * mnuAudioMusicVolume = new GameMenuSelectItem(MENU_AUDIO_MUSIC_VOLUME, "Music Volume", volumeOpt);
		GameMenuSelectItem * mnuAudioFXVolume = new GameMenuSelectItem(MENU_AUDIO_FX_VOLUME, "SFX VOlume", volumeOpt);
		GameMenuLeafItem * mnuAudioBack = new GameMenuLeafItem(MENU_AUDIO_BACK, "Back to Options");

		mnuVideoStereo3D->SetSelectedValue((char*)&(gameConfig.stereo3D), sizeof(int));
		mnuVideoShadowsEnabled->SetSelectedValue((char*)&(gameConfig.shadowsEnabled), sizeof(int));
		mnuVideoShadowQuality->SetSelectedValue((char*)&(gameConfig.shadowQuality), sizeof(int));
		
		mnuAudioMusicVolume->SetSelectedValue((char*)&(gameConfig.musicVolume), sizeof(int));
		mnuAudioFXVolume->SetSelectedValue((char*)&(gameConfig.fxVolume), sizeof(int));

		mnuRoot->AddChild(mnuNewGame); mnuNewGame->AddGameMenuActionListener(listener);
		mnuRoot->AddChild(mnuCustomStages); mnuCustomStages->AddGameMenuActionListener(listener);
		mnuRoot->AddChild(mnuOptions); mnuOptions->AddGameMenuActionListener(listener);
		mnuRoot->AddChild(mnuExit); mnuExit->AddGameMenuActionListener(listener);

		mnuNewGame->AddChild(mnuNewGameStart); mnuNewGameStart->AddGameMenuActionListener(listener);
		mnuNewGame->AddChild(mnuNewGameInsertCode); mnuNewGameInsertCode->AddGameMenuActionListener(listener);
		mnuNewGame->AddChild(mnuNewGameBack); mnuNewGameBack->AddGameMenuActionListener(listener);

		mnuCustomStages->AddChild(mnuCustomSelectStage); mnuCustomSelectStage->AddGameMenuActionListener(listener);
		mnuCustomStages->AddChild(mnuCustomStartGame); mnuCustomStartGame->AddGameMenuActionListener(listener);
		mnuCustomStages->AddChild(mnuCustomBack); mnuCustomBack->AddGameMenuActionListener(listener);

		mnuOptions->AddChild(mnuOptionsAudio); mnuOptionsAudio->AddGameMenuActionListener(listener);
		mnuOptions->AddChild(mnuOptionsVideo); mnuOptionsVideo->AddGameMenuActionListener(listener);
		mnuOptions->AddChild(mnuOptionsBack); mnuOptionsBack->AddGameMenuActionListener(listener);

		mnuOptionsVideo->AddChild(mnuVideoStereo3D); mnuVideoStereo3D->AddGameMenuActionListener(listener);
		mnuOptionsVideo->AddChild(mnuVideoShadowsEnabled); mnuVideoShadowsEnabled->AddGameMenuActionListener(listener);
		mnuOptionsVideo->AddChild(mnuVideoShadowQuality); mnuVideoShadowQuality->AddGameMenuActionListener(listener);
		mnuOptionsVideo->AddChild(mnuVideoBack); mnuVideoBack->AddGameMenuActionListener(listener);

		mnuOptionsAudio->AddChild(mnuAudioMusicVolume); mnuAudioMusicVolume->AddGameMenuActionListener(listener);
		mnuOptionsAudio->AddChild(mnuAudioFXVolume); mnuAudioFXVolume->AddGameMenuActionListener(listener);
		mnuOptionsAudio->AddChild(mnuAudioBack); mnuAudioBack->AddGameMenuActionListener(listener);

		// temp
		mnuNewGameInsertCode->SetCode(StageGenerator::GetStageCode(1));
		// temp


		return mnuRoot;
	}

	/* General menu item */
	void GameMenuItem::AddGameMenuActionListener(GameMenuActionListener * listener) { this->listeners.push_back(listener); }
	int GameMenuItem::GetID() { return this->id; }
	void GameMenuItem::SetParent(GameMenuParentItem * parent) { this->parent = parent; };
	void GameMenuItem::FireAction(int action) {
		for(int i = 0; i < this->listeners.size(); i++) 
			listeners[i]->ActionPerformed(action, this);
	}
	GameMenuParentItem * GameMenuItem::GetParent() { return this->parent; }

	GameMenuItem::GameMenuItem(int id){ 
		this->id = id; 
		this->parent = NULL;
	}

	GameMenuItem::~GameMenuItem(void){}


	/* Parent menu item */
	bool GameMenuParentItem::IsLeaf() { return false; }
	int GameMenuParentItem::GetChildCount() { return this->children.size(); }
	int GameMenuParentItem::GetSelectedChild() { return this->selectedChild; }
	void GameMenuParentItem::SetSelectedChild(int index) { this->selectedChild = index; }
	vector<GameMenuItem*> GameMenuParentItem::GetChildren() { return this->children; }
	string GameMenuParentItem::GetDisplayText() { return this->text; }
	void GameMenuParentItem::AddChild(GameMenuItem * child) { 
		this->children.push_back(child); 
		child->SetParent(this);
	}

	GameMenuParentItem::GameMenuParentItem(int id, string text): GameMenuItem(id) { 
		this->text = text;
		this->selectedChild = 0;
		this->children = vector<GameMenuItem*>();
	}
	GameMenuParentItem::GameMenuParentItem(int id, string text, vector<GameMenuItem*> children): GameMenuItem(id) { 
		this->text = text;
		this->selectedChild = 0;
		this->children = children; 
	}

	GameMenuParentItem::~GameMenuParentItem(void) {}

	/* Leaf menu item */

	bool GameMenuLeafItem::IsLeaf() { return true; }
	int GameMenuLeafItem::GetChildCount() { return 0; }
	vector<GameMenuItem*> GameMenuLeafItem::GetChildren() { return this->children; }
	string GameMenuLeafItem::GetDisplayText() { return this->text; }

	GameMenuLeafItem::GameMenuLeafItem(int id, string text): GameMenuItem(id) { this->text = text; }
	GameMenuLeafItem::~GameMenuLeafItem() {}

	/* Select menu item */

	const string GameMenuSelectItem::SEPARATOR = string(": ");

	void GameMenuSelectItem::SetSelectedOption(int index) { 
		this->selectedOption = index; 
		this->FireAction(0);
	}
	int GameMenuSelectItem::GetSelectedOption() { return this->selectedOption; }		
	void GameMenuSelectItem::SetSelectedValue(char * data, int dataSize) {
		for(int i = 0; i <this->selectOptions.size(); i++) {

			char * optData = (char*) this->selectOptions[i].GetData();

			bool match = true;

			for(int j = 0; j < dataSize; j++) {
				if(data[j] != optData[j]) {
					match = false;
					break;
				}
			}

			if(match) {
				this->SetSelectedOption(i);
				this->FireAction(0);
				return;
			}
		}
	}
	char * GameMenuSelectItem::GetSelectedValue() {
		return (char*)this->selectOptions[this->selectedOption].GetData();
	}

	int GameMenuSelectItem::GetOptionCount() { return this->selectOptions.size(); }

	string GameMenuSelectItem::GetDisplayText() {
		return this->text + GameMenuSelectItem::SEPARATOR + this->selectOptions[selectedOption].GetDisplayText();
	}

	vector<GameMenuSelectItemOption> GameMenuSelectItem::GetOptions() {
		return this->selectOptions;
	}

	GameMenuSelectItem::GameMenuSelectItem(int id, string text, vector<GameMenuSelectItemOption> selectOptions): GameMenuLeafItem(id, text) {
		this->selectOptions = selectOptions;
		this->selectedOption = 0;
	}

	GameMenuSelectItem::~GameMenuSelectItem() {}

	/* Select option */

	GameMenuSelectItemOption::GameMenuSelectItemOption(string displayText, int dataSize, void * pointer) {
		this->displayText = displayText;
		this->dataSize = dataSize;

		this->data = new char[this->dataSize];

		for(int i = 0; i < this->dataSize; i++)
			this->data[i] = ((char*)pointer)[i];
	}
		
	string GameMenuSelectItemOption::GetDisplayText() { return this->displayText; }
	void * GameMenuSelectItemOption::GetData() { return this->data; }
	int GameMenuSelectItemOption::GetDataSize() { return this->dataSize; }

	/* Input Menu */

	GameMenuInputItem::GameMenuInputItem(int id, string text, int dataSize) : GameMenuLeafItem(id, text) {
		this->dataSize = dataSize;
		this->data = new char[dataSize];
		this->inputMode = false;
		this->selectedCharacter = 0;
	}

	bool GameMenuInputItem::IsInputMode() { return this->inputMode; }
	void GameMenuInputItem::SetInputMode(bool value) { this->inputMode = value; }

	char * GameMenuInputItem::GetData() {
		char * dataCopy = new char[this->dataSize];
		for(int i = 0; i < this->dataSize; i++) {
			dataCopy[i] = data[i];
		}
		return dataCopy;
	}

	void GameMenuInputItem::SetData(char * data) {
		for(int i = 0; i < this->dataSize; i++) {
			this->data[i] = data[i];
		}
	}

	int GameMenuInputItem::GetDataSize() { return this->dataSize; }

	void GameMenuInputItem::SetSelectedIndex(int index) {
		this->selectedCharacter = max(0, min(index, this->dataSize-1));
	}
	int GameMenuInputItem::GetSelectedIndex() { return this->selectedCharacter; }

	void GameMenuInputItem::SetCurrentCharacter(char value) { 
		if(this->AcceptCharacter(value)) {
			this->data[this->selectedCharacter] = value; 
			this->SetSelectedIndex(this->selectedCharacter + 1);
		}
	}
		
	string GameMenuInputItem::GetDisplayText() {
		return this->text + ": " + this->GetFormattedData();
	}

	/* Insert code menu */

	bool GameMenuCodeInputItem::AcceptCharacter(char value) {
		return (value >= '0' && value <= '9');
	}

	string GameMenuCodeInputItem::GetFormattedData() {
		static char buf[30];

		sprintf(buf, "%c%c%c%c-%c%c%c%c-%c%c%c%c",
			this->data[0], this->data[1], this->data[2], this->data[3], 
			this->data[4], this->data[5], this->data[6], this->data[7], 
			this->data[8], this->data[9], this->data[10], this->data[11]
		);

		return string(buf);
	}

	void GameMenuCodeInputItem::SetCode(__int64 code) {
		static char buf[13];
		sprintf(buf, "%lld", code);
		this->SetData(buf);
	}

	__int64 GameMenuCodeInputItem::GetCode() {
		static char buf[13];
		for(int i = 0; i < this->dataSize; i++)
			buf[i] = this->data[i];

		data[16] = '\0';

		return _atoi64(buf);
	}

	GameMenuCodeInputItem::GameMenuCodeInputItem(int id) : GameMenuInputItem(id, "Level Code", 12) {}

	/* Start game level menu */
	GameMenuCurrentLevel::GameMenuCurrentLevel(int id, string text) : GameMenuLeafItem(id, text) {
		this->currentLevel = 1;
	}
	void GameMenuCurrentLevel::SetCurrentLevel(int level) {
		this->currentLevel = level;
	}
	int GameMenuCurrentLevel::GetCurrentLevel() {
		return this->currentLevel;
	}
	string GameMenuCurrentLevel::GetDisplayText() {
		static char buf[15];
		sprintf(buf, "%d", this->currentLevel);
		return this->text + " (Level " + buf + ")";
	}
}