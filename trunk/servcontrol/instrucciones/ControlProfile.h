#ifndef _CONTROL_PROFILE_H_
#define _CONTROL_PROFILE_H_

#include <Datagram.h>

class ControlProfile : public TransferableProfile {
	std::vector<std::pair<int, TransferableCreator*> > mCreators;
	std::vector<std::pair<std::string, int> > mCreatorIds;
public:
	ControlProfile();
	virtual ~ControlProfile();
	const TransferableProfile::Creators& getCreators(const std::string& protocol) const throw(TransferableVersionException&);
	const TransferableProfile::CreatorIds& getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&);
};

#endif

