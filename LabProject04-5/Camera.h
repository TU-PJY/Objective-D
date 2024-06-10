#pragma once

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

class Camera {
protected:
	XMFLOAT3						m_xmf3Right{};
	XMFLOAT3						m_xmf3Up{};
	XMFLOAT3						m_xmf3Look{};

	DWORD							m_nMode{};

	XMFLOAT3						m_xmf3LookAtWorld{};
	XMFLOAT3						m_xmf3Offset{};
	float           				m_fTimeLag{};

	XMFLOAT4X4						m_xmf4x4View{};
	XMFLOAT4X4						m_xmf4x4Projection{};

#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	BoundingFrustum					m_xmFrustumView{};
	BoundingFrustum					m_xmFrustumWorld{};
	XMFLOAT4X4						m_xmf4x4InverseView{};
#else
	XMFLOAT4						m_pxmf4FrustumPlanes[6]{};
#endif

	D3D12_VIEWPORT					m_d3dViewport{};
	D3D12_RECT						m_d3dScissorRect{};

public:
	XMFLOAT3						CamPos{};
	float           				CamPitch{};
	float           				CamRoll{};
	float           				CamYaw{};


	Camera() {
		m_xmf4x4View = Mat4::Identity();
		m_xmf4x4Projection = Mat4::Identity();

		m_d3dViewport = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
		m_d3dScissorRect = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

		CamPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		CamPitch = 0.0f;
		CamRoll = 0.0f;
		CamYaw = 0.0f;

		m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
		m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
		m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_xmf3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf3LookAtWorld = XMFLOAT3(0.0f, 0.0f, 0.0f);

		m_nMode = 0x00;
		m_fTimeLag = 0.0f;
	}


	virtual ~Camera() {}


	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {}


	virtual void ReleaseShaderVariables() {}


	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) {
		XMFLOAT4X4 xmf4x4View;
		XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
		pd3dCommandList->SetGraphicsRoot32BitConstants(2, 16, &xmf4x4View, 0);

		XMFLOAT4X4 xmf4x4Projection;
		XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
		pd3dCommandList->SetGraphicsRoot32BitConstants(2, 16, &xmf4x4Projection, 16);

		pd3dCommandList->SetGraphicsRoot32BitConstants(2, 3, &CamPos, 32);
	}


	void GenerateViewMatrix() {
		m_xmf4x4View = Mat4::LookAtLH(CamPos, m_xmf3LookAtWorld, m_xmf3Up);
	}


	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up) {
		CamPos = xmf3Position;
		m_xmf3LookAtWorld = xmf3LookAt;
		m_xmf3Up = xmf3Up;

		GenerateViewMatrix();
	}


	void RegenerateViewMatrix() {
		m_xmf3Look = Vec3::Normalize(m_xmf3Look);
		m_xmf3Right = Vec3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
		m_xmf3Up = Vec3::CrossProduct(m_xmf3Look, m_xmf3Right, true);

		m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
		m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
		m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
		m_xmf4x4View._41 = -Vec3::DotProduct(CamPos, m_xmf3Right);
		m_xmf4x4View._42 = -Vec3::DotProduct(CamPos, m_xmf3Up);
		m_xmf4x4View._43 = -Vec3::DotProduct(CamPos, m_xmf3Look);

		m_xmf4x4InverseView._11 = m_xmf3Right.x; m_xmf4x4InverseView._12 = m_xmf3Right.y; m_xmf4x4InverseView._13 = m_xmf3Right.z;
		m_xmf4x4InverseView._21 = m_xmf3Up.x; m_xmf4x4InverseView._22 = m_xmf3Up.y; m_xmf4x4InverseView._23 = m_xmf3Up.z;
		m_xmf4x4InverseView._31 = m_xmf3Look.x; m_xmf4x4InverseView._32 = m_xmf3Look.y; m_xmf4x4InverseView._33 = m_xmf3Look.z;
		m_xmf4x4InverseView._41 = CamPos.x; m_xmf4x4InverseView._42 = CamPos.y; m_xmf4x4InverseView._43 = CamPos.z;

		m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
	}


	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle) {
		//	m_xmf4x4Projection = Mat4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
		XMMATRIX xmmtxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
		XMStoreFloat4x4(&m_xmf4x4Projection, xmmtxProjection);

#ifdef _WITH_DIERECTX_MATH_FRUSTUM
		BoundingFrustum::CreateFromMatrix(m_xmFrustumView, xmmtxProjection);
#endif
	}


	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f) {
		m_d3dViewport.TopLeftX = float(xTopLeft);
		m_d3dViewport.TopLeftY = float(yTopLeft);
		m_d3dViewport.Width = float(nWidth);
		m_d3dViewport.Height = float(nHeight);
		m_d3dViewport.MinDepth = fMinZ;
		m_d3dViewport.MaxDepth = fMaxZ;
	}


	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom) {
		m_d3dScissorRect.left = xLeft;
		m_d3dScissorRect.top = yTop;
		m_d3dScissorRect.right = xRight;
		m_d3dScissorRect.bottom = yBottom;
	}


	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList) {
		pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
		pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);
	}


	void SetMode(DWORD nMode) { m_nMode = nMode; }
	DWORD GetMode() { return(m_nMode); }

	void SetPosition(XMFLOAT3 xmf3Position) { CamPos = xmf3Position; }
	XMFLOAT3& GetPosition() { return(CamPos); }

	void SetLookAtPosition(XMFLOAT3 xmf3LookAtWorld) { m_xmf3LookAtWorld = xmf3LookAtWorld; }
	XMFLOAT3& GetLookAtPosition() { return(m_xmf3LookAtWorld); }

	XMFLOAT3& GetRightVector() { return(m_xmf3Right); }
	XMFLOAT3& GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3& GetLookVector() { return(m_xmf3Look); }

	float& GetPitch() { return(CamPitch); }
	float& GetRoll() { return(CamRoll); }
	float& GetYaw() { return(CamYaw); }

	void SetOffset(XMFLOAT3 xmf3Offset) { m_xmf3Offset = xmf3Offset; }
	XMFLOAT3& GetOffset() { return(m_xmf3Offset); }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }

	XMFLOAT4X4 GetViewMatrix() { return(m_xmf4x4View); }
	XMFLOAT4X4 GetProjectionMatrix() { return(m_xmf4x4Projection); }
	D3D12_VIEWPORT GetViewport() { return(m_d3dViewport); }
	D3D12_RECT GetScissorRect() { return(m_d3dScissorRect); }

	virtual void Move(const XMFLOAT3& xmf3Shift) { CamPos.x += xmf3Shift.x; CamPos.y += xmf3Shift.y; CamPos.z += xmf3Shift.z; }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed) { }
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }


	void CalculateFrustumPlanes() {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
		m_xmFrustumView.Transform(m_xmFrustumWorld, XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4View)));
#else
		XMFLOAT4X4 mtxViewProjection = Mat4::Multiply(m_xmf4x4View, m_xmf4x4Projection);

		m_pxmf4FrustumPlanes[0].x = -(mtxViewProjection._14 + mtxViewProjection._11);
		m_pxmf4FrustumPlanes[0].y = -(mtxViewProjection._24 + mtxViewProjection._21);
		m_pxmf4FrustumPlanes[0].z = -(mtxViewProjection._34 + mtxViewProjection._31);
		m_pxmf4FrustumPlanes[0].w = -(mtxViewProjection._44 + mtxViewProjection._41);

		m_pxmf4FrustumPlanes[1].x = -(mtxViewProjection._14 - mtxViewProjection._11);
		m_pxmf4FrustumPlanes[1].y = -(mtxViewProjection._24 - mtxViewProjection._21);
		m_pxmf4FrustumPlanes[1].z = -(mtxViewProjection._34 - mtxViewProjection._31);
		m_pxmf4FrustumPlanes[1].w = -(mtxViewProjection._44 - mtxViewProjection._41);

		m_pxmf4FrustumPlanes[2].x = -(mtxViewProjection._14 - mtxViewProjection._12);
		m_pxmf4FrustumPlanes[2].y = -(mtxViewProjection._24 - mtxViewProjection._22);
		m_pxmf4FrustumPlanes[2].z = -(mtxViewProjection._34 - mtxViewProjection._32);
		m_pxmf4FrustumPlanes[2].w = -(mtxViewProjection._44 - mtxViewProjection._42);

		m_pxmf4FrustumPlanes[3].x = -(mtxViewProjection._14 + mtxViewProjection._12);
		m_pxmf4FrustumPlanes[3].y = -(mtxViewProjection._24 + mtxViewProjection._22);
		m_pxmf4FrustumPlanes[3].z = -(mtxViewProjection._34 + mtxViewProjection._32);
		m_pxmf4FrustumPlanes[3].w = -(mtxViewProjection._44 + mtxViewProjection._42);

		m_pxmf4FrustumPlanes[4].x = -(mtxViewProjection._13);
		m_pxmf4FrustumPlanes[4].y = -(mtxViewProjection._23);
		m_pxmf4FrustumPlanes[4].z = -(mtxViewProjection._33);
		m_pxmf4FrustumPlanes[4].w = -(mtxViewProjection._43);

		m_pxmf4FrustumPlanes[5].x = -(mtxViewProjection._14 - mtxViewProjection._13);
		m_pxmf4FrustumPlanes[5].y = -(mtxViewProjection._24 - mtxViewProjection._23);
		m_pxmf4FrustumPlanes[5].z = -(mtxViewProjection._34 - mtxViewProjection._33);
		m_pxmf4FrustumPlanes[5].w = -(mtxViewProjection._44 - mtxViewProjection._43);

		for (int i = 0; i < 6; i++) m_pxmf4FrustumPlanes[i] = Plane::Normalize(m_pxmf4FrustumPlanes[i]);
#endif
	}


	bool IsInFrustum(BoundingBox& xmBoundingBox) {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
		return(m_xmFrustumWorld.Intersects(xmBoundingBox));
#else
		XMFLOAT3 xmf3NearPoint, xmf3Normal;
		XMFLOAT3 xmf3Minimum = Vec3::Subtract(xmBoundingBox.Center, xmBoundingBox.Extents);
		XMFLOAT3 xmf3Maximum = Vec3::Add(xmBoundingBox.Center, xmBoundingBox.Extents);
		for (int i = 0; i < 6; i++)
		{
			xmf3Normal = XMFLOAT3(m_pxmf4FrustumPlanes[i].x, m_pxmf4FrustumPlanes[i].y, m_pxmf4FrustumPlanes[i].z);
			if (xmf3Normal.x >= 0.0f)
			{
				if (xmf3Normal.y >= 0.0f)
				{
					if (xmf3Normal.z >= 0.0f)
					{
						xmf3NearPoint = XMFLOAT3(xmf3Minimum.x, xmf3Minimum.y, xmf3Minimum.z);
					}
					else
					{
						xmf3NearPoint = XMFLOAT3(xmf3Minimum.x, xmf3Minimum.y, xmf3Maximum.z);
					}
				}
				else
				{
					if (xmf3Normal.z >= 0.0f)
					{
						xmf3NearPoint = XMFLOAT3(xmf3Minimum.x, xmf3Maximum.y, xmf3Minimum.z);
					}
					else
					{
						xmf3NearPoint = XMFLOAT3(xmf3Minimum.x, xmf3Maximum.y, xmf3Maximum.z);
					}
				}
			}
			else
			{
				if (xmf3Normal.y >= 0.0f)
				{
					if (xmf3Normal.z >= 0.0f)
					{
						xmf3NearPoint = XMFLOAT3(xmf3Maximum.x, xmf3Minimum.y, xmf3Minimum.z);
					}
					else
					{
						xmf3NearPoint = XMFLOAT3(xmf3Maximum.x, xmf3Minimum.y, xmf3Maximum.z);
					}
				}
				else
				{
					if (xmf3Normal.z >= 0.0f)
					{
						xmf3NearPoint = XMFLOAT3(xmf3Maximum.x, xmf3Maximum.y, xmf3Minimum.z);
					}
					else
					{
						xmf3NearPoint = XMFLOAT3(xmf3Maximum.x, xmf3Maximum.y, xmf3Maximum.z);
					}
				}
			}
			if ((Vec3::DotProduct(xmf3Normal, xmf3NearPoint) + m_pxmf4FrustumPlanes[i].w) > 0.0f) return(false);
		}

		return(true);
#endif
	}


	bool IsInFrustum(BoundingOrientedBox& xmBoundingBox) {
#ifdef _WITH_DIERECTX_MATH_FRUSTUM
		return(m_xmFrustumWorld.Intersects(xmBoundingBox));
#else
#endif
	}
};

extern Camera m_pCamera;