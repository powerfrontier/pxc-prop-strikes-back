#include <ControlProfile.h>
#include <ServerLoad.h>
#include <RemoveZone.h>
#include <GetZone.h>
#include <RouterChangeZone.h>
#include <iostream>

using namespace std;

ControlProfile::ControlProfile() : TransferableProfile(), mCreators(), mCreatorIds() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(0, new ServerLoadSend::Creator("ServerLoadRcvd"))); // Recibo x con id y

	mCreatorIds.push_back(std::pair<std::string, int>("ServerLoadSend", 1)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("RemoveZoneSend", 2)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("GetZoneSendSend", 3)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("RouterChangeZoneSend", 4)); // Envio x con id y
}

ControlProfile::~ControlProfile() {
		cout << "Destruct controlProfile";
	fflush(stdout);
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
	cout << "End Destruct controlProfile";
	fflush(stdout);
}

const TransferableProfile::Creators& ControlProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreators;
	else throw TransferableVersionException("Unknown protocol");
}

const TransferableProfile::CreatorIds& ControlProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreatorIds;
	else throw TransferableVersionException("Unknown protocol");
}
