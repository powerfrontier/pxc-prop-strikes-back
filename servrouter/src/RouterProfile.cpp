#include <RouterProfile.h>
#include <InstControl.h>
#include <InstServer.h>

RouterProfile::RouterProfile() {
	
	//Control
	mCreatorIds.push_back(std::pair<std::string, int>("GetServerIpSend", 1));
	
	mCreators.push_back(std::pair<int, TransferableCreator*>(5, new ChangeZoneRcvd::Creator("ChangeZoneRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(6, new GetServerIpRvcd::Creator("GetServerIpRvcd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(7, new SetZoneToServerRcvd::Creator("SetZoneToServerRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(60, new AddUserRcvd::Creator("AddUserRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(61, new DelUserRcvd::Creator("DelUserRcvd")));
	
	//Server
	mCreatorIds.push_back(std::pair<std::string, int>("AddUserSend", 8));
	mCreatorIds.push_back(std::pair<std::string, int>("DelUserSend", 9));
	mCreatorIds.push_back(std::pair<std::string, int>("AddRouterSend", 150));
	
	mCreators.push_back(std::pair<int, TransferableCreator*>(8, new SetAddUserRcvd::Creator("SetAddUserRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(9, new SetDelUserRcvd::Creator("SetDelUserRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(155, new EndZoneTransferRcvd::Creator("EndZoneTransferRcvd")));
	
	//Clients
	mCreatorIds.push_back(std::pair<std::string, int>("ConnectCLientSend", 201));
	
	mCreators.push_back(std::pair<int, TransferableCreator*>(200, new ConnectCLientRcvd::Creator("ConnectCLientRcvd")));
}

RouterProfile::~RouterProfile() {
	
}

