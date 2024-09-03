#pragma once
#include "FrameworkUtil.h"
#include "Ufo.h"
#include <string>

inline std::string Mode1() {
	framework.AddObject(new Aircraft, "obj1", Layer::L1);
	framework.AddObject(new Aircraft, "obj2", Layer::L1);
	framework.AddObject(new Map, "map", Layer::Terrain);

	return __func__;
}