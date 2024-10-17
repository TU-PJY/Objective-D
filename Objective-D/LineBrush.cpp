#include "LineBrush.h"

// 선의 색상을 지정한다.
void LineBrush::SetColor(float R, float G, float B) {
	LineColor.x = R;
	LineColor.y = G;
	LineColor.z = B;
}

void LineBrush::Init(ID3D12GraphicsCommandList* CmdList) {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();
	TransparencyValue = 1.0f;

	camera.SetToStaticMode();
	CBVUtil::InputCBV(CmdList, BoolLightCBV, 0);
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

	AlphaInfo Alphainfo{ TransparencyValue };
	CmdList->SetGraphicsRoot32BitConstants(ALPHA_INDEX, 1, &Alphainfo, 0);

	LineTex->Render(CmdList);
	BasicShader->Render(CmdList, true);

	camera.InitMatrix();
	camera.GenerateOrthoMatrix(1.0, 1.0, ASPECT_RATIO, 0.0f, 10.0f);
	camera.SetViewportsAndScissorRects(CmdList);
	camera.UpdateShaderVariables(CmdList);

	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(GAME_OBJECT_INDEX, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(GAME_OBJECT_INDEX, 3, &LineColor, 16);

	ImagePannel->Render(CmdList);
}