
// Include from STL
#include <string>


#include "FNV.h"


namespace YS_Util
{

	static uint32_t HashString(const std::string string) { return static_cast<uint32_t>(Fnv_1a(string.c_str())); }
}