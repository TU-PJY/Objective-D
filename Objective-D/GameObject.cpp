#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// GameObject 클래스는 모든 객체들이 상속받는 부모 클래스이다.
// 모든 객체는 반드시 이 클래스로부터 상속받아야 Scene이 객체를 업데이트하고 렌더링한다.

// 객체의 렌더링 상태를 초기화 한다. 모든 객체는 이 함수로 렌더링 과정이 시작된다.
// 기본 RANDER_TYPE_3D로 타입이 지정되어있다.
// RENDER_TYPE_3D_STATIC또는 RENDER_TYPE_2D_STATIC일 경우 행렬 초기화를 실행하지 않는다. 
void GameObject::InitRenderState(int RenderTypeFlag) {
	// 출력 모드 지정
	RenderType = RenderTypeFlag;

	if (RenderTypeFlag != RENDER_TYPE_3D_STATIC && RenderTypeFlag != RENDER_TYPE_2D_STATIC) {
		Transform::Identity(TranslateMatrix);
		Transform::Identity(RotateMatrix);
		Transform::Identity(ScaleMatrix);
	}

	if (RenderTypeFlag == RENDER_TYPE_2D || RenderTypeFlag == RENDER_TYPE_2D_STATIC)
		Transform::Identity(ImageAspectMatrix);

	// 매쉬 색상 초기화
	SetColor(XMFLOAT3(0.0, 0.0, 0.0));

	// 텍스처 상태 초기화
	FlipTexture(FLIP_TYPE_NONE);
	ObjectAlpha = 1.0f;

	// 출력 모드 변경
	switch(RenderTypeFlag) {
	case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		camera.SetToStaticMode();
		break;

	case RENDER_TYPE_3D: case RENDER_TYPE_3D_STATIC:
		SetLightUse(ENABLE_LIGHT);
		camera.SetToDefaultMode();
		break;
	}
}

// 객체 메쉬의 색상을 설정한다.
void GameObject::SetColor(XMFLOAT3 Color) {
	ObjectColor = Color;
}

// 객체 매쉬의 색상을 설정한다.
void GameObject::SetColorRGB(float R, float G, float B) {
	ObjectColor.x = R;
	ObjectColor.y = G;
	ObjectColor.z = B;
}

// 텍스처를 반전시킨다. 모델에 따라 다르게 사용할 수 있다.
void GameObject::FlipTexture(int FlipType) {
	// 이미지 출력 모드에서는 수직 반전이 기본 적용이기 때문에 반전이 다르게 동작한다.
	if (RenderType == RENDER_TYPE_2D || RenderType == RENDER_TYPE_2D_STATIC) {
		switch (FlipType) {
		case FLIP_TYPE_V:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_NONE); break;

		case FLIP_TYPE_H:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_HV);   break;

		case FLIP_TYPE_HV:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_H);    break;

		case FLIP_TYPE_NONE:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_V);    break;
		}
	}

	else
		CBVUtil::Input(ObjectCmdList, FlipCBV, FlipType);
}

// 조명 활성화 / 비활성화
void GameObject::SetLightUse(int Flag) {
	CBVUtil::Input(ObjectCmdList, BoolLightCBV, Flag);
}

// 3D 렌더링
void GameObject::Render3D(Mesh* MeshPtr, Texture* TexturePtr, float AlphaValue, bool DepthTestFlag) {
	TexturePtr->Render(ObjectCmdList);

	switch (DepthTestFlag) {
	case true:
		ObjectShader->RenderDefault(ObjectCmdList);    break;

	case false:
		ObjectShader->RenderDepthNone(ObjectCmdList);  break;
	}

	ObjectAlpha = AlphaValue;
	CBVUtil::Input(ObjectCmdList, LightCBV, 0);
	CBVUtil::Input(ObjectCmdList, FogCBV, 0);

	SetCamera();
	UpdateShaderVariables();

	MeshPtr->Render(ObjectCmdList);
}

// 2D 렌더링
void GameObject::Render2D(Texture* TexturePtr, float AlphaValue, bool EnableAspect) {
	if(EnableAspect)
		Transform::ImageAspect(ImageAspectMatrix, TexturePtr->Width, TexturePtr->Height);
	TexturePtr->Render(ObjectCmdList);
	ImageShader->RenderDepthNone(ObjectCmdList);
	ObjectAlpha = AlphaValue;

	SetCamera();
	UpdateShaderVariables();

	ImagePannel->Render(ObjectCmdList);
}

// 마우스 모션으로부터 회전값 업데이트 한다.
void GameObject::UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY) {
	RotationX += DeltaY;
	RotationY += DeltaX;
}

// 마우스 모션으로부터 회전값 업데이트 한다.
void GameObject::UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY) {
	Rotation.x += DeltaY;
	Rotation.y += DeltaX;
}

// 값에 종횡비를 곱한다. UI를 구현할때 주로 사용한다.
float GameObject::ASP(float Value) {
	return ASPECT * Value;
}

// 피킹 시 사용하는 함수이다. 프로그래머가 이 함수를 직접 사용할 일은 없다.
int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;

	XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
	GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
	nIntersected = MeshPtr->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);

	return(nIntersected);
}

//////////////////////////////////////// private

// 행렬과 쉐이더 및 색상 관련 값들을 쉐이더에 전달한다. Render함수를 실행하면 이 함수도 실행된다. 즉, 직접 사용할 일이 없다.
void GameObject::UpdateShaderVariables() {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RotateMatrix), XMLoadFloat4x4(&TranslateMatrix));
	ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), ResultMatrix);

	// 이미지 출력 모드일경우 종횡비를 적용한다.
	if (RenderType == RENDER_TYPE_2D || RenderType == RENDER_TYPE_2D_STATIC)
		ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ImageAspectMatrix), ResultMatrix);

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(ObjectCmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(ObjectCmdList, &ObjectColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(ObjectCmdList, &ObjectAlpha, GAME_OBJECT_INDEX, 1, 19);
}

// 렌더링 전 카메라를 설정한다.
void GameObject::SetCamera() {
	// 카메라 뷰 행렬을 설정한다.
	camera.SetViewMatrix();

	// 렌더 타입에 따라 다른 행렬을 초기화 한다.
	switch (RenderType) {
	case RENDER_TYPE_3D: case RENDER_TYPE_3D_STATIC:
		camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f); 
		break;

	case RENDER_TYPE_3D_ORTHO:
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0f, 10.0f);
		break;

	case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		camera.GenerateStaticMatrix();
	}

	camera.SetViewportsAndScissorRects();
	camera.UpdateShaderVariables();
}

// 피킹 시 사용하는 함수이다. 프로그래머가 이 함수를 직접 사용할 일은 없다.
void GameObject::GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, ResultMatrix * xmmtxView);
	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}