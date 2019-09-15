#ifndef SERVER_H
#define SERVER_H

#include "Standard.h"
#include "LinkedList.h"

struct Message;
class Game;

class Server
{
    public:
        Server();
        virtual ~Server();

        void tick(int ms);

        //void join(Pipe* client);
        //void disconnect(Pipe* client);

        void sendMessageToAllClients(Message message);
        void sendMessageToClient(Message message, int playerID);
        void recieveMessage(Message message);
    protected:

    private:
        // game entity
        Game* game_;

        // clients
        //LinkedList<Pipe*>* clients_;
        void resyncClient(int clientID);
        int getClientIndexOfPlayer(int index);
        int getPlayerIndexOfClient(int index);

        // message passing utils
        void processMessage(Message* msg);
        LinkedList<Message*>* msgQueue_;
};

#endif // SERVER_H
