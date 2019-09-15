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

		Player* getPlayer();
		void setPlayer(Player* p);

		void kill();
		bool isDead();
    protected:

    private:
		SOCKET socket_;
        Server* server_;
		int clientID_;

		Player* player_;

		bool dead_;
};

void listenToPipe(Pipe* p);

#endif // PIPE_H
