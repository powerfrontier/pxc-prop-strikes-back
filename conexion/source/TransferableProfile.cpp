#include <Transferable.h>

TransferableProfile::TransferableProfile() : mCreators(), mCreatorIds() {}

TransferableProfile::~TransferableProfile() {
	Creators::iterator it = mCreators.begin();
	
	while(it != mCreators.end()) {
		delete it->second;
		++it;
	}
}

const TransferableProfile::Creators& TransferableProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	return mCreators;
}

const TransferableProfile::CreatorIds& TransferableProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	return mCreatorIds;
}