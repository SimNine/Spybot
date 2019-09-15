#ifndef PIPE_H
#define PIPE_H

#include "Standard.h"

class Server;
struct Message;
class Player;

class Pipe
{
    public:
        Pipe(SOCKET socket, Server* server);
        virtual ~Pipe();

        void listenData();
        void sendData(Message m);

		int getClientID();

		std::string getName();
		void setName(std::string name);

		Player* getPlayer();
		void setPlayer(Player* p);
		void kill();
		bool isDead();
    protected:

    private:
		SOCKET socket_;
        Server* server_;
		int clientID_;
		std::string name_;

		Player* player_;

		bool dead_;
};

#endif // PIPE_H
