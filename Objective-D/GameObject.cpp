#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"

// GameObject Ŭ������ ��� ��ü���� ��ӹ޴� �θ� Ŭ�����̴�.
// ��� ��ü�� �ݵ�� �� Ŭ�����κ��� �󺹹޾ƾ� �����ӿ�ũ�� ��ü�� ������Ʈ�ϰ� �������Ѵ�.
// �Ϻ� �Լ����� ������ ���Ϸ� �и� �����̴� �ڵ忡 ������ ���� �� �ִ�.

// ��ü�� ����� �ʱ�ȭ �Ѵ�. ��� ��ü�� ��ȯ �۾� �� �ݵ�� �� �Լ��� ù ��°�� �����ؾ� �Ѵ�.
void GameObject::InitMatrix(ID3D12GraphicsCommandList* CmdList, RenderType Type) {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();

	// ī�޶�� �Ϲ� ��� ���� ����Ѵٴ� ���� ������ �Ѵ�.
	camera.SetToDefaultMode();

	// ��� ��� ����
	renderType = Type;

	// ������ ����ϴ� ������ ����
	EnableLight(CmdList);
	SetAlpha(CmdList, 1.0);
	FlipTexture(CmdList, false, false);
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

// �ؽ�ó�� ���ε��Ѵ�. �ݵ�� �Ž��� �������ϱ� ���� ����ؾ� �Ѵ�. Ŀ�ǵ� ����Ʈ�� ������ �ؽ�ó�� �Ķ���ͷ� �����ϸ� �ȴ�.
// �¿� �Ǵ� ���� �Ǵ� �¿���� ������ �����ϴ�.
void GameObject::BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr) {
	if (TexturePtr)
		TexturePtr->Render(CmdList);
}

// ���̴��� �����Ѵ�. �ݵ�� �Ž��� �������ϱ� ���� ����ؾ� �Ѵ�. Ŀ�ǵ� ����Ʈ�� ������ ���̴��� �Ķ���ͷ� �����ϸ� �ȴ�.
void GameObject::UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest) {
	if (ShaderPtr)
		ShaderPtr->Render(CmdList, DepthTest);
}

// �޽��� ������ �Ѵ�. ��ȯ �۾��� ���� �� �� �������� �����Ѵ�. Ŀ�ǵ� ����Ʈ, ���̴�, �׸��� �������� �Ž��� �ĸ����Ϳ� �־��ָ� �ȴ�.
void GameObject::RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr) {
	// ���� ������ ����¡ ���� ���̴��� �����Ѵ�.
	// �̹��� ����� ��� ������ ��Ȱ��ȭ �ȴ�.
	// 
	// ī�޶� ����� �ʱ�ȭ �Ѵ�
	camera.RegenerateViewMatrix();

	if (renderType == RenderType::Pers)
		camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT_RATIO, 45.0f);
	else if (renderType == RenderType::Ortho)
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT_RATIO, 0.0f, 10.0f);

	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);


	SendLightInfo(CmdList);
	UpdateShaderVariables(CmdList);
	if (MeshPtr)
		MeshPtr->Render(CmdList);
}

// �ؽ�ó�� ������Ų��. �𵨿� ���� �ٸ��� ����� �� �ִ�.
void GameObject::FlipTexture(ID3D12GraphicsCommandList* CmdList, bool H_Flip, bool V_Flip) {
	int Index{};

	if (!H_Flip && !V_Flip)     Index = 0;
	else if (H_Flip && !V_Flip) Index = 1;
	else if (!H_Flip && V_Flip) Index = 2;
	else if (H_Flip && V_Flip) Index = 3;

	CBVUtil::InputCBV(CmdList, FlipCBV, Index);
}

// �̹��� ��� ���� ��ȯ�Ѵ�. ���� ���� �� ���� ����� ��Ȱ��ȭ �Ѵ�.
void GameObject::SetToImageMode(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, FlipCBV, 1);
	camera.SetToStaticMode();
	DisableLight(CmdList);
}

// �ؽ�ó ������ �����Ѵ�.
void GameObject::SetAlpha(ID3D12GraphicsCommandList* CmdList, float AlphaValue) {
	AlphaInfo Alphainfo{ AlphaValue };
	CmdList->SetGraphicsRoot32BitConstants(ALPHA_INDEX, 1, &Alphainfo, 0);
}

// ���� ��� ��Ȱ��ȭ
void GameObject::DisableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, BoolLightCBV, 0);
}

// ���� ��� Ȱ��ȭ
void GameObject::EnableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, BoolLightCBV, 1);
}

// ���̴��� ���� ������ ����
void GameObject::SendLightInfo(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, LightCBV, 0);
}

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

// ��İ� ���̴� �� ���� ���� ������ ���̴��� �����Ѵ�. RenderMesh�Լ��� �����ϸ� �� �Լ��� ����ȴ�. ��, ���� ����� ���� ����.
void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(GAME_OBJECT_INDEX, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(GAME_OBJECT_INDEX, 3, &ModelColor, 16);
}