#pragma once

#include "GUIContainer.h"
#include "LinkedList.h"

class ChatDisplay : public GUIContainer {
public:
	ChatDisplay(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, int historySize);
	~ChatDisplay();

	void draw();
	void tick(int ms);

	std::string getInput();
	void clearInput();
	void addInputChar(char c);

	void addMessage(std::string);

	bool isActive();
	void setActive(bool b);
private:
	bool active_;
	std::string inputText_;

	int logSize_;
	LinkedList<std::string*>* chatLog_;
	LinkedList<int*>* chatLogTiming_;

	const int textSize_ = 20;
};

