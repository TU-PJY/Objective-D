#pragma once
#include "Framework.h"
#include "Ufo.h"
#include <string>

inline std::string Mode1() {
	fw.AddObject(new Aircraft(Layer::L1, "obj1"), Layer::L1);
	fw.AddObject(new TestObject(Layer::L2, "obj2", XMFLOAT3(-20.0, 0.0, 20.0), XMFLOAT3(1.0, 0.0, 0.0)), Layer::L2);	
	fw.AddObject(new TestObject(Layer::L2, "obj3", XMFLOAT3(-10.0, 0.0, 20.0), XMFLOAT3(0.0, 1.0, 0.0)), Layer::L2);
	fw.AddObject(new TestObject(Layer::L2, "obj4", XMFLOAT3(0.0, 0.0, 20.0), XMFLOAT3(0.0, 0.0, 1.0)), Layer::L2);
	fw.AddObject(new TestObject(Layer::L2, "obj5", XMFLOAT3(10.0, 0.0, 20.0), XMFLOAT3(1.0, 1.0, 0.0)), Layer::L2);
	fw.AddObject(new TestObject(Layer::L2, "obj6", XMFLOAT3(20.0, 0.0, 20.0), XMFLOAT3(0.0, 1.0, 1.0)), Layer::L2);

	return __func__;
}