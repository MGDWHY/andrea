#pragma once

#include <string>
#include <vector>
#include "Global.h"
#include "StageGenerator.h"

using namespace std;

namespace GameMenu {

	const int MENU_ROOT = 0;

	const int MENU_NEWGAME = 1;
	const int MENU_CUSTOMSTAGES = 2;
	const int MENU_OPTIONS = 3;
	const int MENU_EXIT = 4;

	const int MENU_NEWGAME_START = 19;
	const int MENU_NEWGAME_INSERTCODE = 20;
	const int MENU_NEWGAME_BACK = 21;
	
	const int MENU_OPTIONS_VIDEO = 5;
	const int MENU_OPTIONS_AUDIO = 6;
	const int MENU_OPTIONS_BACK = 7;

	const int MENU_VIDEO_STEREO3D = 9;
	const int MENU_VIDEO_SHADOWQUALITY = 10;
	const int MENU_VIDEO_SHADOWSENABLED = 11;
	const int MENU_VIDEO_BACK = 12;

	const int MENU_AUDIO_MUSIC_VOLUME = 13;
	const int MENU_AUDIO_FX_VOLUME = 14;
	const int MENU_AUDIO_BACK = 15;

	const int MENU_CUSTOM_SELECTSTAGE = 16;
	const int MENU_CUSTOM_STARTGAME = 17;
	const int MENU_CUSTOM_BACK = 18;

	class GameMenuActionListener;
	class GameMenuItem;
	class GameMenuParentItem;
	class GameMenuLeafItem;
	class GameMenuSelectItem;
	class GameMenuInputItem;
	class GameMenuCodeInputItem;
	class GameMenuCurrentLevel;

	GameMenuItem * BuildGameMenu(GameMenuActionListener * listener, const GameConfig& gameConfig, const vector<string>& stageFiles);
	GameMenuItem * GetMenuById(int id, GameMenuItem * root = NULL);

	class GameMenuActionListener {
	public:
		virtual void ActionPerformed(int action, GameMenuItem * source ) = 0;
	};

	class GameMenuItem
	{
	private:
		int id;
		GameMenuParentItem * parent;
		vector<GameMenuActionListener*> listeners;
	public:

		virtual bool IsLeaf() = 0;
		virtual vector<GameMenuItem*> GetChildren() = 0;
		virtual int GetChildCount() = 0;
		virtual string GetDisplayText() = 0;

		int GetID();
		void AddGameMenuActionListener(GameMenuActionListener * listener);
		void SetParent(GameMenuParentItem * parent);
		void FireAction(int action);
		GameMenuParentItem * GetParent();

		GameMenuItem(int id);
		~GameMenuItem(void);
	};

	class GameMenuParentItem: public GameMenuItem {
	protected:
		int selectedChild;
		string text;
		vector<GameMenuItem*> children;
	public:
		bool IsLeaf();
		vector<GameMenuItem*> GetChildren();
		int GetChildCount();
		int GetSelectedChild();
		void SetSelectedChild(int index);
		void AddChild(GameMenuItem * child);
		string GetDisplayText();

		GameMenuParentItem(int id, string text);	
		GameMenuParentItem(int id, string text, vector<GameMenuItem*> children);
		~GameMenuParentItem(void);
	};

	class GameMenuLeafItem: public GameMenuItem {
	protected:
		string text;
		vector<GameMenuItem*> children;
	public:
		bool IsLeaf();
		vector<GameMenuItem*> GetChildren();
		int GetChildCount();
		string GetDisplayText();

		GameMenuLeafItem(int id, string text);
		~GameMenuLeafItem(void);
	};

	class GameMenuSelectItemOption {
	private:
		int dataSize;
		char * data;
		string displayText;
	public:

		GameMenuSelectItemOption(string displayText, int dataSize, void * pointer);
		
		string GetDisplayText();
		void * GetData();
		int GetDataSize();

	};

	class GameMenuSelectItem : public GameMenuLeafItem
	{
	protected:
		int selectedOption;
		vector<GameMenuSelectItemOption> selectOptions;
	public:
		static const string SEPARATOR;

		string GetDisplayText();

		int GetSelectedOption();
		void SetSelectedOption(int index);

		void SetSelectedValue(char * data, int dataSize);
		char * GetSelectedValue();

		int GetOptionCount();
		vector<GameMenuSelectItemOption> GetOptions();


		GameMenuSelectItem(int id, string text, vector<GameMenuSelectItemOption> selectOptions);
		~GameMenuSelectItem(void);
	};

	class GameMenuInputItem : public GameMenuLeafItem
	{
	protected:
		bool inputMode;
		int selectedCharacter;
		int dataSize;
		char * data;
	
		virtual bool AcceptCharacter(char value) = 0;
		virtual string GetFormattedData() = 0;
	public:

		GameMenuInputItem(int id, string text, int dataSize);

		bool IsInputMode();
		void SetInputMode(bool value);

		char * GetData();
		void SetData(char * data);
		int GetDataSize();

		void SetSelectedIndex(int index);
		int GetSelectedIndex();

		void SetCurrentCharacter(char value);
		
		string GetDisplayText();
	};

	class GameMenuCodeInputItem : public GameMenuInputItem {
	protected:
		bool AcceptCharacter(char value);
		string GetFormattedData();
	public:
		GameMenuCodeInputItem(int id);
		void SetCode(__int64 code);
		__int64 GetCode();
	};

	class GameMenuCurrentLevel : public GameMenuLeafItem {
	protected:
		int currentLevel;
	public:

		GameMenuCurrentLevel(int id, string text);
		void SetCurrentLevel(int level);
		int GetCurrentLevel();
		string GetDisplayText();
	};

}