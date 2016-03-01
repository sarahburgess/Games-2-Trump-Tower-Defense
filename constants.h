#ifndef Constants_H
#define Constants_H

//Defines to make life easier

	#define Vector3 D3DXVECTOR3
#define Matrix D3DXMATRIX
#define Identity D3DXMatrixIdentity
#define Translate D3DXMatrixTranslation
#define RotateX D3DXMatrixRotationX
#define RotateY D3DXMatrixRotationY
#define RotateZ D3DXMatrixRotationZ
#define ToRadian D3DXToRadian
#define ToDegree D3DXToDegree

const int MAXBULL = 30;
const int NUMBERN = 10;
const float MAX_SPAWN_TIME = 25.0f;

const float MAX_LEVEL_TIME = 1000000.0f;

const char WAVE_BANK[]  = "..\\Games-2-Trump-Tower-Defense\\audio\\Win\\wb.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "..\\Games-2-Trump-Tower-Defense\\audio\\Win\\sb.xsb";
//LPCWSTR  SOUND_BANK[] = "audio\\Win\\SoundsExample1.xsb";
// audio cues
const char RIFLE[] = "rifle";
const char HIT[]   = "hit";
const char BEEP[]  = "beep1";
const char PAIN[]  = "pain";

const char HYMN[]  = "hymn";
#endif