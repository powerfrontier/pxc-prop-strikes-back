#include <ControlProfile.h>

ControlProfile::ControlProfile() : TransferableProfile(), mCreators(), mCreatorIds() {
	//mCreators.push_back(std::pair<int, TransferableCreator*>(0, new SendServerLoad::Creator("SendServerLoad")));

	mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent", 0));
}

ControlProfile::~ControlProfile() {
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
}

const TransferableProfile::Creators& ControlProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreators;
	else throw TransferableVersionException("Unknown protocol");
}

const TransferableProfile::CreatorIds& ControlProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreatorIds;
	else throw TransferableVersionException("Unknown protocol");
}
