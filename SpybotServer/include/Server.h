#ifndef SERVER_H
#define SERVER_H

#include "Standard.h"
#include "LinkedList.h"

class Pipe;
struct Message;
class Game;

class Server
{
    public:
        Server();
        virtual ~Server();

        void tick(int ms);

        void connect(SOCKET client);
        void disconnect(Pipe* client);

		void sendMessageToClient(Message message, int clientID);
        void sendMessageToAllClients(Message message);
		void sendMessageToAllClientsExcept(Message message, int clientID);
        void recieveMessage(Message message);

		LinkedList<Pipe*>* getClientList();
		void processAITurns();

		void processCommandLoop();
		void processCommand(std::string cmd);
    protected:

    private:
        // game
        Game* game_;

        // clients
		Pipe* ownerClient_;
        LinkedList<Pipe*>* clients_;
		Pipe* getClientByID(int clientID);

		// message processing core
		void processMessage(Message* msg);
		LinkedList<Message*>* msgQueue_;
		std::mutex mtx;

		// message processing helpers
        void resyncClient(int clientID);
		void resyncAll();
		void quitAll();
};

#endif // SERVER_H
