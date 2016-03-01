//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef Wall_H
#define Wall_H

//#include "d3dUtil.h"
#include "d3dUtil.h"

namespace WallNameSpace
{
	const int MAX_HITS = 50;
}

class Wall
{
public:

	Wall();
	~Wall();
	D3DXVECTOR3 getSize();

	void init(ID3D10Device* device, float scale);
	void init(ID3D10Device* device, float scale, D3DXCOLOR c);
	void draw();
	float scale;
private:
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};


#endif // Wall_H
