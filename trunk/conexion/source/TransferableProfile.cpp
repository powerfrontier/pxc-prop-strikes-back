#include <Transferable.h>

TransferableProfile::TransferableProfile() : mCreators(), mCreatorIds {}

virtual TransferableProfile::~TransferableProfile() {
	Creators::iterator it = mCreators.begin();
	
	while(it != mCreators.end()) {
		delete it->second;
		++it;
	}
}

const Creators& TransferableProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	return mCreators;
}

const CreatorIds& TransferableProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	return mCreatorIds;
}