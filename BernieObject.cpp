
#include "BernieObject.h"

BernieObject::BernieObject()
{
	radius = 0;
	speed = 10;
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
	health = 1;
}

void BernieObject::update(float dt)
{
	if(health <= 0)
	{
		setInActive();
	}
	position += velocity*dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);

}

bool BernieObject::collided(GameObject *gameObject)
{
	Vector3 diff = position - gameObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}

void BernieObject::reduceHealth(int damage)
{
	health-=damage;
}