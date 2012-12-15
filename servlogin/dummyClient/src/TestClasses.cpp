#include <TestClasses.h>
#include <iostream>
#include <Connection.h>


TestTransferableSent100::TestTransferableSent100(char* user, char* pwd) 	: Datagram<TestTransferableSent100>("TestTransferableSent100")
{
	      int i;
  		for( i = 0; i < 32; ++i){
			username[i] = user[i];			
		}
		i = 0;
		for( i = 0; i < 32; ++i){
			password[i] = pwd[i];
			if(pwd[i] == '\0' ) {
			// printf("ladsa\n");
		std::cout << "lala " << password << std::endl;
		fflush(stdout); 
			  return;
			}
		}  
		
}

TestTransferableSent100::~TestTransferableSent100() {

}

TestTransferableSent102::TestTransferableSent102(int idClient,int idToken) 	: Datagram<TestTransferableSent102>("TestTransferableSent102"), clientId(idClient),token(idToken) {

}

TestTransferableSent102::~TestTransferableSent102() {

}

TestTransferableRcvd101::~TestTransferableRcvd101() {

}

void TestTransferableRcvd101::exec(Connection* c) const throw() {
	std::cout <<"Ha llegado una instruccion 101: " << std::endl;
	std::cout << "answerCode: " << answerCode << std::endl;
	std::cout << "Idcliente: " << clientId << std::endl;
	std::cout << "Token: " << token << std::endl;
}


TestTransferableRcvd103::~TestTransferableRcvd103() {

}

void TestTransferableRcvd103::exec(Connection* c) const throw() {
	
	std::cout <<"Ha llegado una instruccion 103 con answerCode "  << answerCode << std::endl;
}

TestProfile::TestProfile() : TransferableProfile(){
	mCreators.push_back(std::pair<int, TransferableCreator*>(101, new TestTransferableRcvd101::Creator("TestTransferableRcvd101")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(103, new TestTransferableRcvd103::Creator("TestTransferableRcvd103")));

	mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent100", 100));
	mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent102", 102));
}

TestProfile::~TestProfile() {
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
}
/*
const TransferableProfile::Creators& TestProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreators;
	else throw TransferableVersionException("Unknown protocol");
}

const TransferableProfile::CreatorIds& TestProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreatorIds;
	else throw TransferableVersionException("Unknown protocol");
}*/
