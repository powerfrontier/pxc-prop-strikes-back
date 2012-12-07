#ifndef _LOGIN_PROFILE_H_
#define _LOGIN_PROFILE_H_

#include <Datagram.h>

class LoginProfile : public TransferableProfile {
	std::vector<std::pair<int, TransferableCreator*> > mCreators;
	std::vector<std::pair<std::string, int> > mCreatorIds;
public:
	LoginProfile();
	virtual ~LoginProfile();
};

#endif

