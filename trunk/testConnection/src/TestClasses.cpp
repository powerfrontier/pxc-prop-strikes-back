#include <TestClasses.h>
#include <iostream>
#include <Connection.h>

TestTransferableSent7::TestTransferableSent7()  : Datagram<TestTransferableSent7>("TestTransferableSent7")
                                                , m_zona(0)
                                                , m_server(0) {

}

TestTransferableSent7::~TestTransferableSent7() {

}

TestTransferableSent2::TestTransferableSent2()  : Datagram<TestTransferableSent2>("TestTransferableSent2") {

}

TestTransferableSent2::~TestTransferableSent2() {

}

TestTransferableRcvd::TestTransferableRcvd()    : Datagram<TestTransferableRcvd>("TestTransferableRcvd")
                                                , m_1(0)
                                                , m_2(0)
                                                , m_3(0)
                                                , m_4(0) {

}

TestTransferableRcvd::~TestTransferableRcvd() {

}

void TestTransferableRcvd::exec(Connection* c) const throw() {
        std::cout <<"Ha llegado un: " << m_1 <<" " << m_2 <<" " << m_3 << " "<< m_4 << std::endl;
}

TestProfile::TestProfile() : TransferableProfile(){
        mCreators.push_back(std::pair<int, TransferableCreator*>(0, new TestTransferableRcvd::Creator("TestTransferableRcvd")));

        mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent2", 2));
        mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent7", 7));
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

