#include <Transferable.h>

#include <iostream>

const std::string TransferableFactory::NO_PROTOCOL = "none";

TransferableFactory::TransferableFactory() throw()	: mProtocolVersion(NO_PROTOCOL)
							, mCreators() { }

TransferableFactory::~TransferableFactory() throw() {
	clear();
	setProfile(NULL);
}

void TransferableFactory::clear() throw() {
	mProtocolVersion = NO_PROTOCOL;

	mCreators.clear();
	mSendingType.clear();
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

const std::string& TransferableFactory::protocol() const throw() {
	return mProtocolVersion;
}

void TransferableFactory::setProtocol(const std::string& version) throw(TransferableVersionException&) {
	const TransferableProfile::Creators* creators = NULL;
	const TransferableProfile::CreatorIds* creatorIds = NULL;

	std::cout << "SetProtocol in" << std::endl;
	
	clear();
	if (!mProfile) throw TransferableVersionException("No Profile set for TransferableFactory");
	try {
		if (version != NO_PROTOCOL) {
			
			creators = &mProfile->getCreators(version);
			creatorIds = &mProfile->getCreatorIds(version);

			for (int i = 0; i < creators->size(); ++i) {
				mCreators.insert((*creators)[i]);
				std::cout << "mCreators: " << (*creators)[i].first << " , " << (void*)(*creators)[i].second << std:: endl;
				fflush(stdout);
			}
			for (int i = 0; i < creatorIds->size(); ++i) {
				mSendingType.insert((*creatorIds)[i]);
				std::cout << "mCreatorIds: " << (*creatorIds)[i].first << " , " << (*creatorIds)[i].second << std:: endl;
				fflush(stdout);
			}
			mProtocolVersion = version;	
		} else {
			std::cout << "version == NO_PROTOCOL!" << std::endl;
			fflush(stdout);
		}
	} catch (TransferableVersionException& e) {
		
		std::cout << "SetProtocol exception" << std::endl;
		clear();
		setProtocol(NULL);
		throw e;
	}
	
	std::cout << "SetProtocol out" << std::endl;
}

void TransferableFactory::setProfile(TransferableProfile* profile) throw() {
	if (mProfile) delete mProfile;
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

