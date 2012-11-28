#include <Transferable.h>

const std::string TransferableFactory::NO_PROTOCOL = "none";

TransferableFactory::TransferableFactory() 	: mProtocolVersion(NO_PROTOCOL)
						, mCreators() {
	try {
		setLatestVersion();
	} catch ( TransferableVersionException& e) { mProtocolVersion = NO_PROTOCOL; }
}

TransferableFactory::~TransferableFactory() throw() {
	clear();
}

void TransferableFactory::clear() throw() {
	std::map<int, TransferableCreator*>::iterator it;

	for (it = mCreators.begin(); it != mCreators.end(); ++it) delete it->second;
	mCreators.clear();
}

void TransferableFactory::addCreator(TransferableCreator& creator) throw(TransferableVersionException&) {
	std::map<int, TransferableCreator*>::iterator it;
	int type = 0;

		type = creator.type();
		it = mCreators.find(type);

		if (it != mCreators.end()) {
			mCreators.insert(std::pair<int, TransferableCreator*>(type, &creator));
		} else throw TransferableVersionException("Creator already in factory");
}

void TransferableFactory::setLatestVersion() throw(TransferableVersionException&) {
	setProtocol(TRANSFERABLE_LAST_VERSION);
}

const std::string& TransferableFactory::protocolVersion() const throw() {
	return mProtocolVersion;
}

void TransferableFactory::setProtocol(const std::string& version) throw(TransferableVersionException&) {
	clear();
	
	try {
		if (version == NO_PROTOCOL) {

		} else if (version == "0.1a") {
			//TODO
			/* addCreator(new MyTransferableCreator());
			 *
			 *
			 */
		} else throw TransferableVersionException("Protocol unavailable");
	} catch (TransferableVersionException& e) {
		clear();
		throw e;
	}
}

int TransferableFactory::type(const std::string& type) const throw(TransferableVersionException&) {
	std::map<std::string, int>::const_iterator it;

	if (mProtocolVersion == NO_PROTOCOL) throw TransferableVersionException("No protocol stablished");

	it = mSendingType.find(type);
	if (it == mSendingType.end()) throw TransferableVersionException("Sending type not found");

	return it->second;
}

TransferableCreator* TransferableFactory::creator(int creatorType) const throw(TransferableVersionException&){
	std::map<int, TransferableCreator*>::const_iterator it;

	if (mProtocolVersion == NO_PROTOCOL) throw TransferableVersionException("No protocol stablished");

	it = mCreators.find(creatorType);
	if (it != mCreators.end()) {
		return it->second;
	} else throw TransferableVersionException("Creator not found");
}

