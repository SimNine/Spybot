#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class MainScreenGlow;
class TextEntryBox;

class MainScreen : public GUIContainer
{
    public:
        MainScreen();
        virtual ~MainScreen();

        void draw();
        void tick(int);

        void toggleOptions();

		void showNameEntry();
		void cancelNameEntry();
		void submitNameEntry();

		void showIPEntry();
		void cancelIPEntry();
		void submitIPEntry();

		void keyPress(char c);
    protected:
    private:
        int textBkgDisplacement;

        LinkedList<MainScreenGlow*>* glowList;

        GUIContainer* optionsContainer;
        GUIContainer* mainContainer;
		GUIContainer* nameEntryContainer;
		TextEntryBox* nameEntryBox;
		GUIContainer* IPEntryContainer;
		TextEntryBox* IPEntryBox;
};

#endif // MAINSCREEN_H
