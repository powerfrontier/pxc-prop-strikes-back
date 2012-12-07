#include <Transferable.h>
#include <iostream>

TransferableProfile::TransferableProfile() : mCreators(), mCreatorIds() {}

TransferableProfile::~TransferableProfile() {
	Creators::iterator it = mCreators.begin();
	
	while(it != mCreators.end()) {
		delete it->second;
		++it;
	}
}

const TransferableProfile::Creators& TransferableProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	std::cout << "mCreators.size() " << mCreators.size() << std::endl; 
	return mCreators;
}

const TransferableProfile::CreatorIds& TransferableProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	std::cout << "mCreatorIds.size() " << mCreatorIds.size() << std::endl; 
	return mCreatorIds;
}