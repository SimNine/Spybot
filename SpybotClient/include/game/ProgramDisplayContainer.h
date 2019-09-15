#ifndef PROGRAMDISPLAYCONTAINER_H
#define PROGRAMDISPLAYCONTAINER_H

#include "Standard.h"
#include "GUIContainer.h"

class Program;
class GUITexture;
class ProgramDisplayActionButton;

class ProgramDisplayContainer : public GUIContainer
{
    public:
        ProgramDisplayContainer(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent);
        virtual ~ProgramDisplayContainer();

        void draw();

		void setCurrProg(Program* p);
    protected:
    private:
		Program* currProg_;

		GUITexture* iconBacking_;
		GUITexture* icon_;
		GUITexture* nameText_;
		GUITexture* ownerText_;
		GUITexture* healthText_;
		GUITexture* movesText_;
		GUITexture* actionsText_;
		GUITexture* descText_;

		LinkedList<ProgramDisplayActionButton*>* actionButtons_;
};

#endif // PROGRAMDISPLAYCONTAINER_H
