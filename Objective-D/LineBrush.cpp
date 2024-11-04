#include "LineBrush.h"
#include "TextureUtil.h"
#include "ShaderUtil.h"
#include "MeshUtil.h"
#include "GameResource.h"
#include "CBVUtil.h"
#include "TransformUtil.h"
#include "CameraUtil.h"
#include "RootConstants.h"
#include "MathUtil.h"
#include "RootConstantUtil.h"

// 선의 색상을 지정한다.
void LineBrush::SetColor(float R, float G, float B) {
	LineColor.x = R;
	LineColor.y = G;
	LineColor.z = B;
}

void LineBrush::Init(ID3D12GraphicsCommandList* CmdList) {
	TranslateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();
	TransparencyValue = 1.0f;
	CBVUtil::Input(CmdList, BoolLightCBV, 0);

	camera.SetToStaticMode();
	camera.SetViewMatrix();
	camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT, 0.0, 10.0);
	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);
}

// 선을 그린다.
void LineBrush::Draw(ID3D12GraphicsCommandList* CmdList, float X1, float Y1, float X2, float Y2, float Width, float Alpha) {
	Init(CmdList);

	TransparencyValue = Alpha;

	Length = Math::CalcDistance2D(X1, Y1, X2, Y2);
	Rotation = Math::CalcDegree2D(X1, Y1, X2, Y2);

	Transform::Move(TranslateMatrix, X1, Y1, 0.0);
	Transform::Rotate(TranslateMatrix, Rotation, 0.0, 0.0, 1.0);
	Transform::Move(TranslateMatrix, Length / 2.0, 0.0, 0.0);
	Transform::Scale(ScaleMatrix, Width + Length, Width, 1.0);

	LineTex->Render(CmdList);
	ObjectShader->Render(CmdList, false);

	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&TranslateMatrix));
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &LineColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &TransparencyValue, ALPHA_INDEX, 1, 0);

	ImagePannel->Render(CmdList);
}