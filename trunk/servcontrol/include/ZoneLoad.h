#ifndef _ZoneLoad_h
#define _ZoneLoad_h


class ZoneLoad {
	public:
	int zone;
	double load;
		
	ZoneLoad(int idZone, double nload): zone(idZone), load(nload) {}
	ZoneLoad(){}
	~ZoneLoad(){}
};

#endif
