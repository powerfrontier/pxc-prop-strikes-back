#include <ControlProfile.h>
#include <ServerLoad.h>

ControlProfile::ControlProfile() : TransferableProfile(), mCreators(), mCreatorIds() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(0, new ServerLoadRcvd::Creator("ServerLoadRcvd"))); // Recibo x con id y

	mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent", 0)); // Envio x con id y
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
