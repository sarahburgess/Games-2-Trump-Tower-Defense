#include "Crosshairs.h"

Crosshairs::Crosshairs()
{
	speed = 10;
	active = true;
	Identity(&world);
	rotX = 0;
	rotY = 0;
	rotZ = 0;
}

Crosshairs::~Crosshairs()
{
}

void Crosshairs::draw()
{
	if (!active)
		return;
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        line->draw();
    }

}

void Crosshairs::init(Line *l, Vector3 pos, float s)
{
	line = l;
	position = pos;
	scale = s;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	velocity = Vector3(0,0,0);
}

void Crosshairs::update(float dt)
{
	position += velocity*dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);
	Matrix rotXM, rotYM, rotZM, transM;
	RotateX(&rotXM, rotX);
	RotateY(&rotYM, rotY);
	RotateZ(&rotZM, rotZ); 
	Translate(&transM, position.x, position.y, position.z);
	world = rotXM * rotYM * rotZM * transM;
}

