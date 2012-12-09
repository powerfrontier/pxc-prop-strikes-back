#ifndef _TEST_CLASSES_H_
#define _TEST_CLASSES_H_

#include <Datagram.h>

//Orden que se envía
class TestTransferableSent7 : public Datagram<TestTransferableSent7> {
	int m_zona;
	int m_server;
public:
	TestTransferableSent7();
	virtual ~TestTransferableSent7();
	
};

class TestTransferableSent2 : public Datagram<TestTransferableSent2> {
public:
	TestTransferableSent2();
	virtual ~TestTransferableSent2();
	
};


//Orden que se recibe
class TestTransferableRcvd : public Datagram<TestTransferableRcvd> {
	int m_1;
	int m_2;
	double m_3;
	int m_4;

public:
	TestTransferableRcvd();
	virtual ~TestTransferableRcvd();
	
	void exec(Connection* c) const throw();
};

class TestProfile : public TransferableProfile {
public:
	TestProfile();
	virtual ~TestProfile();
//	const TransferableProfile::Creators& getCreators(const std::string& protocol) const throw(TransferableVersionException&);
//	const TransferableProfile::CreatorIds& getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&);
};

#endif

