#include <RouterProfile.h>

RouterProfile::RouterProfile() {
	
	//Control
	mCreatorIds.push_back(std::pair<std::string, int>("GetServerIpSend"), 1);
	
	mCreators.push_back(std::pair<int, TransferableCreator*>(6, new GetServerIpRvcd::Creator("GetServerIpRvcd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(7, new SetZoneToServerRcvd::Creator("SetZoneToServerRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(60, new AddUserRcvd::Creator("AddUserRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(61, new DelUserRcvd::Creator("DelUserRcvd")));
	
	//mCreators.push_back(std::pair<int, TransferableCreator*>(, ));
	//mCreators.push_back(std::pair<int, TransferableCreator*>(, ));
	
	//Router
	mCreatorIds.push_back(std::pair<std::string, int>(150, "AddRouterSend"));
	
	mCreators.push_back(std::pair<int, TransferableCreator*>(155, new EndZoneTransferRcvd::Creator("EndZoneTransferRcvd")));
	
	//mCreators.push_back(std::pair<int, TransferableCreator*>(, ));
	//mCreators.push_back(std::pair<int, TransferableCreator*>(, ));
	
	//Clients
}

RouterProfile::~RouterProfile() {
	
}

