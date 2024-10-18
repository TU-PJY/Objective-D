#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// 터레인을 담당하는 유틸이다.
// 터레인 유틸은 한 번에 한 개의 매쉬만 가질 수 있다.

// 터레인으로 사용할 매쉬를 설정한다
void TerrainUtil::SetMesh(Mesh* MeshPtr) {
	TerrainMesh = MeshPtr;
}

// 터레인을 렌더링할 쉐이더를 설정한다
void TerrainUtil::SetShader(Shader* ShaderPtr) {
	TerrainShader = ShaderPtr;
}

// 터레인에 매핑할 텍스처를 설정한다
void TerrainUtil::SetTexture(Texture* TexturePtr) {
	TerrainTexture = TexturePtr;
}

// 특정 오브젝트가 터레인의 바닥에 닿았는지 검사한다.
// 닿았을 경우 true를 리턴한다.
bool TerrainUtil::CheckFloor(XMFLOAT3 Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			return true;
	}

	return false;
}

// 특정 오브젝트의 위치를 터레인 바닥의 위치로 이동시킨다.
// 이 함수를 업데이트 루프에서 사용할 경우 오브젝트가 터레인 바닥에서 떨어지지 않게 된다.
void TerrainUtil::ClampToFloor(XMFLOAT3& Position) {
	if (TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
}

// 특정 오브젝트의 위치가 터레인 바닥에 도달할 경우 터레인 바닥의 높이로 오브젝트를 이동시킨다
void TerrainUtil::CheckCollision(XMFLOAT3& Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
	}
}

// 터레인의 색상을 변경한다
void TerrainUtil::SetColor(float R, float G, float B) {
	TerrainColor.x = R;
	TerrainColor.y = G;
	TerrainColor.z = B;
}

// 터레인의 크기를 설정한다
void TerrainUtil::Scale(XMFLOAT3 SizeValue) {
	XMMATRIX ScaleMat = XMMatrixScaling(SizeValue.x, SizeValue.y, SizeValue.z);
	TranslateMatrix = Mat4::Multiply(ScaleMat, TranslateMatrix);
}

// 터레인을 렌더링한다
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
	CBVUtil::Input(CmdList, BoolLightCBV, 1);
}

void TerrainUtil::DisableLight(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, BoolLightCBV, 0);
}

void TerrainUtil::InputLightInfo(ID3D12GraphicsCommandList* CmdList) {
	CBVUtil::Input(CmdList, LightCBV, 0);
}

void TerrainUtil::FlipTexture(ID3D12GraphicsCommandList* CmdList, bool H_Flip, bool V_Flip) {
	int Index{};

	if (!H_Flip && !V_Flip)     Index = 0;
	else if (H_Flip && !V_Flip) Index = 1;
	else if (!H_Flip && V_Flip) Index = 2;
	else if (H_Flip && V_Flip)  Index = 3;

	CBVUtil::Input(CmdList, FlipCBV, Index);
}

void TerrainUtil::SetAlpha(ID3D12GraphicsCommandList* CmdList, float Alpha) {
	AlphaValue = Alpha;
}

// 쉐이더로 터레인 변환 값을 전달한다
void TerrainUtil::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMLoadFloat4x4(&TranslateMatrix);
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));

	RCUtil::Input(CmdList, &xmf4x4World, GAME_OBJECT_INDEX, 16, 0);
	RCUtil::Input(CmdList, &TerrainColor, GAME_OBJECT_INDEX, 3, 16);
	RCUtil::Input(CmdList, &AlphaValue, ALPHA_INDEX, 1, 0);
}