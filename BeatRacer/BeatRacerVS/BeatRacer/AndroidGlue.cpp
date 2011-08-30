#include "stdafx.h"
//#include <jni.h>
#include "CBeatTrack.h"
#include "CGraphicInterface_Android.h"

CBeatTrack* Track = 0;
CGraphicInterface* GI = 0;

void Java_com_game_BeatRacer_Track_Start( /*JNIEnv*  env*/ )
{
	if(Track)
	{
		Track->Start();
	}
}

void Java_com_game_BeatRacer_Track_Pause( /*JNIEnv*  env*/ )
{
	if(Track)
	{
		Track->Pause();
	}
}

void Java_com_game_BeatRacer_Track_Resume( /*JNIEnv*  env*/ )
{
	if(Track)
	{
		Track->Resume();
	}
}

void Java_com_game_BeatRacer_Track_Advance( /*JNIEnv*  env, jint dTime*/ )
{
	if(Track)
	{
		Track->Advance(0);
		Track->Draw(GI);
	}
}

void Java_com_game_BeatRacer_Track_Initialize( /*JNIEnv*  env*/ )
{
	Track = new CBeatTrack();
}

void Java_com_game_BeatRacer_Track_Destroy( /*JNIEnv*  env*/ )
{
	if(Track)
	{
		delete Track;
	}
}

int Java_com_game_BeatRacer_Track_Hit( /*JNIEnv*  env, jint hitType*/ )
{
	if(Track)
	{
		return Track->Hit(HitType_Down);
	}
	return HitResult_None;
}

void Java_com_game_BeatRacer_GI_Initialize( /*JNIEnv*  env*/ )
{
	GI = new CGraphicInterface_Android();
}

void Java_com_game_BeatRacer_GI_Destroy( /*JNIEnv*  env*/ )
{
	if(GI)
	{
		delete GI;
	}
}