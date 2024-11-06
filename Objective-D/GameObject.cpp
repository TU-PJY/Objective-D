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
// RENDER_TYPE_3D_STATIC�Ǵ� RENDER_TYPE_2D_STATIC�� ��� ��� �ʱ�ȭ�� �������� �ʴ´�. 
void GameObject::InitRenderState(int RenderTypeFlag) {
	// ��� ��� ����
	RenderType = RenderTypeFlag;

	if (RenderTypeFlag != RENDER_TYPE_3D_STATIC && RenderTypeFlag != RENDER_TYPE_2D_STATIC) {
		TranslateMatrix = Mat4::Identity();
		RotateMatrix = Mat4::Identity();
		ScaleMatrix = Mat4::Identity();
		ImageAspectMatrix = Mat4::Identity();
	}

	// �Ž� ���� �ʱ�ȭ
	SetColor(XMFLOAT3(0.0, 0.0, 0.0));

	// �ؽ�ó ���� �ʱ�ȭ
	FlipTexture(FLIP_TYPE_NONE);
	ObjectAlpha = 1.0f;

	// ��� ��� ����
	switch(RenderTypeFlag) {
	case RENDER_TYPE_2D: case RENDER_TYPE_2D_STATIC:
		SetToImageMode();
		break;

	case RENDER_TYPE_3D: case RENDER_TYPE_3D_STATIC:
		SetToDefaultMode(); 
		break;
	}
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
	// �̹��� ��� ��忡���� ���� ������ �⺻ �����̱� ������ ������ �ٸ��� �����Ѵ�.
	if (RenderType == RENDER_TYPE_2D || RenderType == RENDER_TYPE_2D_STATIC) {
		switch (FlipType) {
		case FLIP_TYPE_V:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_NONE);
			break;

		case FLIP_TYPE_H:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_HV);
			break;

		case FLIP_TYPE_HV:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_H);
			break;

		case FLIP_TYPE_NONE:
			CBVUtil::Input(ObjectCmdList, FlipCBV, FLIP_TYPE_V);
			break;
		}
	}

	else
		CBVUtil::Input(ObjectCmdList, FlipCBV, FlipType);
}

// 3D ������
void GameObject::Render3D(Mesh* MeshPtr, Texture* TexturePtr, float AlphaValue, bool DepthTestFlag) {
	TexturePtr->Render(ObjectCmdList);
	ObjectShader->Render(ObjectCmdList, DepthTestFlag);
	CBVUtil::Input(ObjectCmdList, LightCBV, 0);

	ObjectAlpha = AlphaValue;

	SetCamera();
	UpdateShaderVariables();

	MeshPtr->Render(ObjectCmdList);
}

// 2D ������
void GameObject::Render2D(Texture* TexturePtr, float AlphaValue, bool EnableAspect) {
	if(EnableAspect)
		Transform::ImageAspect(ImageAspectMatrix, TexturePtr->Width, TexturePtr->Height);
	TexturePtr->Render(ObjectCmdList);
	ImageShader->Render(ObjectCmdList, false);
	ObjectAlpha = AlphaValue;

	SetCamera();
	UpdateShaderVariables();

	ImagePannel->Render(ObjectCmdList);
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

// ��İ� ���̴� �� ���� ���� ������ ���̴��� �����Ѵ�. RenderMesh�Լ��� �����ϸ� �� �Լ��� ����ȴ�. ��, ���� ����� ���� ����.
void GameObject::UpdateShaderVariables() {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RotateMatrix), XMLoadFloat4x4(&TranslateMatrix));
	ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), ResultMatrix);

	// �̹��� ��� ����ϰ�� ��Ⱦ�� �����Ѵ�.
	if (RenderType == RENDER_TYPE_2D || RenderType == RENDER_TYPE_2D_STATIC)
		ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ImageAspectMatrix), ResultMatrix);

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(ObjectCmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(ObjectCmdList, &ModelColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(ObjectCmdList, &ObjectAlpha, GAME_OBJECT_INDEX, 1, 19);
}

// �̹��� ��� ���� ��ȯ�Ѵ�. �ؽ�ó ���� ���� �� ���� ����� ��Ȱ��ȭ �Ѵ�.
void GameObject::SetToImageMode() {
	DisableLight();
	camera.SetToStaticMode();
}

// �⺻ ��� ���� ��ȯ�Ѵ�. �ؽ�ó ���� ���� �� ���� ����� Ȱ��ȭ �Ѵ�.
void GameObject::SetToDefaultMode() {
	EnableLight();
	camera.SetToDefaultMode();
}

// ������ �� ī�޶� �����Ѵ�.
void GameObject::SetCamera() {
	// ī�޶� �� ����� �����Ѵ�.
	camera.SetViewMatrix();

	// ���� Ÿ�Կ� ���� �ٸ� ����� �ʱ�ȭ �Ѵ�.
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