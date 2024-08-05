#include "CameraUtil.h"
#include "FrameworkUtil.h"

void Camera::Update(float FT) {
	if (RunningMode == CamMode::MODE1) {
		auto obj = fw.FindObject("obj1", LayerRange::Single, Layer::L1);
		if (obj) Track(obj->Position, obj, FT);
	}
}