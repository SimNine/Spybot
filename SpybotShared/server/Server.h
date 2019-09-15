#ifndef SERVER_H
#define SERVER_H

#include "Standard.h"
#include "LinkedList.h"

class Pipe;
struct Message;
class Game;
class Player;
class User;

class Server {
public:
	Server(bool isLocal, std::string savePath);
	virtual ~Server();

	void processAllMessages();

	void connect(SOCKET client);
	void disconnect(Pipe* client);
	void login(Pipe* client, User* user);

	void sendMessageToClient(Message message, int clientID);
	void sendMessageToAllClients(Message message);
	void sendMessageToAllClientsExcept(Message message, int clientID);
	void recieveMessage(Message message);

	LinkedList<Pipe*>* getClientList();
	void processAITurn(Player* p);

	Game* getGame();
	LinkedList<User*>* getUsers();
	Pipe* getOwner();
	bool isLocal();
protected:

private:
	// level and userdata path
	std::string savePath_;

	// locality modifier
	bool isLocal_;

	// game
	Game* game_;
	GAMEMODE gameMode_;

	// clients
	Pipe* ownerClient_;
	LinkedList<Pipe*>* clients_;
	Pipe* getClientByID(int clientID);

	// users
	LinkedList<User*>* users_;
	void loadUsers();
	void saveUsers();
	User* getUserByName(std::string name);

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
