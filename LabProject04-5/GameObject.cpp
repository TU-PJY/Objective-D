#include "GameObject.h"

// GameObject Ŭ������ ��� ��ü���� ��ӹ޴� �θ� Ŭ�����̴�. 
// ��� ��ü�� �ݵ�� �� Ŭ�����κ��� �󺹹޾ƾ� �����ӿ�ũ�� ��ü�� ������Ʈ�ϰ� �������Ѵ�.
// �Ϻ� �Լ����� ������ ���Ϸ� �и� �����̴� �ڵ忡 ������ ���� �� �ִ�.

// ��ü�� ����� �ʱ�ȭ �Ѵ�. ��� ��ü�� ��ȯ �۾� �� �ݵ�� �� �Լ��� ù ��°�� �����ؾ� �Ѵ�.
void GameObject::InitMatrix() {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();
}

// ��ü �޽��� ������ �����Ѵ�. 
void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

// RGB���� �̿��Ͽ� ��ü �Ž��� ������ �����Ѵ�.
void GameObject::SetColor(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

//  ��ü�� ������ �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� right����, �ӵ����� �־��ָ� �ȴ�.
void GameObject::MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

//  ��ü�� ������ �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� look����, �ӵ����� �־��ָ� �ȴ�.
void GameObject::MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

// ��ü�� ���� �����δ�. ���� �ڽ��� ��ġ���� �ڽ��� up����, �ӵ����� �־��ָ� �ȴ�.
void GameObject::MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

////////////////////////////////

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
void GameObject::GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, ResultMatrix * xmmtxView);
	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;

	XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
	GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
	nIntersected = MeshPtr->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);

	return(nIntersected);
}

// �޽��� ������ �Ѵ�. ��ȯ �۾��� ���� �� �� �������� �����Ѵ�. Ŀ�ǵ� ����Ʈ, ���̴�, �׸��� �������� �Ž��� �ĸ����Ϳ� �־��ָ� �ȴ�.
void GameObject::RenderMesh(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, Mesh* MeshPtr) {
	if (ShaderPtr)
		ShaderPtr->Render(CmdList);

	UpdateShaderVariables(CmdList);

	if (MeshPtr)
		MeshPtr->Render(CmdList);
}

// ��İ� ���̴� �� ���� ���� ������ ���̴��� �����Ѵ�. RenderMesh�Լ��� �����ϸ� �� �Լ��� ����ȴ�. ��, ���� ����� ���� ����.
void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
}