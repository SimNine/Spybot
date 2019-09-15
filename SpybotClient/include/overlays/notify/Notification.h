#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Standard.h"

class Notification {
public:
	Notification(std::string content, int duration, int size);
	virtual ~Notification();

	void tick(int ms);
	void setTimeLeft(int ms);
	int getTimeLeft();

	void setSize(int size);
	int getSize();

	std::string getContent();
protected:
private:
	std::string content_;

	int timeLeft_;
	int textSize_;
};

#endif // NOTIFICATION_H
