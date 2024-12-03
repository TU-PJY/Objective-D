#pragma once
#include "CameraUtil.h"
#include "GameObject.h"

class PickingUtil {
public:
	bool PickByCursor(LPARAM lParam, GameObject* Object, Mesh* MeshPtr);
	bool PickByWinCoord(int X, int Y, GameObject* Object, Mesh* MeshPtr);
};