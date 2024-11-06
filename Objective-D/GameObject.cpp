#include "GameObject.h"
#include "CameraUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// GameObject Ŭ������ ��� ��ü���� ��ӹ޴� �θ� Ŭ�����̴�.
// ��� ��ü�� �ݵ�� �� Ŭ�����κ��� �󺹹޾ƾ� Scene�� ��ü�� ������Ʈ�ϰ� �������Ѵ�.
// �Ϻ� �Լ����� ������ ���Ϸ� �и� �����̴� �ڵ忡 ������ ���� �� �ִ�.


// ��ü�� ������ ���¸� �ʱ�ȭ �Ѵ�. ��� ��ü�� �� �Լ��� ������ ������ ���۵ȴ�.
// �⺻ RANDER_TYPE_PERS�� Ÿ���� �����Ǿ��ִ�.
void GameObject::InitRenderState(int RenderTypeFlag) {
	// RENDER_TYPE_NONE�� ��� ��� �ʱ�ȭ�� �������� �ʴ´�. 
	// ��ȯ�� �ǽð����� ������ �ʿ䰡 ���� �ͷ����̳� ������ ������ �뵵�� ����ϴ� �÷����̴�.
	if (RenderTypeFlag != RENDER_TYPE_NONE) {
		TranslateMatrix = Mat4::Identity();
		RotateMatrix = Mat4::Identity();
		ScaleMatrix = Mat4::Identity();
	}

	// ��� ��� ����
	RenderType = RenderTypeFlag;

	// �ؽ�ó ���� �ʱ�ȭ
	ObjectAlpha = 1.0f;
	FlipTexture(FLIP_TYPE_NONE);

	// �Ž� ���� �ʱ�ȭ
	SetColor(XMFLOAT3(0.0, 0.0, 0.0));

	// �̹��� Ÿ�� �������� ��� �̹��� ���� ����
	if (RenderTypeFlag == RENDER_TYPE_IMAGE)
		SetToImageMode();

	else // ������ Ÿ���� ��� �⺻ ���� ����
		SetToDefaultMode();
}

// ��ü �޽��� ������ �����Ѵ�.
void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

// ��ü �Ž��� ������ �����Ѵ�.
void GameObject::SetColorRGB(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

// ���� ��� ��Ȱ��ȭ
void GameObject::DisableLight() {
	CBVUtil::Input(ObjectCmdList, BoolLightCBV, 0);
}

// ���� ��� Ȱ��ȭ
void GameObject::EnableLight() {
	CBVUtil::Input(ObjectCmdList, BoolLightCBV, 1);
}

// �ؽ�ó�� ������Ų��. �𵨿� ���� �ٸ��� ����� �� �ִ�.
void GameObject::FlipTexture(int FlipType) {
	CBVUtil::Input(ObjectCmdList, FlipCBV, FlipType);
}

// �޽��� ������ �Ѵ�. ��ȯ �۾��� ���� �� �� �������� �����Ѵ�.
void GameObject::RenderMesh(Mesh* MeshPtr, Texture* TexturePtr, Shader* ShaderPtr, float Alpha, bool DepthTestFlag) {
	// �ؽ�ó ���ε� �� ���̴� ����
	// �̹��� ���� Ÿ���� �ƴ϶�� ���� �˻� �÷��׸� �������� �Ѵ�.
	BindTexture(TexturePtr);
	if(RenderType != RENDER_TYPE_IMAGE)
		UseShader(ShaderPtr, DepthTestFlag);
	else 
		UseShader(ShaderPtr, false);

	// ī�޶� ����
	SetCamera();

	// ���İ� ����
	ObjectAlpha = Alpha;

	// ���̴��� ���� ����
	UpdateShaderVariables();

	// �Ž� ������
	UseMesh(MeshPtr);
}

// ���콺 ������κ��� ȸ���� ������Ʈ �Ѵ�.
void GameObject::UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY) {
	RotationX += DeltaY;
	RotationY += DeltaX;
}

// ���콺 ������κ��� ȸ���� ������Ʈ �Ѵ�.
void GameObject::UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY) {
	Rotation.x += DeltaY;
	Rotation.y += DeltaX;
}

// ���� ��Ⱦ�� ���Ѵ�. UI�� �����Ҷ� �ַ� ����Ѵ�.
float GameObject::ASP(float Value) {
	return ASPECT * Value;
}

// ��ŷ �� ����ϴ� �Լ��̴�. ���α׷��Ӱ� �� �Լ��� ���� ����� ���� ����.
int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;

	XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
	GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
	nIntersected = MeshPtr->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);

	return(nIntersected);
}

// ������Ʈ�� Ŀ�ǵ� ����Ʈ�� �ο��Ѵ�.
void GameObject::InputCommandList(ID3D12GraphicsCommandList* CmdList) {
	ObjectCmdList = CmdList;
}


//////////////////////////////////////// private
// �ؽ�ó�� ���ε��Ѵ�. 
void GameObject::BindTexture(Texture* TexturePtr) {
	if (TexturePtr)
		TexturePtr->Render(ObjectCmdList);
}

// ���̴��� �����Ѵ�.
void GameObject::UseShader(Shader* ShaderPtr, bool DepthTest) {
	if (ShaderPtr)
		ShaderPtr->Render(ObjectCmdList, DepthTest);
}

// �Ž��� ���� ������ �Ѵ�
void GameObject::UseMesh(Mesh* MeshPtr) {
	if (MeshPtr)
		MeshPtr->Render(ObjectCmdList);
}

// ��İ� ���̴� �� ���� ���� ������ ���̴��� �����Ѵ�. RenderMesh�Լ��� �����ϸ� �� �Լ��� ����ȴ�. ��, ���� ����� ���� ����.
void GameObject::UpdateShaderVariables() {
	CBVUtil::Input(ObjectCmdList, LightCBV, 0);

	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(ObjectCmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(ObjectCmdList, &ModelColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(ObjectCmdList, &ObjectAlpha, ALPHA_INDEX, 1, 0);
}

// �̹��� ��� ���� ��ȯ�Ѵ�. �ؽ�ó ���� ���� �� ���� ����� ��Ȱ��ȭ �Ѵ�.
void GameObject::SetToImageMode() {
	FlipTexture(FLIP_TYPE_V);
	DisableLight();
	camera.SetToStaticMode();
}

// �⺻ ��� ���� ��ȯ�Ѵ�. �ؽ�ó ���� ���� �� ���� ����� Ȱ��ȭ �Ѵ�.
void GameObject::SetToDefaultMode() {
	FlipTexture(FLIP_TYPE_NONE);
	EnableLight();
	camera.SetToDefaultMode();
}

// ������ �� ī�޶� �����Ѵ�.
void GameObject::SetCamera() {
	// ī�޶� �� ����� �����Ѵ�.
	camera.SetViewMatrix();

	// ���� Ÿ�Կ� ���� �ٸ� ���� ����� �����Ѵ�.
	if (RenderType == RENDER_TYPE_PERS)
		camera.GeneratePerspectiveMatrix(0.01f, 5000.0f, ASPECT, 45.0f);

	else if (RenderType == RENDER_TYPE_ORTHO || RenderType == RENDER_TYPE_IMAGE)
		camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0f, 10.0f);

	camera.SetViewportsAndScissorRects(ObjectCmdList);
	camera.UpdateShaderVariables(ObjectCmdList);
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