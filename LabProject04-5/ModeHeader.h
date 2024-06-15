#pragma once
#include "Framework.h"
#include "Ufo.h"
#include <string>

inline std::string Mode1() {
	for (int i = 0; i < 10; ++i)
		fw.AddObject(new TestObject(Layer::L2, "test_obj"), Layer::L2);

	fw.AddObject(new Aircraft(Layer::L1, "obj1"), Layer::L1);


	return __func__;
}