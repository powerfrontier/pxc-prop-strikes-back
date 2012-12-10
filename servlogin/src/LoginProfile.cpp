#include <LoginRequest.h>
#include <LogoutRequest.h>
#include <LoginProfile.h>
#include <iostream>


using namespace std;

LoginProfile::LoginProfile() : TransferableProfile(), mCreators(), mCreatorIds() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(100, new LoginRequestRcvd::Creator("LoginRequestRcvd"))); 
	mCreators.push_back(std::pair<int, TransferableCreator*>(102, new LogoutRequestRcvd::Creator("LogoutRequestRcvd")));
	
	
	mCreatorIds.push_back(std::pair<std::string, int>("LoginRequestSend", 101)); 
	mCreatorIds.push_back(std::pair<std::string, int>("LogoutRequestSend", 103));

}

LoginProfile::~LoginProfile() {
	cout << "Destruct LoginProfile";
	fflush(stdout);
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
	cout << "End Destruct controlProfile";
	fflush(stdout);
}


