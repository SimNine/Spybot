#ifndef CLIENT_H
#define CLIENT_H

#include "Standard.h"
#include "LinkedList.h"

struct Message;
class Game;
class Player;

class Client
{
    public:
        Client();
        virtual ~Client();

        void connectIP(std::string IP);
        void disconnect();

		void listen();
        void sendMessage(Message message);
        void recieveMessage(Message message);
		void processAllMessages();

		Player* getPlayer();
		void setPlayer(Player* player);

		Game* getGame();
		void setGame(Game* game);

		LinkedList<int*>* getClientList();
    protected:

    private:
        // game entity (NULL when no game is being played)
		Game* game_;
		Player* player_;
		SOCKET socket_;
		int myClientID_;

        // message passing utils
        void processMessage(Message* msg);
        LinkedList<Message*>* msgQueue_;
		std::mutex mtx;

		// list of all clients connected to server (null if no connection)
		LinkedList<int*>* clientList_;
};

void listenOnSocket(Client* c);

#endif // SERVER_H
