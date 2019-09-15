#ifndef USER_H
#define USER_H

#include "Standard.h"

class User
{
    public:
        User();
        virtual ~User();

		std::string username_;
		std::string password_;
		int progs_[PROGRAM_NUM_PROGTYPES];
		int numWins_;
		int numLosses_;
		int numDraws_;
		int numResigns_;
		int numGames_;
		
		bool loggedIn_;
    protected:

    private:
};

#endif // PIPE_H
