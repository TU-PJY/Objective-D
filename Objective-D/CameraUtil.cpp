#include "CameraUtil.h"
#include "FrameworkUtil.h"
#include "RootConstants.h"

// Config.h 에서 작성한 모드에 따라 카메라가 다르게 동작하도록 작성할 수 있다.
// 예) 카메라 추적 대상 변경, 카메라 시점 변경 등
void Camera::Update(float FT) {
	switch (Mode) {
	case CamMode::MODE1:
		break;
	}
}

Camera::Camera() {
	ViewMatrix = Mat4::Identity();
	ProjectionMatrix = Mat4::Identity();

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Pitch = 0.0f;
	Roll = 0.0f;
	Yaw = 0.0f;

	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	LookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);

	MovingDelay = 0.0f;
}

// 정적 출력 모드로 전환한다.
void Camera::SetToStaticMode() {
	StaticMode = true;
}

// 일반 출력 모드로 전환한다.
void Camera::SetToDefaultMode() {
	StaticMode = false;
}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMFLOAT4X4 xmf4x4View;
	XMFLOAT4X4 xmf4x4Projection;

	std::cout << StaticMode << std::endl;

	// 스테틱 모드 실행 시 스테틱 행렬을 쉐이더로 전달한다.
	if (StaticMode) {
		XMFLOAT3 StaticPosition{ 0.0, 0.0, 0.0 };
		XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&StaticViewMatrix)));
		XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&StaticProjectionMatrix)));
		CmdList->SetGraphicsRoot32BitConstants(CAMERA_INDEX, 3, &StaticPosition, 32);
	}

	else {
		XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&ViewMatrix)));
		XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&ProjectionMatrix)));
		CmdList->SetGraphicsRoot32BitConstants(CAMERA_INDEX, 3, &Position, 32);
	}

	CmdList->SetGraphicsRoot32BitConstants(CAMERA_INDEX, 16, &xmf4x4View, 0);
	CmdList->SetGraphicsRoot32BitConstants(CAMERA_INDEX, 16, &xmf4x4Projection, 16);
}

// 카메라 행렬을 초기화 한다.
void Camera::InitMatrix() {
	Look = Vec3::Normalize(Look);
	Right = Vec3::CrossProduct(Up, Look, true);
	Up = Vec3::CrossProduct(Look, Right, true);

	ViewMatrix._11 = Right.x; ViewMatrix._12 = Up.x; ViewMatrix._13 = Look.x;
	ViewMatrix._21 = Right.y; ViewMatrix._22 = Up.y; ViewMatrix._23 = Look.y;
	ViewMatrix._31 = Right.z; ViewMatrix._32 = Up.z; ViewMatrix._33 = Look.z;

	ViewMatrix._41 = -Vec3::DotProduct(Position, Right);
	ViewMatrix._42 = -Vec3::DotProduct(Position, Up);
	ViewMatrix._43 = -Vec3::DotProduct(Position, Look);

	InverseView._11 = Right.x; InverseView._12 = Right.y; InverseView._13 = Right.z;
	InverseView._21 = Up.x; InverseView._22 = Up.y; InverseView._23 = Up.z;
	InverseView._31 = Look.x; InverseView._32 = Look.y; InverseView._33 = Look.z;
	InverseView._41 = Position.x; InverseView._42 = Position.y; InverseView._43 = Position.z;

	FrustumView.Transform(FrustumWorld, XMLoadFloat4x4(&InverseView));
}

// 원근 투영 행렬을 초기화한다. 윈도우 사이즈 변경 시 이 함수가 실행된다.
void Camera::GeneratePerspectiveMatrix(float NearPlane, float FarPlane, float AspRatio, float Fov) {
	ProjectionMatrix = Mat4::Identity();
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(Fov), AspRatio, NearPlane, FarPlane);
	XMStoreFloat4x4(&ProjectionMatrix, Projection);

#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	BoundingFrustum::CreateFromMatrix(FrustumView, Projection);
#endif
}

// 직각 투영 행렬을 초기화 한다
void Camera::GenerateOrthoMatrix(float Width, float Height, float AspRatio, float Near, float Far) {
	ProjectionMatrix = Mat4::Identity();
	XMMATRIX Projection = XMMatrixOrthographicLH(Width * AspRatio, Height, Near, Far);
	XMStoreFloat4x4(&ProjectionMatrix, Projection);

#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	BoundingFrustum::CreateFromMatrix(FrustumView, Projection);
#endif
}

// 정적 출력을 위한 스테틱 행렬을 생성한다. UI, 이미지 등의 출력을 목적으로 하는 행렬이므로 
// 프로그램 실행 시 최초 1회만 실행한다.
void Camera::InitStaticMatrix() {
	StaticViewMatrix = Mat4::Identity();

	// 직각투영이 디폴트이다.
	StaticProjectionMatrix = Mat4::Identity();
	XMMATRIX Projection = XMMatrixOrthographicLH(1.0 * ASPECT_RATIO, 1.0, 0.0, 10.0);
	XMStoreFloat4x4(&StaticProjectionMatrix, Projection);
}

// 윈도우 사이즈 변경 시 실행된다.
void Camera::UpdateStaticMatrix() {
	StaticProjectionMatrix = Mat4::Identity();
	XMMATRIX Projection = XMMatrixOrthographicLH(1.0 * ASPECT_RATIO, 1.0, 0.0, 10.0);
	XMStoreFloat4x4(&StaticProjectionMatrix, Projection);
}

// 뷰포트를 설정한다. 한 번 설정한 이후에는 건들 필요 없다.
void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float zMin, float zMax) {
	Viewport.TopLeftX = float(xTopLeft);
	Viewport.TopLeftY = float(yTopLeft);
	Viewport.Width = float(nWidth);
	Viewport.Height = float(nHeight);
	Viewport.MinDepth = zMin;
	Viewport.MaxDepth = zMax;
}

// 시저 렉트를 설정한다. 한 번 설정한 이후에는 건들 필요 없다.
void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom) {
	ScissorRect.left = xLeft;
	ScissorRect.top = yTop;
	ScissorRect.right = xRight;
	ScissorRect.bottom = yBottom;
}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList* CmdList) {
	CmdList->RSSetViewports(1, &Viewport);
	CmdList->RSSetScissorRects(1, &ScissorRect);
}


void Camera::GenerateViewMatrix() {
	ViewMatrix = Mat4::LookAtLH(Position, LookAt, Up);
}

void Camera::GenerateViewMatrix(XMFLOAT3 PositionValue, XMFLOAT3 LookAtVector, XMFLOAT3 UpVector) {
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&PositionValue), XMLoadFloat3(&LookAtVector), XMLoadFloat3(&UpVector));
	XMStoreFloat4x4(&ViewMatrix, viewMatrix);
}


// 카메라 모드를 변경한다. Config.h에 작성했던 모드 열거형을 파라미터에 넣으면 된다.
void Camera::SetCameraMode(CamMode ModeValue) {
	Mode = ModeValue;
}

// 위치 이동, 시점 추적 위치 설정 등 회전각도, 위치, 벡터 관련 함수들이다.
void Camera::SetPosition(XMFLOAT3 PositionValue) { Position = PositionValue; }
XMFLOAT3& Camera::GetPosition() { return(Position); }
void Camera::SetLookAtPosition(XMFLOAT3 LookAtValue) { LookAt = LookAtValue; }
XMFLOAT3& Camera::GetLookAtPosition() { return(LookAt); }

XMFLOAT3& Camera::GetRightVector() { return(Right); }
XMFLOAT3& Camera::GetUpVector() { return(Up); }
XMFLOAT3& Camera::GetLookVector() { return(Look); }

float& Camera::GetPitch() { return(Pitch); }
float& Camera::GetRoll() { return(Roll); }
float& Camera::GetYaw() { return(Yaw); }

void Camera::SetOffset(XMFLOAT3 Value) { Offset = Value; }
XMFLOAT3& Camera::GetOffset() { return(Offset); }

void Camera::SetTimeLag(float DelayValue) { MovingDelay = DelayValue; }
float Camera::GetTimeLag() { return(MovingDelay); }

XMFLOAT4X4 Camera::GetViewMatrix() { return(ViewMatrix); }
XMFLOAT4X4 Camera::GetProjectionMatrix() { return(ProjectionMatrix); }
D3D12_VIEWPORT Camera::GetViewport() { return(Viewport); }
D3D12_RECT Camera::GetScissorRect() { return(ScissorRect); }

// 카메라의 위치를 변경한다.
void Camera::SetPosition(float X, float Y, float Z) {
	Position.x = X;
	Position.y = Y;
	Position.z = Z;
}

// 현재 시점에서 앞으로 움직인다.
void Camera::MoveForward(float MoveDistance) {
	XMFLOAT3 NormlaizedLook = Vec3::Normalize(Look);

	Position.x += NormlaizedLook.x * MoveDistance;
	Position.y += NormlaizedLook.y * MoveDistance;
	Position.z += NormlaizedLook.z * MoveDistance;
}

// 카메라 회전, 벡터 초기화 후 새로운 벡터를 지정한다.
void Camera::Rotate(float X, float Y, float Z) {
	Look = XMFLOAT3(0.0, 0.0, 1.0);
	Right = XMFLOAT3(1.0, 0.0, 0.0);
	Up = XMFLOAT3(0.0, 1.0, 0.0);

	Pitch = X;
	Yaw = Y;
	Roll = Z;

	// 회전 행렬 생성
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	XMStoreFloat3(&Look, XMVector3TransformNormal(XMLoadFloat3(&Look), RotationMatrix));
	XMStoreFloat3(&Right, XMVector3TransformNormal(XMLoadFloat3(&Right), RotationMatrix));
	XMStoreFloat3(&Up, XMVector3TransformNormal(XMLoadFloat3(&Up), RotationMatrix));
}

// 파라미터로 입력받은 위치, 업벡터, 라이트벡터, 룩벡터를 추적한다.
void Camera::Track(XMFLOAT3& ObjectPosition, XMFLOAT3& UpVec, XMFLOAT3& RightVec, XMFLOAT3& LookVec, float fTimeElapsed) {
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();

	XMFLOAT3 UpVector = UpVec;
	XMFLOAT3 RightVector = RightVec;
	XMFLOAT3 LookVector = LookVec;

	RotateMatrix._21 = UpVector.x;
	RotateMatrix._22 = UpVector.y;
	RotateMatrix._23 = UpVector.z;

	RotateMatrix._11 = RightVector.x;
	RotateMatrix._12 = RightVector.y;
	RotateMatrix._13 = RightVector.z;

	RotateMatrix._31 = LookVector.x;
	RotateMatrix._32 = LookVector.y;
	RotateMatrix._33 = LookVector.z;

	XMFLOAT3 Direction = Vec3::Subtract(Vec3::Add(ObjectPosition, Vec3::TransformCoord(Offset, RotateMatrix)), Position);

	float Length = Vec3::Length(Direction);
	Direction = Vec3::Normalize(Direction);

	float TimeLagScale = (MovingDelay) ? fTimeElapsed * (1.0f / MovingDelay) : 1.0f;
	float MoveDistance = Length * TimeLagScale;

	if (MoveDistance > Length)
		MoveDistance = Length;

	if (Length < 0.01f)
		MoveDistance = Length;

	Position = Vec3::Add(Position, Direction, MoveDistance);
	SetLookAt(ObjectPosition, UpVec);
}

// 카메라가 바라보는 방향을 설정한다. Track에서 실행되므로 보통의 경우 직접 쓸 일은 없다.
void Camera::SetLookAt(XMFLOAT3& ObjectPosition, XMFLOAT3& UpVec) {
	XMFLOAT4X4 mtxLookAt = Mat4::LookAtLH(Position, ObjectPosition, UpVec);
	Right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	Up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	Look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}


// 프러스텀 관련 함수들
void Camera::CalculateFrustumPlanes() {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	FrustumView.Transform(FrustumWorld, XMMatrixInverse(NULL, XMLoadFloat4x4(&ViewMatrix)));
#else
	XMFLOAT4X4 CamViewProjection = Mat4::Multiply(ViewMatrix, ProjectionMatrix);

	FrustumPlane[0].x = -(CamViewProjection._14 + CamViewProjection._11);
	FrustumPlane[0].y = -(CamViewProjection._24 + CamViewProjection._21);
	FrustumPlane[0].z = -(CamViewProjection._34 + CamViewProjection._31);
	FrustumPlane[0].w = -(CamViewProjection._44 + CamViewProjection._41);

	FrustumPlane[1].x = -(CamViewProjection._14 - CamViewProjection._11);
	FrustumPlane[1].y = -(CamViewProjection._24 - CamViewProjection._21);
	FrustumPlane[1].z = -(CamViewProjection._34 - CamViewProjection._31);
	FrustumPlane[1].w = -(CamViewProjection._44 - CamViewProjection._41);

	FrustumPlane[2].x = -(CamViewProjection._14 - CamViewProjection._12);
	FrustumPlane[2].y = -(CamViewProjection._24 - CamViewProjection._22);
	FrustumPlane[2].z = -(CamViewProjection._34 - CamViewProjection._32);
	FrustumPlane[2].w = -(CamViewProjection._44 - CamViewProjection._42);

	FrustumPlane[3].x = -(CamViewProjection._14 + CamViewProjection._12);
	FrustumPlane[3].y = -(CamViewProjection._24 + CamViewProjection._22);
	FrustumPlane[3].z = -(CamViewProjection._34 + CamViewProjection._32);
	FrustumPlane[3].w = -(CamViewProjection._44 + CamViewProjection._42);

	FrustumPlane[4].x = -(CamViewProjection._13);
	FrustumPlane[4].y = -(CamViewProjection._23);
	FrustumPlane[4].z = -(CamViewProjection._33);
	FrustumPlane[4].w = -(CamViewProjection._43);

	FrustumPlane[5].x = -(CamViewProjection._14 - CamViewProjection._13);
	FrustumPlane[5].y = -(CamViewProjection._24 - CamViewProjection._23);
	FrustumPlane[5].z = -(CamViewProjection._34 - CamViewProjection._33);
	FrustumPlane[5].w = -(CamViewProjection._44 - CamViewProjection._43);

	for (int i = 0; i < 6; i++)
		FrustumPlane[i] = Plane::Normalize(FrustumPlane[i]);
#endif
}

bool Camera::IsInFrustum(BoundingBox& BoundingBox) {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	return(FrustumWorld.Intersects(BoundingBox));
#else
	XMFLOAT3 NearPoint, Normal;
	XMFLOAT3 Min = Vec3::Subtract(BoundingBox.Center, BoundingBox.Extents);
	XMFLOAT3 Max = Vec3::Add(BoundingBox.Center, BoundingBox.Extents);
	for (int i = 0; i < 6; i++) {
		Normal = XMFLOAT3(FrustumPlane[i].x, FrustumPlane[i].y, FrustumPlane[i].z);

		if (Normal.x >= 0.0f) {
			if (Normal.y >= 0.0f) {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Min.x, Min.y, Min.z);
				else
					NearPoint = XMFLOAT3(Min.x, Min.y, Max.z);
			}

			else {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Min.x, Max.y, Min.z);
				else
					NearPoint = XMFLOAT3(Min.x, Max.y, Max.z);
			}
		}

		else {
			if (Normal.y >= 0.0f) {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Max.x, Min.y, Min.z);
				else
					NearPoint = XMFLOAT3(Max.x, Min.y, Max.z);
			}

			else {
				if (Normal.z >= 0.0f)
					NearPoint = XMFLOAT3(Max.x, Max.y, Min.z);
				else
					NearPoint = XMFLOAT3(Max.x, Max.y, Max.z);
			}
		}

		if ((Vec3::DotProduct(Normal, NearPoint) + FrustumPlane[i].w) > 0.0f)
			return(false);
	}

	return(true);
#endif
}

bool Camera::IsInFrustum(BoundingOrientedBox& BoundingBox) {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	return(FrustumWorld.Intersects(BoundingBox));
#else
#endif
}