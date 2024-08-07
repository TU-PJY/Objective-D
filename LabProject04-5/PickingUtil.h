#pragma once
#include "CameraUtil.h"

class PickingUtil {
public:
	bool PickByCursor(LPARAM lParam, int ObjectIntersected);
	bool PickByCoord(float X, float Y, int ObjectIntersected);
};

extern PickingUtil pickingUtil;