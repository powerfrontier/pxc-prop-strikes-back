#include <GameServerProfile.h>
#include <InstRouter.h>
#include <InstControl.h>

GameServerProfile::GameServerProfile() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(2, new ServerLoadOrderRcvd::Creator("ServerLoadOrderRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(3, new SetZoneOrderRcvd::Creator("SetZoneOrderRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(4, new GetZoneOrderRcvd::Creator("GetZoneOrderRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(6, new SetServerIpRvcd::Creator("SetServerIpRvcd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(7, new SetZoneToServerRcvd::Creator("SetZoneToServerRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(8, new AddUserRcvd::Creator("AddUserRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(9, new DelUserRcvd::Creator("DelUserRcvd")));
	
	mCreatorIds.push_back(std::pair<std::string, int>("ServerLoadAnswerSend", 0));
	mCreatorIds.push_back(std::pair<std::string, int>("GetServerIpSend", 1));
}

GameServerProfile::~GameServerProfile() {
	auto it = mCreators.begin();
	
	while (it != mCreators.end()) {
		delete it->second;
		++it;
	}
}
