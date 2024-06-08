#define NEOFLAG_DEBUG 1 // turning on NEOFLAG_DEBUG gets a LOT of output
#define NEOFLAG_INFO 1  // turning of NEOFLAG_INFO will make this mod completely silent
// NOTE: IF NEOFLAG_DEBUG is set, then NEOFLAG_INFO should be too

// ugh, in C/C++ I can #define these values, looks like enforce is just defined/undefined
// and need a const for an actual value (based on behavior not docs)
const int NEOFLAGINFO_VERSION = 1;

const string NEOFLAG_FILENAME = "$profile:neoflags.json";