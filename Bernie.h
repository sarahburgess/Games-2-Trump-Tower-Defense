//=======================================================================================
// Bernie.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef Bernie_H
#define Bernie_H

//#include "d3dUtil.h"
#include "d3dUtil.h"


namespace BernieNameSpace 
{
	const int MAX_SPEED = 12;
	const short int MAX_HITS = 3;
}
class Bernie
{
public:

	Bernie();
	~Bernie();

	void init(ID3D10Device* device, float scale);
	void init(ID3D10Device* device, float scale, D3DXCOLOR c);
	void draw();

private:
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};


#endif // Bernie_H
