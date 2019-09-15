#ifndef CLIENT_H
#define CLIENT_H

#include "Standard.h"
#include "LinkedList.h"

struct Message;
class Game;
class Player;
class ClientMirror;

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

		ClientMirror* getClientMirrorByClientID(int clientID);
		ClientMirror* getClientMirrorByPlayerID(int playerID);
		LinkedList<ClientMirror*>* getClientList();
		ClientMirror* getServerOwner();
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
		LinkedList<ClientMirror*>* clientList_;
		ClientMirror* serverOwner_;
};

#endif // SERVER_H
