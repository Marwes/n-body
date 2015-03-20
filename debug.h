#pragma once

#ifdef DEBUG
#define DPRINT(x) do { std::cerr << x << std::endl; } while(0)
#else
#define DPRINT(x)
#endif
