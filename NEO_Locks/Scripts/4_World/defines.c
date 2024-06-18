//#define NEOLOCK_DEBUG // turning on NEOLOCK_DEBUG gets a LOT of output
#define NEOLOCK_INFO  // turning of NEOLOCK_INFO will make this mod completely silent
// NOTE: IF NEOFLAG_DEBUG is set, then NEOFLAG_INFO should be too or else you may miss error msgs

// ugh, in C/C++ I can #define these values, looks like enforce is just defined/undefined
// and need a const for an actual value (based on behavior not docs)
const int NEOLOCK_VERSION = 1;

//first version will not persist locker ID across restarts, we will see if this is 
//desirable later.
//const string NEOLOCK_FILENAME = "$profile:neolockss.json";