#pragma once
#include "FrameworkUtil.h"
#include "Ufo.h"
#include <string>

inline std::string Mode1() {
	fw.AddObject(new Aircraft, "obj1", Layer::L1);
	fw.AddObject(new Map, "map", Layer::Terrain);

	return __func__;
}