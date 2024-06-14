#pragma once
#include "Framework.h"
#include "Ufo.h"
#include <string>

inline std::string Mode1() {
	fw.AddObject(new Aircraft(LayerFW::L1, "obj1"), LayerFW::L1);
	fw.AddObject(new Aircraft(LayerFW::L1, "obj2"), LayerFW::L1);
	fw.AddObject(new Aircraft(LayerFW::L1, "obj3"), LayerFW::L1);
	fw.AddObject(new Aircraft(LayerFW::L1, "obj3"), LayerFW::L1);
	fw.AddObject(new Aircraft(LayerFW::L1, "obj5"), LayerFW::L1);

	return __func__;
}