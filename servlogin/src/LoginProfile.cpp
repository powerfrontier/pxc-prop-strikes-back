#include <LoginProfile.h>
#include <iostream>


using namespace std;

LoginProfile::LoginProfile() : TransferableProfile(), mCreators(), mCreatorIds() {
//	mCreators.push_back(std::pair<int, TransferableCreator*>(0, new ServerLoadRcvd::Creator("ServerLoadRcvd"))); // Recibo x con id y	
//	mCreatorIds.push_back(std::pair<std::string, int>("ServerLoadSend", 1)); // Envio x con id y

}

LoginProfile::~LoginProfile() {
	cout << "Destruct LoginProfile";
	fflush(stdout);
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
	cout << "End Destruct controlProfile";
	fflush(stdout);
}


