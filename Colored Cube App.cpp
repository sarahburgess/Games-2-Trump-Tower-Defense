//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================


#include "d3dApp.h"
#include "Box.h"
#include "GameObject.h"
#include "Line.h"
#include "Quad.h"
#include <d3dx9math.h>
#include "LineObject.h"
#include "input.h"
#include "WallObject.h"
#include "Crosshairs.h"
#include "audio.h"
#include "BernieObject.h"


class ColoredCubeApp : public D3DApp
{
public:
	ColoredCubeApp(HINSTANCE hInstance);
	~ColoredCubeApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	void buildFX();
	void buildVertexLayouts();

private:
	Quad quad1;
	Line line, line2;

	//Box mBox, redBox;
	//GameObject gameObject1, gameObject2, gameObject3, spinner;
	LineObject xLine, yLine, zLine;
	Wall trumpWall;
	WallObject trumpWallObj;
	Crosshairs crosshairObjHor,crosshairObjVert;
	Bernie bern;
	BernieObject bernies[NUMBERN];
	Box bullet;
	Box bernieBullet;
	GameObject bullets[MAXBULL];
	GameObject bernieBullets[MAXBULL];
	Input *input;

	float currentTime;
	Audio *audio;

	float spinAmount;

	bool gameActive;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	//my addition
	ID3D10EffectVariable* mfxFLIPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	float shotTimer;
	float bernieShotTimer[NUMBERN];

	bool didShoot;

	bool berniesRemaining;

	//my edits
	D3DXMATRIX worldBox1, worldBox2;

	float mTheta;
	float mPhi;
	float gameTimer;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	ColoredCubeApp theApp(hInstance);

	theApp.initApp();

	return theApp.run();
}

ColoredCubeApp::ColoredCubeApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
	mfxWVPVar(0), mTheta(0.0f), mPhi(PI*0.25f), gameTimer(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 

}

ColoredCubeApp::~ColoredCubeApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void ColoredCubeApp::initApp()
{
	D3DApp::initApp();

	input = new Input();

	currentTime = 0.0;

	berniesRemaining = true;

	gameActive = true;

	input->initialize(getMainWnd(), false); 

	audio = new Audio();
    if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')  // if sound files defined
    {
		if (!audio->initialize()){
			
		}
	}

	bern.init(md3dDevice,1.0f, BLACK);

	//mBox.init(md3dDevice, 1.0f, WHITE);
	//redBox.init(md3dDevice, 1.0f, RED);
	line.init(md3dDevice, 1.0f, BLACK);
	line2.init(md3dDevice, 1.0f, BLACK);
	trumpWall.init(md3dDevice,2.0f,CHARCOAL_GREY);
	xLine.init(&line, Vector3(0,0,0), 5);
	xLine.setPosition(Vector3(0,0,0));
	yLine.init(&line, Vector3(0,0,0), 5);
	yLine.setPosition(Vector3(0,0,0));
	yLine.setRotationZ(ToRadian(90));
	zLine.init(&line, Vector3(0,0,0), 5);
	zLine.setPosition(Vector3(0,0,0));
	zLine.setRotationY(ToRadian(90));

	trumpWallObj.init(&trumpWall, 1, Vector3(8,0,8), Vector3(0,0,0), 0,1);

	for(int i = 0; i < NUMBERN; i++) {
		bernies[i].init(&bern,2,Vector3(0,0,0),Vector3(0,0,-3),0,1);
		int randPosition = (int)trumpWallObj.getPosition().x + (rand() % (int)trumpWall.getSize().x);
		bernies[i].setStopPosition(15 + (rand()%30));
		
		bernies[i].setSpawnTime(MAX_SPAWN_TIME - (rand()%15));

		bernies[i].setPosition(Vector3(randPosition,0,60));
		

		if(rand()%3==1)
		{
			bernies[i].setActive();
			bernies[i].setVelocity(Vector3(bernies[i].getVelocity().x, bernies[i].getVelocity().y, -(int)rand()%BernieNameSpace::MAX_SPEED));
		}
		else
		{
			bernies[i].setInActive();
			bernies[i].setVelocity(Vector3(0,0,0));
		}
	}

	crosshairObjHor.init(&line2, Vector3(10,10,10), 1);
	crosshairObjHor.setPosition(Vector3(50,6+0.5,22.5+0.5));
	crosshairObjHor.setSpeed(15);
	crosshairObjHor.setRotationY(ToRadian(90));

	crosshairObjVert.init(&line, Vector3(10,10,10), 1);
	crosshairObjVert.setPosition(Vector3(50,6,22.5));
	crosshairObjVert.setSpeed(15);
	crosshairObjVert.setRotationZ(ToRadian(90));

	quad1.init(md3dDevice,1000, DIRT);
	quad1.setPosition(Vector3(0,-1.2,0));

	spinAmount = 0;
	shotTimer = 0;

	bullet.init(md3dDevice,0.1f,BLACK);

	for (int i = 0; i < NUMBERN; i++) {
		bernieShotTimer[i] = 0;
	}
	for(int i = 0; i < MAXBULL; i++)
	{

		GameObject temp;
		temp.init(&bullet,1.0f,Vector3(0,0,0),Vector3(0,0,0),700,1);
		temp.setInActive();

		bullets[i] = temp;
	}
	bernieBullet.init(md3dDevice, 0.1f, RED);
	for(int i = 0; i < MAXBULL; i++)
	{

		GameObject temp;
		temp.init(&bernieBullet,1.0f,Vector3(0,0,0),Vector3(0,0,0),100,1);
		temp.setInActive();

		bernieBullets[i] = temp;
	}
	didShoot = false;

	buildFX();
	buildVertexLayouts();
}

void ColoredCubeApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void ColoredCubeApp::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	audio->run();
	xLine.update(dt);
	yLine.update(dt);
	zLine.update(dt);
	quad1.update(dt);

	shotTimer+=dt;

	for(int i = 0; i < NUMBERN; i++) {
		bernieShotTimer[i]+=dt;
	}

	for(int i = 0; i < NUMBERN; i++) { //assigns each bernie 3 bullets that only shoot once he has come to a stop and fire to the wall
		if(bernies[i].getActiveState() == 1 && bernieShotTimer[i] >= 3.5 && bernies[i].getVelocity() == Vector3(0,0,0) && bernies[i].getPosition().z <= trumpWallObj.getPosition().z + 40) {
			if(bernieBullets[i*3].getActiveState() == 0) {
				bernieBullets[i*3].setActive();
				bernieBullets[i*3].setPosition(Vector3(bernies[i].getPosition().x,bernies[i].getPosition().y+1.3,bernies[i].getPosition().z));
				float dist = 20;
				Vector3 direct = Vector3(0,0,-5);
				bernieBullets[i*3].setVelocity(direct);
				bernieShotTimer[i] = 0;
				audio->playCue(HIT);
			}
			else if(bernieBullets[i*3 + 1].getActiveState() == 0) {
				bernieBullets[i*3 + 1].setActive();
				bernieBullets[i*3 + 1].setPosition(Vector3(bernies[i].getPosition().x,bernies[i].getPosition().y+1.3,bernies[i].getPosition().z));
				float dist = 20;
				Vector3 direct = Vector3(0,0,-5);
				bernieBullets[i*3 + 1].setVelocity(direct);
				bernieShotTimer[i] = 0;
				audio->playCue(HIT);
			}
			else if(bernieBullets[i*3 + 2].getActiveState() == 0) {
				bernieBullets[i*3 + 2].setActive();
				bernieBullets[i*3 + 2].setPosition(Vector3(bernies[i].getPosition().x,bernies[i].getPosition().y+1.3,bernies[i].getPosition().z));
				float dist = 20;
				Vector3 direct = Vector3(0,0,-5);
				bernieBullets[i*3 + 2].setVelocity(direct);
				bernieShotTimer[i] = 0;
				audio->playCue(HIT);
			}
		}

	}

	for(int i = 0; i < MAXBULL; i++)
	{
		if(bullets[i].getActiveState() == 1)
		{
			bullets[i].update(dt);
		}
		if(bernieBullets[i].getActiveState() == 1) 
		{
			bernieBullets[i].update(dt);
			if(bernieBullets[i].getPosition().z <= trumpWallObj.getPosition().z + (trumpWallObj.getWall()->getSize().z * trumpWallObj.getScale()))
			{
				bernieBullets[i].setInActive();
				trumpWallObj.wasHit();
			}
		}
		/*if(bullets[i].getActiveState() == 1 && (bullets[i].getPosition().x > mClientWidth || bullets[i].getPosition().x < 0 || bullets[i].getPosition().y > mClientHeight || bullets[i].getPosition().y < 0))
		{
			bullets[i].setInActive();
		}*/
	}

	if(berniesRemaining)
	{
		berniesRemaining = false;
		for(int i = 0; i <NUMBERN; i++) {
			if(bernies[i].getActiveState()==true)
			{
				berniesRemaining = true;
			}
			if(gameActive)
			{
				if(bernies[i].getActiveState()==false && bernies[i].getInactiveTime()>bernies[i].getSpawnTime())
				{//if bernie is not active and his inactive time has surpassed the spawn time then ready to spawn bernie
					bernies[i].setActive();
					bernies[i].setVelocity(Vector3(bernies[i].getVelocity().x, bernies[i].getVelocity().y, -(int)rand()%BernieNameSpace::MAX_SPEED));
				}
			}
			if(bernies[i].getPosition().z <= bernies[i].getStopPosition())//if Bernie reaches his stop position then set velocity to 0
			{
				bernies[i].setVelocity(Vector3(0,0,0));
			}
			if(bernies[i].getHits() >= BernieNameSpace::MAX_HITS)//if Bernie's hit total surpasses his max hit total he is dead
			{
				bernies[i].setInActive();
				bernies[i].kill();
				bernies[i].setHits(0);
				audio->playCue(PAIN);
			}
			if(bernies[i].didDie()&&(bernies[i].getActiveState()==false))//bernie died and is inactive
			{
				//set to alive
				bernies[i].setToAlive();
				//give him a new random position
				int randPosition = (int)trumpWallObj.getPosition().x + (rand() % (int)trumpWall.getSize().x);
				//give him a new random stop point
				bernies[i].setStopPosition(15 + (rand()%30));

				bernies[i].setPosition(Vector3(randPosition,0,60));

				bernies[i].setSpawnTime(MAX_SPAWN_TIME - (rand()%15));

			}
			bernies[i].update(dt);
		}
	}
	trumpWallObj.update(dt);
	crosshairObjHor.update(dt);
	crosshairObjVert.update(dt);

	if(currentTime >= MAX_LEVEL_TIME)
	{
		gameActive = false;
	}

	currentTime += dt;

	gameTimer += dt;

	D3DXVECTOR3 dir(0, 0, 0);
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		dir.z = 1;
	}
	if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
		dir.z = -1;
	}
	if(GetAsyncKeyState(VK_UP) & 0x8000) {
		dir.y = 1;
	}
	if(GetAsyncKeyState(VK_DOWN) & 0x8000) {
		dir.y = -1;
	}
	D3DXVec3Normalize(&dir, &dir);
	Vector3 lastPosVert = crosshairObjVert.getPosition();
	Vector3 lastPosHor = crosshairObjHor.getPosition();

	crosshairObjHor.setVelocity(crosshairObjHor.getSpeed()*dir);
	crosshairObjVert.setVelocity(crosshairObjVert.getSpeed()*dir);
	if(crosshairObjVert.getPosition().z <= 16.5)
	{
		crosshairObjHor.setPosition(Vector3(lastPosHor.x, lastPosHor.y, lastPosHor.z + .5));
		crosshairObjVert.setPosition(Vector3(lastPosVert.x, lastPosVert.y, lastPosVert.z + .5));
	}
	if(crosshairObjVert.getPosition().z > 29)
	{
		crosshairObjHor.setPosition(Vector3(lastPosHor.x, lastPosHor.y, lastPosHor.z - .5));
		crosshairObjVert.setPosition(Vector3(lastPosVert.x, lastPosVert.y, lastPosVert.z - .5));
	}
	if(crosshairObjVert.getPosition().y > 9.5)
	{
		crosshairObjHor.setPosition(Vector3(lastPosHor.x, lastPosHor.y- .5, lastPosHor.z));
		crosshairObjVert.setPosition(Vector3(lastPosVert.x, lastPosVert.y- .5, lastPosVert.z));
	}
	if(crosshairObjVert.getPosition().y <.3)
	{
		crosshairObjHor.setPosition(Vector3(lastPosHor.x, lastPosHor.y + .5, lastPosHor.z));
		crosshairObjVert.setPosition(Vector3(lastPosVert.x, lastPosVert.y + .5, lastPosVert.z));
	}
	// Restrict the angle mPhi.
	if( mPhi < 0.1f )	mPhi = 0.1f;
	if( mPhi > PI-0.1f)	mPhi = PI-0.1f;

	// Convert Spherical to Cartesian coordinates: mPhi measured from +y
	// and mTheta measured counterclockwise from -z.
	float x =  62.0f;
	float z = 22.0f;
	float y =  7.0f;

	// COLLISION DETECTION HERE
	spinAmount += dt ;
	if (ToRadian(spinAmount*40)>2*PI)
		spinAmount = 0;
	bool collided = false;
	for(int i = 0; i < NUMBERN; i++) {
		for(int j = 0; j < MAXBULL; j++) {
			if(bernies[i].collided(&bullets[j]) && collided == false) {
				if(bullets[j].getActiveState()) {
					audio->playCue(HIT);
					bernies[i].wasHit();
				}
				bullets[j].setInActive();
			}
		}
	}

	// Build the view matrix.
	D3DXVECTOR3 pos(x,y,z);
	D3DXVECTOR3 target(0.0f, 0.0f, 25.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

	Matrix lk;
	D3DXMatrixIdentity(&lk);
	D3DXMatrixLookAtLH(&lk, &pos, &target, &up);
	if(GetAsyncKeyState(VK_SPACE) & 0x8000 && shotTimer >= .3)
	{
		for(int i = 0; i < MAXBULL; i++)
		{
			if(bullets[i].getActiveState() == 0)
			{
				audio->playCue(RIFLE);
				bullets[i].setActive();

				Vector3 start = Vector3(x - 10, y,z);
				bullets[i].setPosition(start);
				Vector3 end = Vector3(x-20, crosshairObjHor.getPosition().y, crosshairObjHor.getPosition().z - .5);


				/*	
				double xs = (target.x - (crosshairObjHor.getPosition().x-50)) * (target.x - (crosshairObjHor.getPosition().x - 50));
				double ys = (target.y - crosshairObjHor.getPosition().y) * (target.y - crosshairObjHor.getPosition().y);
				double zs = (target.z - crosshairObjHor.getPosition().z) * (target.x - crosshairObjHor.getPosition().z);
*/
				//double d = hypot(hypot(target.x - (crosshairObjHor.getPosition().x-50),target.y - crosshairObjHor.getPosition().y)),target.z - crosshairObjHor.getPosition().z);
				Vector3 direct = end - start;
				D3DXVec3Normalize(&direct,&direct);
				bullets[i].setVelocity(direct*100);
				didShoot = false;
				shotTimer = 0;
				break;
			}
		}
	}

	for (int i = 0; i < MAXBULL; i++) {
		if(bernieBullets[i].getActiveState()) {
			bernieBullets[i].update(dt);
		}
	}

	for(int i = 0; i < MAXBULL; i++)
	{
		if(bullets[i].getActiveState() == 1)
		{
			bullets[i].update(dt);
		}
		if(bullets[i].getPosition().x <= -5)
		{
			bullets[i].setInActive();
		}
	}
}

void ColoredCubeApp::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);

	// set some variables for the shader
	int foo[1];
	foo[0] = 0;
	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	//setting the color flip variable in the shader
	mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));

	//draw the lines
	mWVP = xLine.getWorldMatrix()*mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	xLine.setMTech(mTech);
	xLine.draw();

	mWVP = yLine.getWorldMatrix() *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	yLine.setMTech(mTech);
	yLine.draw();

	mWVP = zLine.getWorldMatrix() *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	zLine.setMTech(mTech);
	zLine.draw();

	mWVP = crosshairObjHor.getWorldMatrix() * mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	crosshairObjHor.setMTech(mTech);
	crosshairObjHor.draw();

	mWVP = crosshairObjVert.getWorldMatrix() * mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	crosshairObjVert.setMTech(mTech);
	crosshairObjVert.draw();

	//draw the quad using the "old" method
	//compare how messy this is compared to the objectified geometry classes
	mWVP = quad1.getWorld()*mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	mTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex( p )->Apply(0);
		quad1.draw();
	}

	//draw the boxes
	//mWVP = gameObject1.getWorldMatrix()  *mView*mProj;
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//gameObject1.setMTech(mTech);
	//gameObject1.draw();

	mWVP = trumpWallObj.getWorldMatrix()  *mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	trumpWallObj.setMTech(mTech);
	if(trumpWallObj.getActiveState()==true)trumpWallObj.draw();

	//mWVP = gameObject2.getWorldMatrix()*mView*mProj;
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//gameObject2.setMTech(mTech);
	//gameObject2.draw();


	//mWVP = gameObject3.getWorldMatrix()*mView*mProj;
	//foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//gameObject3.setMTech(mTech);
	//gameObject3.draw();
	//    
	////draw the spinning box
	//if (ToRadian(spinAmount*40) > PI)
	//	foo[0] = 1;
	//else
	//	foo[0] = 0;
	//mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));
	//Matrix spin;
	//RotateY(&spin, ToRadian(spinAmount*40));
	//Matrix translate;
	//Translate(&translate, 5, 0, 0);
	//mWVP = spinner.getWorldMatrix() *translate * spin  *mView*mProj;
	//mfxWVPVar->SetMatrix((float*)&mWVP);
	//spinner.setMTech(mTech);
	//spinner.draw();


	////period motion box
	//
	//if(gameObject1.collided(&gameObject2)||gameObject1.collided(&gameObject3)) gameObject1.setVelocity(-gameObject1.getVelocity());
	//int randBern = rand() % NUMBERN;

	for(int i = 0; i< NUMBERN; i++) {
		mWVP = bernies[i].getWorldMatrix()  *mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		bernies[i].setMTech(mTech);
		if(bernies[i].getActiveState()==true)bernies[i].draw();
	}

	for(int i = 0; i < MAXBULL; i++)
	{
		if(bullets[i].getActiveState())
		{
			mWVP = bullets[i].getWorldMatrix()*mView*mProj;
			mfxWVPVar->SetMatrix((float*)&mWVP);
			bullets[i].setMTech(mTech);
			bullets[i].draw();
		}
	}

	for(int i = 0; i < MAXBULL; i++)
	{
		if(bernieBullets[i].getActiveState())
		{
			mWVP = bernieBullets[i].getWorldMatrix()*mView*mProj;
			mfxWVPVar->SetMatrix((float*)&mWVP);
			bernieBullets[i].setMTech(mTech);
			bernieBullets[i].draw();
		}
	}
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	mSwapChain->Present(0, 0);
}

void ColoredCubeApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	/*#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
	#endif*/

	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"../Games-2-Trump-Tower-Defense/color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("ColorTech");

	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxFLIPVar = mFX->GetVariableByName("flip");

}

void ColoredCubeApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}