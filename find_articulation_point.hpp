#ifndef _FIND_ARTICULATION_POINT_H_
#define _FIND_ARTICULATION_POINT_H_


#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_iterator.hpp>

void find_articulation_points(const std::unique_ptr<ScMemoryContext>&, ScAddr);
#endif
