#ifndef _TEST_CLASSESSERVER_H_
#define _TEST_CLASSESSERVER_H_

#include <Datagram.h>

//Orden que se envía
class TestTransferableSent : public Datagram<TestTransferableSent> {
        int m_1;
        int m_2;
        double m_3;
        int m_4;

public:
        TestTransferableSent();
        virtual ~TestTransferableSent();
       
        TestTransferableSent& m1(int);
};

class test : public Datagram<test> {
        public:
                test();
                virtual ~test();
               
                void exec(Connection *c) const throw();
};

//Orden que se recibe
class TestTransferableRcvd : public Datagram<TestTransferableRcvd> {
        int m_zona;
        int m_server;
public:
        TestTransferableRcvd();
        TestTransferableRcvd(int zone, int server);
        virtual ~TestTransferableRcvd();
       
        void exec(Connection* c) const throw();
};

class TestProfile : public TransferableProfile {
       
public:
        TestProfile();
        virtual ~TestProfile();
        //const TransferableProfile::Creators& getCreators(const std::string& protocol) const throw(TransferableVersionException&);
        //const TransferableProfile::CreatorIds& getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&);
};

#endif

