#ifndef CLIENTMIRROR_H
#define CLIENTMIRROR_H

#include "Standard.h"
#include "LinkedList.h"

class Player;

class ClientMirror
{
    public:
        ClientMirror();
        virtual ~ClientMirror();

		int clientID_;
		Player* player_;
		std::string name_;
		bool owner_;
    protected:

    private:
};

#endif // SERVER_H
