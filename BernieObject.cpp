
#include "BernieObject.h"

BernieObject::BernieObject()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
}

BernieObject::~BernieObject()
{
	bernie = NULL;
}

void BernieObject::draw()
{
	if (!active)
		return;
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        bernie->draw();
    }
		/*bernie->draw();*/
}

void BernieObject::init(Bernie *b, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	bernie = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
}

void BernieObject::update(float dt)
{
	position += velocity*dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);

}

bool BernieObject::collided(BernieObject *BernieObject)
{
	Vector3 diff = position - BernieObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + BernieObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}