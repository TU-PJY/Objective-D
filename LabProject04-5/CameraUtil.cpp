#include "CameraUtil.h"
#include "FrameworkUtil.h"

void Camera::Update(float FT) {
	if (RunningMode == CamMode::MODE1) {
		auto obj = framework.Find("obj1");
		if (obj) Track(obj->Position, obj, FT);
	}
}