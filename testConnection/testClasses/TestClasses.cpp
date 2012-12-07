#include <TestClasses.h>
#include <iostream>
#include <Connection.h>

TestTransferableSent::TestTransferableSent() 	: Datagram<TestTransferableSent>("TestTransferableSent")
						, m_1(0)
						, m_2(0)
						, m_3(0)
						, m_4(0) {

}

TestTransferableSent::~TestTransferableSent() {

}

TestTransferableSent& TestTransferableSent::m1(int m) {
	m_1 = m;
	return *this;
}


TestTransferableRcvd::TestTransferableRcvd() 	: Datagram<TestTransferableRcvd>("TestTransferableRcvd")
						, m_1(0)
						, m_2(0)
						, m_3(0)
						, m_4(0) {

}

TestTransferableRcvd::~TestTransferableRcvd() {

}

void TestTransferableRcvd::exec(Connection* c) const throw() {
	std::cout <<"Ha llegado un: " << m_1 << std::endl;
	std::string n = c->getIp();
	std::cout << "FUUUUU" << std::endl;
	std::cout << n << std::endl;
	c->send(((new TestTransferableSent())->m1(m_1+1)));
}

TestProfile::TestProfile() : TransferableProfile(), mCreators(), mCreatorIds() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(0, new TestTransferableRcvd::Creator("TestTransferableRcvd")));

	mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent", 0));
}

TestProfile::~TestProfile() {
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
}

const TransferableProfile::Creators& TestProfile::getCreators(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreators;
	else throw TransferableVersionException("Unknown protocol");
}

const TransferableProfile::CreatorIds& TestProfile::getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&) {
	if(protocol == "0.1a") return mCreatorIds;
	else throw TransferableVersionException("Unknown protocol");
}
