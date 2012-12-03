#include <Datagram.h>

class GetServerLoad : public Datagram<GetServerLoad> {
public:
	GetServerLoad(): Datagram<GetServerLoad>("GetServerLoad")  {};
};
