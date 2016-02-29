//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Wall.h"
#include "Vertex.h"

Wall::Wall()
: mNumVertices(0), mNumFaces(0), md3dDevice(0), mVB(0), mIB(0)
{
}
 
Wall::~Wall()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
void Wall::init(ID3D10Device* device, float scale, D3DXCOLOR c)
{
	md3dDevice = device;
 
	mNumVertices = 8;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(-10.0f, 0.0f, -1.0f), c},
		{D3DXVECTOR3(-10.0f, 0.0f, 1.0f), c},
		{D3DXVECTOR3(-10.0f, 5.0f, -1.0f), c},
		{D3DXVECTOR3(-10.0f, 5.0f, 1.0f), c},
		{D3DXVECTOR3(10.0f, 0.0f, +1.0f), c},
		{D3DXVECTOR3(10.0f, 0.0f, -1.0f), c},
		{D3DXVECTOR3(10.0f, 5.0f, 1.0f), c},
		{D3DXVECTOR3(10.0f, 5.0f, -1.0f), c}
    };

	// Scale the box.
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// side face
		0, 1, 2,
		1, 3, 2,

		// back face
		5, 0, 7,
		0, 2, 7,

		// side face
		5, 7, 6,
		4, 5, 6,

		// face
		1, 4, 3,
		4, 6, 3,

		// top face
		3, 6, 2,
		6, 7, 2,

		// bottom face
		3, 6, 2, 
		6, 7, 2
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
void Wall::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;
 
	mNumVertices = 8;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(-10.0f, 0.0f, -1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(-10.0f, 0.0f, 1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(-10.0f, 5.0f, -1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(-10.0f, 5.0f, 1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(10.0f, 0.0f, +1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(10.0f, 0.0f, -1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(10.0f, 5.0f, 1.0f), CHARCOAL_GREY},
		{D3DXVECTOR3(10.0f, 5.0f, -1.0f), CHARCOAL_GREY}
    };

	// Scale the box.
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// side face
		0, 1, 2,
		1, 2, 3,

		// back face
		5, 0, 7,
		0, 3, 7,

		// side face
		5, 4, 7,
		4, 6, 7,

		// face
		1, 4, 3,
		4, 6, 3,

		// top face
		3, 6, 2,
		6, 7, 2,

		// bottom face
		3, 6, 2, 
		6, 7, 2
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Wall::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}