#pragma once
#include "CameraUtil.h"

class PickingUtil {
public:
	bool PickByCursor(LPARAM lparam, GameObject* Object, Mesh* MeshPtr);
	bool PickByCoord(float x, float y, GameObject* Object, Mesh* MeshPtr);
};