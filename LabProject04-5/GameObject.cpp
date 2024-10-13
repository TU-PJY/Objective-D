#include "GameObject.h"

// GameObject 클래스는 모든 객체들이 상속받는 부모 클래스이다. 
// 모든 객체는 반드시 이 클래스로부터 상복받아야 프레임워크가 객체를 업데이트하고 렌더링한다.
// 일부 함수들은 별도의 파일로 분리 예정이니 코드에 변동이 있을 수 있다.

// 객체의 행렬을 초기화 한다. 모든 객체는 변환 작업 전 반드시 이 함수를 첫 번째로 실행해야 한다.
void GameObject::InitMatrix() {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();
}

// 객체 메쉬의 색상을 설정한다. 
void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

// RGB값을 이용하여 객체 매쉬의 색상을 설정한다.
void GameObject::SetColor(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

//  객체를 옆으로 움직인다. 현재 자신의 위치값과 자신의 right벡터, 속도값을 넣어주면 된다.
void GameObject::MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

//  객체를 앞으로 움직인다. 현재 자신의 위치값과 자신의 look벡터, 속도값을 넣어주면 된다.
void GameObject::MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

// 객체를 위로 움직인다. 현재 자신의 위치값과 자신의 up벡터, 속도값을 넣어주면 된다.
void GameObject::MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

////////////////////////////////

// 텍스처를 바인딩한다. 반드시 매쉬를 렌더링하기 전에 사용해야 한다. 커맨드 리스트와 매핑할 텍스처를 파라미터로 전달하면 된다.
void GameObject::BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr) {
	if (TexturePtr)
		TexturePtr->Render(CmdList);
}

// 쉐이더를 적용한다. 반드시 매쉬를 렌더링하기 전에 사용해야 한다. 커맨드 리스트와 적용할 쉐이더를 파라미터로 전달하면 된다.
// 필요에 따라 여러 종류의 쉐이더에 대해 여러번 사용할 수 있다.
void GameObject::UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr) {
	if (ShaderPtr)
		ShaderPtr->Render(CmdList);
}

// 메쉬를 랜더링 한다. 변환 작업이 끝난 후 맨 마지막에 실행한다. 커맨드 리스트, 쉐이더, 그리고 렌더링할 매쉬를 파리미터에 넣어주면 된다.
void GameObject::RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr) {
	UpdateShaderVariables(CmdList);
	if (MeshPtr)
		MeshPtr->Render(CmdList);
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
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
}