#pragma once
#include "Framework.h"
#include "Ufo.h"
#include <string>

inline std::string Mode1() {
	for (int i = 0; i < 10; ++i) {
		fw.AddObject(new Ufo(LayerFW::L1, "ufo"), LayerFW::L1);
		fw.AddObject(new Aircraft(LayerFW::L1, "aircraft"), LayerFW::L1);
	}
	
	fw.AddObject(new Ufo(LayerFW::L1, "ufo_target"), LayerFW::L1);
	fw.AddObject(new Aircraft(LayerFW::L1, "aircraft_target"), LayerFW::L1);

	return __func__;
}