
#include "WallObject.h"

WallObject::WallObject()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
}

WallObject::~WallObject()
{
	wall = NULL;
}

void WallObject::draw()
{
	if (!active)
		return;
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        wall->draw();
    }
		/*box->draw();*/
}

void WallObject::init(Wall *w, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	wall = w;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	velocity = vel;
	speed = sp;
	scale = w->scale;
	radiusSquared = radius * radius;
	hits = 0;
}

void WallObject::update(float dt)
{
	if(hits>=WallNameSpace::MAX_HITS)
	{
		active = false;
	}
	position += velocity*dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);
}

bool WallObject::collided(GameObject *gameObject)
{
	Vector3 diff = position - gameObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}