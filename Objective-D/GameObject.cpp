#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// GameObject 클래스는 모든 객체들이 상속받는 부모 클래스이다.
// 모든 객체는 반드시 이 클래스로부터 상복받아야 프레임워크가 객체를 업데이트하고 렌더링한다.
// 일부 함수들은 별도의 파일로 분리 예정이니 코드에 변동이 있을 수 있다.

// 객체의 행렬을 초기화 한다. 모든 객체는 변환 작업 전 반드시 이 함수를 첫 번째로 실행해야 한다.
void GameObject::InitMatrix(ID3D12GraphicsCommandList* CmdList, int RenderTypeFlag) {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();

	// 출력 모드 지정
	RenderType = RenderTypeFlag;

	// 텍스처 상태 초기화
	ObjectAlpha = 1.0f;
	FlipTexture(CmdList, FLIP_TYPE_NONE);

	// 매쉬 색상 초기화
	SetColor(0.0, 0.0, 0.0);

	// 이미지 타입 렌더링일 경우 이미지 모드로 변경
	if (RenderTypeFlag == RENDER_TYPE_IMAGE)
		SetToImageMode(CmdList);

	else // 나머지 타입일 경우 기본 모드로 변경
		SetToDefaultMode(CmdList);
}

// 객체 메쉬의 색상을 설정한다.
void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

// 객체 매쉬의 색상을 설정한다.
void GameObject::SetColor(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

//  객체를 앞으로 움직인다. 현재 자신의 위치값과 자신의 look벡터, 속도값을 넣어주면 된다.
void GameObject::MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

//  객체를 옆으로 움직인다. 현재 자신의 위치값과 자신의 right벡터, 속도값을 넣어주면 된다.
void GameObject::MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

// 객체를 위로 움직인다. 현재 자신의 위치값과 자신의 up벡터, 속도값을 넣어주면 된다.
void GameObject::MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

// 텍스처를 반전시킨다. 모델에 따라 다르게 사용할 수 있다.
void GameObject::FlipTexture(ID3D12GraphicsCommandList* CmdList, int FlipType) {
	CBVUtil::Input(CmdList, FlipCBV, FlipType);
}

// 조명 사용 비활성화
void GameObject::DisableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, BoolLightCBV, 0);
}

// 조명 사용 활성화
void GameObject::EnableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, BoolLightCBV, 1);
}

// 쉐이더에 조명 데이터 전송
void GameObject::InputLightInfo(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, LightCBV, 0);
}

// 값에 종횡비를 곱한다. UI를 구현할때 주로 사용한다.
float GameObject::ASP(float Value) {
	return ASPECT * Value;
}

void GameObject::UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY) {
	RotationX += DeltaY;
	RotationY += DeltaX;
}

void GameObject::UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY) {
	Rotation.x += DeltaY;
	Rotation.y += DeltaX;
}

////////////////////////////////
// 
// 텍스처를 바인딩한다. 반드시 매쉬를 렌더링하기 전에 사용해야 한다. 커맨드 리스트와 매핑할 텍스처를 파라미터로 전달하면 된다.
// 좌우 또는 상하 또는 좌우상하 반전이 가능하다.
void GameObject::BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr) {
	if (TexturePtr)
		TexturePtr->Render(CmdList);
}

// 쉐이더를 적용한다. 반드시 매쉬를 렌더링하기 전에 사용해야 한다. 커맨드 리스트와 적용할 쉐이더를 파라미터로 전달하면 된다.
void GameObject::UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest) {
	if (ShaderPtr)
		ShaderPtr->Render(CmdList, DepthTest);
}

// 메쉬를 랜더링 한다. 변환 작업이 끝난 후 맨 마지막에 실행한다.
void GameObject::RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr, Texture* TexturePtr, Shader* ShaderPtr, float Alpha, bool DepthTestFlag) {
	// 텍스처 바인딩
	BindTexture(CmdList, TexturePtr);

	// 이미지 랜더 타입이 아니라면 깊이 검사 플래그를 따르도록 한다.
	if(RenderType != RENDER_TYPE_IMAGE)
		UseShader(CmdList, ShaderPtr, DepthTestFlag);
	else 
		UseShader(CmdList, ShaderPtr, false);

	// 카메라 행렬을 초기화 한다
	camera.InitMatrix();

	if (RenderType == RENDER_TYPE_PERS)
		camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f);

	else if (RenderType == RENDER_TYPE_ORTHO || RenderType == RENDER_TYPE_IMAGE)
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0f, 10.0f);

	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);

	ObjectAlpha = Alpha;

	// 조명 정보를 렌더징 전에 쉐이더로 전달한다.
	// 이미지 모드의 경우 조명이 비활성화 된다.
	InputLightInfo(CmdList);
	UpdateShaderVariables(CmdList);
	if (MeshPtr)
		MeshPtr->Render(CmdList);
}

// 피킹 시 사용하는 함수이다. 프로그래머가 이 함수를 직접 사용할 일은 없다.
int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;

	XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
	GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
	nIntersected = MeshPtr->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);

	return(nIntersected);
}

// 행렬과 쉐이더 및 색상 관련 값들을 쉐이더에 전달한다. RenderMesh함수를 실행하면 이 함수도 실행된다. 즉, 직접 사용할 일이 없다.
void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &ModelColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &ObjectAlpha, ALPHA_INDEX, 1, 0);
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

// 이미지 출력 모드로 전환한다. 텍스처 수직 반전 후 조명 사용을 비활성화 한다.
void GameObject::SetToImageMode(ID3D12GraphicsCommandList* CmdList) {
	FlipTexture(CmdList, FLIP_TYPE_V);
	DisableLight(CmdList);
	camera.SetToStaticMode();
}

// 기본 출력 모드로 전환한다. 텍스처 반전 해제 후 조명 사용을 활성화 한다.
void GameObject::SetToDefaultMode(ID3D12GraphicsCommandList* CmdList) {
	FlipTexture(CmdList, FLIP_TYPE_NONE);
	EnableLight(CmdList);
	camera.SetToDefaultMode();
}