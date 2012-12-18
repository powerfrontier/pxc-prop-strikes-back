#include <ClientProfile.h>
#include <InstLogin.h>
#include <InstRouter.h>
#include <InstServer.h>


ClientProfile::ClientProfile() {
	//Login
	mCreators.push_back(std::pair<int, TransferableCreator*>(101, new LoginRequestRcvd::Creator("LoginRequestRcvd")));
	mCreatorIds.push_back(std::pair<std::string, int>("LoginRequestSend", 100));
	
	//Router
	
	mCreators.push_back(std::pair<int, TransferableCreator*>(201, new ConnectCLientReceived::Creator("ConnectCLientReceived")));
	mCreatorIds.push_back(std::pair<std::string, int>("ConnectClientSend", 200));
	
	//Server
	mCreators.push_back(std::pair<int, TransferableCreator*>(500, new SetUserStatusRcvd::Creator("SetUserStatusRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(501, new SetZoneRcvd::Creator("SetZoneRcvd")));
	
	mCreatorIds.push_back(std::pair<std::string, int>("SetZoneSend", 501));
	mCreatorIds.push_back(std::pair<std::string, int>("SetDestinationSend", 502));
	
}

ClientProfile::~ClientProfile() {
}
