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

	mProtocolVersion = NO_PROTOCOL;

	for (it = mCreators.begin(); it != mCreators.end(); ++it) delete it->second;
	mCreators.clear();
	mSendingType();
	mProtocolVersion = NO_PROTOCOL;
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

void TransferableFactory::protocolVersion(const std::string& version) throw(TransferableVersionException&) {
	TransferableProfile::Creators* creators = NULL;
	TransferableProfile::CreatorIds* creatorIds = NULL;

	clear();
	if (!mProfile) throw TransferableVersionException("No Profile set for TransferableFactory");
	try {
		if (version != NO_PROTOCOL) {
			//TODO: Cargar del profile
			creators = mProfile->getCreators(version);
			creatorIds = mProfile->getCreatorIds(version);

			mProtocolVersion = version;	
		}
	} catch (TransferableVersionException& e) {
		clear();
		throw e;
	}
}

void TransferableFactory::setProfile(TransferableProfile* profile) throw() {
	mProfile = profile;
}

int TransferableFactory::type(const std::string& type) const throw(TransferableVersionException&) {
	std::map<std::string, int>::const_iterator it;

	if (!mProfile) throw TransferableVersionException("No Profile set for TransferableFactory");
	if (mProtocolVersion == NO_PROTOCOL) throw TransferableVersionException("No protocol stablished");

	it = mSendingType.find(type);
	if (it == mSendingType.end()) throw TransferableVersionException("Sending type not found");

	return it->second;
}

TransferableCreator* TransferableFactory::creator(int creatorType) const throw(TransferableVersionException&){
	std::map<int, TransferableCreator*>::const_iterator it;
	
	if (!mProfile) throw TransferableVersionException("No Profile set for TransferableFactory");
	if (mProtocolVersion == NO_PROTOCOL) throw TransferableVersionException("No protocol stablished");

	it = mCreators.find(creatorType);
	if (it != mCreators.end()) {
		return it->second;
	} else throw TransferableVersionException("Creator not found");
}

