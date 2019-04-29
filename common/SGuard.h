#ifndef _SGUARD_H
#define _SGUARD_H

struct GuardPlatformData;

class SGuard {
private:
	GuardPlatformData* p_data;
public:
	SGuard();
	~SGuard();
	void Lock();
	void Unlock();
};

#endif
