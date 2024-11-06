#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// GameObject 클래스는 모든 객체들이 상속받는 부모 클래스이다.
// 모든 객체는 반드시 이 클래스로부터 상복받아야 Scene이 객체를 업데이트하고 렌더링한다.
// 일부 함수들은 별도의 파일로 분리 예정이니 코드에 변동이 있을 수 있다.


// 객체의 렌더링 상태를 초기화 한다. 모든 객체는 이 함수로 렌더링 과정이 시작된다.
// 기본 RANDER_TYPE_PERS로 타입이 지정되어있다.
// RENDER_TYPE_3D_STATIC또는 RENDER_TYPE_2D_STATIC일 경우 행렬 초기화를 실행하지 않는다. 
void GameObject::InitRenderState(int RenderTypeFlag) {
	// 출력 모드 지정
	RenderType = RenderTypeFlag;

	if (RenderTypeFlag != RENDER_TYPE_3D_STATIC && RenderTypeFlag != RENDER_TYPE_2D_STATIC) {
		TranslateMatrix = Mat4::Identity();
		RotateMatrix = Mat4::Identity();
		ScaleMatrix = Mat4::Identity();
	}

	// 매쉬 색상 초기화
	SetColor(XMFLOAT3(0.0, 0.0, 0.0));

	// 텍스처 상태 초기화
	FlipTexture(FLIP_TYPE_NONE);
	ObjectAlpha = 1.0f;

	// 출력 모드 변경
	switch(RenderTypeFlag) {
	case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		SetToImageMode();
		break;

	case RENDER_TYPE_3D: case RENDER_TYPE_3D_STATIC:
		SetToDefaultMode(); 
		break;
	}
}

// 객체 메쉬의 색상을 설정한다.
void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

// 객체 매쉬의 색상을 설정한다.
void GameObject::SetColorRGB(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

// 조명 사용 비활성화
void GameObject::DisableLight() {
	CBVUtil::Input(ObjectCmdList, BoolLightCBV, 0);
}

// 조명 사용 활성화
void GameObject::EnableLight() {
	CBVUtil::Input(ObjectCmdList, BoolLightCBV, 1);
}

// 텍스처를 반전시킨다. 모델에 따라 다르게 사용할 수 있다.
void GameObject::FlipTexture(int FlipType) {
	CBVUtil::Input(ObjectCmdList, FlipCBV, FlipType);
}

// 3D 렌더링
void GameObject::Render3D(Mesh* MeshPtr, Texture* TexturePtr, float AlphaValue, bool DepthTestFlag) {
	TexturePtr->Render(ObjectCmdList);
	ObjectShader->Render(ObjectCmdList, DepthTestFlag);
	CBVUtil::Input(ObjectCmdList, LightCBV, 0);

	ObjectAlpha = AlphaValue;

	SetCamera();
	UpdateShaderVariables();

	MeshPtr->Render(ObjectCmdList);
}

// 2D 렌더링
void GameObject::Render2D(Texture* TexturePtr, float AlphaValue) {
	TexturePtr->Render(ObjectCmdList);
	ImageShader->Render(ObjectCmdList, false);
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

// 오브젝트에 커맨드 리스트를 부여한다.
void GameObject::InputCommandList(ID3D12GraphicsCommandList* CmdList) {
	ObjectCmdList = CmdList;
}


//////////////////////////////////////// private

// 행렬과 쉐이더 및 색상 관련 값들을 쉐이더에 전달한다. RenderMesh함수를 실행하면 이 함수도 실행된다. 즉, 직접 사용할 일이 없다.
void GameObject::UpdateShaderVariables() {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(ObjectCmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(ObjectCmdList, &ModelColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(ObjectCmdList, &ObjectAlpha, ALPHA_INDEX, 1, 0);
}

// 이미지 출력 모드로 전환한다. 텍스처 수직 반전 후 조명 사용을 비활성화 한다.
void GameObject::SetToImageMode() {
	DisableLight();
	camera.SetToStaticMode();
}

// 기본 출력 모드로 전환한다. 텍스처 반전 해제 후 조명 사용을 활성화 한다.
void GameObject::SetToDefaultMode() {
	EnableLight();
	FlipTexture(FLIP_TYPE_NONE);
	camera.SetToDefaultMode();
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

	case RENDER_TYPE_3D_ORTHO: case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0f, 10.0f);
		break;
	}

	camera.SetViewportsAndScissorRects(ObjectCmdList);
	camera.UpdateShaderVariables(ObjectCmdList);
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