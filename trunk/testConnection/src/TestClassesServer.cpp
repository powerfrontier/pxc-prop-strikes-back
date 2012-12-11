#include <TestClassesServer.h>
#include <iostream>
#include <Connection.h>

TestTransferableSent::TestTransferableSent()    : Datagram<TestTransferableSent>("TestTransferableSent")
                                                , m_1(0)
                                                , m_2(0)
                                                , m_3(0)
                                                , m_4(0) {

}

TestTransferableSent::~TestTransferableSent() {

}

test::test() : Datagram<test>("test"){

}

test::~test(){
}

void test::exec(Connection *c) const throw() {
        std::cout << " ME HA LLEGADO LA INSTR 2, envio" << std::endl;
        TestTransferableSent* sent = NULL;
        sent = new TestTransferableSent();
        c->sendAnswer(*sent);
}

TestTransferableRcvd::TestTransferableRcvd()    : Datagram<TestTransferableRcvd>("TestTransferableRcvd")
                                                , m_zona(0)
                                                , m_server(0){

}
TestTransferableRcvd::TestTransferableRcvd(int zone, int server): Datagram<TestTransferableRcvd>("TestTransferableRcvd")
                                                , m_zona(zone)
                                                , m_server(server){

}
TestTransferableRcvd::~TestTransferableRcvd() {

}

void TestTransferableRcvd::exec(Connection* c) const throw() {
        std::cout <<"Ha llegado una zona: " << m_zona << " del servidor: " << m_server << std::endl;
        std::string n = c->getPort();
        std::cout << "Por el puerto: " << n << std::endl;
}

TestProfile::TestProfile() : TransferableProfile() {
        mCreators.push_back(std::pair<int, TransferableCreator*>(2, new test::Creator("test")));
        mCreators.push_back(std::pair<int, TransferableCreator*>(7, new TestTransferableRcvd::Creator("TestTransferableRcvd")));
        mCreatorIds.push_back(std::pair<std::string, int>("TestTransferableSent", 0));
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
}
*/
