#define LOG_ASSERT(LVL, COND, MSG, RET) if(!(COND)){\
	LOG(LVL);
