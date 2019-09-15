#pragma once

#include "Standard.h"
#include "LinkedList.h"
#include "GameConfig.h"

class Pipe;
struct Message;
class Game;
class Player;
class User;

class Server {
public:
	Server(bool isLocal, CAMPAIGN campaign);
	virtual ~Server();

	void processAllMessages();

	Pipe* connect(SOCKET client);
	void disconnect(Pipe* client);
	void login(Pipe* client, User* user);
	void tryLogin(Pipe* client, Message message);

	void sendMessageToNonLoggedInClients(Message message);
	void sendMessageToClient(Message message, int clientID);
	void sendMessageToAllClients(Message message);
	void sendMessageToAllClientsExcept(Message message, int clientID);
	void recieveMessage(Message message);

	LinkedList<Pipe*>* getClientList();
	void processAITurn(Player* p);

	Game* getGame();
	LinkedList<User*>* getUsers();
	User* getUserByName(std::string name);
	Pipe* getOwner();
	bool isLocal();

	void pingSender();

	std::string getCurrentLevel();
	std::string getSavePath();

	void loadUsers();
	void saveUsers();
protected:

private:
	// game configuration
	GameConfig config_;

	// locality modifier
	bool isLocal_;

	// game
	Game* game_;

	// clients
	Pipe* ownerClient_;
	LinkedList<Pipe*>* clients_;
	Pipe* getClientByID(int clientID);

	// users
	LinkedList<User*>* users_;

	// message processing core
	void processMessage(Message* msg);
	LinkedList<Message*>* msgQueue_;

	// mutexes
	std::mutex msgMutex_;

	// message processing helpers
	void resyncClient(int clientID);
	void resyncAll();
	void quitAll();
};