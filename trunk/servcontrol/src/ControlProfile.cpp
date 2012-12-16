#include <ControlProfile.h>
#include <ServerLoad.h>
#include <IPServer.h>
#include <NewUser.h>
#include <RouterNewUser.h>
#include <RouterDeleteUser.h>
#include <GameNewUser.h>
#include <GameDeleteUser.h>
#include <DisconnectUser.h>
#include <iostream>


using namespace std;

ControlProfile::ControlProfile() : TransferableProfile() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(0, new ServerLoadRcvd::Creator("ServerLoadRcvd"))); // Recibo x con id y
	mCreators.push_back(std::pair<int, TransferableCreator*>(1, new IPServerRcvd::Creator("IPServerRcvd"))); // Recibo x con id y
	mCreators.push_back(std::pair<int, TransferableCreator*>(50, new NewUserRcvd::Creator("NewUserRcvd"))); // Recibo x con id y
	mCreators.push_back(std::pair<int, TransferableCreator*>(52, new DisconnectUserRcvd::Creator("DisconnectUserRcvd"))); // Recibo x con id y

	mCreatorIds.push_back(std::pair<std::string, int>("ServerLoadSend", 2)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("RemoveZoneSend", 3)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("GetZoneSend", 4)); // Envio x con id y	
	mCreatorIds.push_back(std::pair<std::string, int>("IPServerSend", 6)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("SetZoneToServerSend", 7)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("GameNewUserSend", 8)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("GameDeleteUserSend", 9)); // Envio x con id y

	mCreatorIds.push_back(std::pair<std::string, int>("LoginOnlineSend", 51)); // Envio x con id y

	mCreatorIds.push_back(std::pair<std::string, int>("RouterChangeZoneSend", 5)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("RouterNewUserSend", 60)); // Envio x con id y
	mCreatorIds.push_back(std::pair<std::string, int>("RouterDeleteUserSend", 61));
	
}

ControlProfile::~ControlProfile() {
		cout << "Destruct controlProfile";
	fflush(stdout);
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
	cout << "End Destruct controlProfile";
	fflush(stdout);
}


