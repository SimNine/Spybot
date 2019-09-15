#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class MainScreenGlow;
class PromptBox;
class GUIButton;
class GUITextbox;

class MainScreen : public GUIContainer
{
    public:
        MainScreen();
        virtual ~MainScreen();

        void draw();
        void tick(int);

        void toggleOptions();

		void showIPEntry();
		void hideIPEntry();
		void submitIPEntry();

		void loginShow();
		void loginHide();
		void loginCreate();
		void loginSubmit();
		void loginClear();

		void hideMainContainer();
		void showMainContainer();

		void keyPress(char c);
    protected:
    private:
        int textBkgDisplacement_;

        LinkedList<MainScreenGlow*>* glowList_;

        GUIContainer* optionsContainer_;
        GUIContainer* mainContainer_;

		GUIContainer* IPEntryContainer_;
		PromptBox* IPEntryBox_;

		GUIContainer* loginEntryContainer_;
		GUIButton* loginCancelButton_;
		GUIButton* loginCreateButton_;
		GUIButton* loginSubmitButton_;
		GUITextbox* loginUsername_;
		GUITextbox* loginPassword_;
};

#endif // MAINSCREEN_H
