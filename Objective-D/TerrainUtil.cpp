#include "TerrainUtil.h"
#include "CBVUtil.h"

// �ͷ����� ����ϴ� ��ƿ�̴�.
// �ͷ��� ��ƿ�� �� ���� �� ���� �Ž��� ���� �� �ִ�.

// �ͷ������� ����� �Ž��� �����Ѵ�
void TerrainUtil::SetMesh(Mesh* MeshPtr) {
	TerrainMesh = MeshPtr;
}

// �ͷ����� �������� ���̴��� �����Ѵ�
void TerrainUtil::SetShader(Shader* ShaderPtr) {
	TerrainShader = ShaderPtr;
}

// �ͷ��ο� ������ �ؽ�ó�� �����Ѵ�
void TerrainUtil::SetTexture(Texture* TexturePtr) {
	TerrainTexture = TexturePtr;
}

// Ư�� ������Ʈ�� �ͷ����� �ٴڿ� ��Ҵ��� �˻��Ѵ�.
// ����� ��� true�� �����Ѵ�.
bool TerrainUtil::CheckFloor(XMFLOAT3 Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			return true;
	}

	return false;
}

// Ư�� ������Ʈ�l ��ġ�� �ͷ��� �ٴ��� ��ġ�� �̵���Ų��.
// �� �Լ��� ������Ʈ �������� ����� ��� ������Ʈ�� �ͷ��� �ٴڿ��� �������� �ʰ� �ȴ�.
void TerrainUtil::ClampToFloor(XMFLOAT3& Position) {
	if (TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴڿ� ������ ��� �ͷ��� �ٴ��� ���̷� ������Ʈ�� �̵���Ų��
void TerrainUtil::CheckCollision(XMFLOAT3& Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
	}
}

// �ͷ����� ������ �����Ѵ�
void TerrainUtil::SetColor(XMFLOAT3 ColorValue) {
	TerrainColor.x = ColorValue.x;
	TerrainColor.y = ColorValue.y;
	TerrainColor.z = ColorValue.z;
}

// �ͷ����� ũ�⸦ �����Ѵ�
void TerrainUtil::Scale(XMFLOAT3 SizeValue) {
	XMMATRIX ScaleMat = XMMatrixScaling(SizeValue.x, SizeValue.y, SizeValue.z);
	TranslateMatrix = Mat4::Multiply(ScaleMat, TranslateMatrix);
}

// �ͷ����� �������Ѵ�
void TerrainUtil::Render(ID3D12GraphicsCommandList* CmdList) {
	if (TerrainTexture)
		TerrainTexture->Render(CmdList);

	if (TerrainShader)
		TerrainShader->Render(CmdList);

	UpdateShaderVariables(CmdList);

	if (TerrainMesh)
		TerrainMesh->Render(CmdList);
}

void TerrainUtil::EnableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, BoolLightCBV, 1);
}

void TerrainUtil::DisableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, BoolLightCBV, 0);
}

void TerrainUtil::SendLightInfo(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::InputCBV(CmdList, LightCBV, 0);
}

void TerrainUtil::FlipTexture(ID3D12GraphicsCommandList* CmdList, bool H_Flip, bool V_Flip) {
	int Index{};

	if (!H_Flip && !V_Flip)     Index = 0;
	else if (H_Flip && !V_Flip) Index = 1;
	else if (!H_Flip && V_Flip) Index = 2;
	else if (H_Flip && V_Flip)  Index = 3;

	CBVUtil::InputCBV(CmdList, FlipCBV, Index);
}

void TerrainUtil::SetAlpha(ID3D12GraphicsCommandList* CmdList, float AlphaValue) {
	AlphaInfo Alphainfo{ AlphaValue };
	CmdList->SetGraphicsRoot32BitConstants(4, 1, &Alphainfo, 0);
}

// ���̴��� �ͷ��� ��ȯ ���� �����Ѵ�
void TerrainUtil::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMLoadFloat4x4(&TranslateMatrix);
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(1, 3, &TerrainColor, 16);
}