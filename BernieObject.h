
#ifndef BernieObject_H
#define BernieObject_H

//#include "d3dUtil.h"
#include "d3dUtil.h"

#include "Bernie.h"
#include "GameObject.h"
#include "constants.h"

class BernieObject :public GameObject
{
public:

	BernieObject();
	~BernieObject();

	void init(Bernie *b, float r, Vector3 pos, Vector3 vel, float sp, float s);
	void draw();
	void update(float dt);

	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setVelocity (Vector3 vel) {velocity = vel;}
	Vector3 getVelocity() {return velocity;}
	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}
	void setRadius(float r) {radius = r; radiusSquared = (scale*r)*(scale*r);}
	float getRadiusSquare() {return radiusSquared;}
	float getRadius() {return radius;}
	Matrix getWorldMatrix() {return world;}
	void setScale(float s) {scale = s; radiusSquared = (s*radius)*(s*radius);}
	float getScale() {return scale;}
	void setActive() {active = true;}
	void setInActive() {active = false;}
	bool getActiveState() {return active;}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	void setStopPosition(int sp){stopPosition = sp;};
	int getStopPosition(){return stopPosition;}
	bool collided(BernieObject *BernieObject);

	bool collided(GameObject *gameObject);

	void wasHit(){hits++;}

	void kill(){died = true;};

	bool didDie(){return died;}
	void setToAlive(){died = false;}

	float getInactiveTime(){return inactiveTime;};
	void setSpawnTime(float st){spawnTime = st;};

	float getSpawnTime(){return spawnTime;};

	float getHits(){return hits;};

	void setHits(int h){hits = h;};


private:
	Bernie *bernie; 
	Vector3 position;
	Vector3 velocity;
	float spawnTime;
	float speed;
	float inactiveTime;
	int stopPosition;
	float radius;
	float radiusSquared;
	bool active;
	bool died;
	Matrix world;
	float scale;
	ID3D10EffectTechnique* mTech;
	int hits;
};


#endif
