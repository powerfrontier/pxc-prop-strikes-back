#include <Zone.h>

Zone::Zone(int zoneId) : mZoneId(zoneId), mUsers() { }

Zone::~Zone() { }

int Zone::id() const { return mZoneId; }

const std::set<int>& Zone::users() const { return mUsers; }

bool Zone::hasUser(int idUser) const {
	std::set<int>::const_iterator i = mUsers.find(idUser);
	return i != mUsers.end();
}