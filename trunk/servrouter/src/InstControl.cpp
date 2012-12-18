#include <InstControl.h>
#include <Router.h>
#include <Connection.h>

#include <iostream>

//SENT

//ID 1
GetServerIpSend::GetServerIpSend(int serverId) : Datagram<GetServerIpSend>("GetServerIpSend"), mServerId(serverId) { }
GetServerIpSend::~GetServerIpSend() { }


//RECEIVED

//ID 5
ChangeZoneRcvd::ChangeZoneRcvd() : Datagram<ChangeZoneRcvd>("ChangeZoneRcvd") { }
ChangeZoneRcvd::~ChangeZoneRcvd() { }

void ChangeZoneRcvd::exec(Connection* c) const throw() {
	//TODO
}

//ID 6
GetServerIpRvcd::GetServerIpRvcd() : Datagram<GetServerIpRvcd>("GetServerIpRvcd") { }
GetServerIpRvcd::~GetServerIpRvcd() { }

void GetServerIpRvcd::exec(Connection* c) const throw() {
	Connection* newConn = new TCPConnectionSecurity();
	c->connect(std::string(mIpServer), std::string(mPortServer));
	
	Router::instance().connectServer(mServerId, newConn);
}


//ID 7: Initialize (not change) a new server/zone pair, create both if not exist
SetZoneToServerRcvd::SetZoneToServerRcvd() : Datagram<SetZoneToServerRcvd>("SetZoneToServerRcvd") { }
SetZoneToServerRcvd::~SetZoneToServerRcvd() { }

void SetZoneToServerRcvd::exec(Connection* c) const throw() {
	if (mIdServer != -1) {
		GetServerIpSend getIp(mIdServer);
		Router::instance().addServer(mIdServer);
		c->sendAnswer(getIp);
	}
	if (mIdZone != -1) Router::instance().addZone(mIdZone);
	
	Router::instance().startZoneToServerTransfer(mIdZone, mIdServer);
	Router::instance().endZoneToServerTransfer(mIdZone);
}


//ID 60: Add a new user/modify it's current token
AddUserRcvd::AddUserRcvd() : Datagram<AddUserRcvd>("AddUserRcvd") { }
AddUserRcvd::~AddUserRcvd() { }

void AddUserRcvd::exec(Connection* c) const throw() {
	Router::instance().addUser(mIdUser, mToken);
	Router::instance().setUserToZone(mIdUser, mZoneId);
}


//ID 61: Removes a currently active user
DelUserRcvd::DelUserRcvd() : Datagram<DelUserRcvd>("DelUserRcvd") { }
DelUserRcvd::~DelUserRcvd() { }

void DelUserRcvd::exec(Connection* c) const throw() {
	Router::instance().delUser(mUserId);
}
