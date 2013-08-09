#pragma once

#include "AObject.h"

class ASystemInfo;

class BRenderer;
class BViewport;
class UWorld;

enum EPlatform {
    Platform_Windows_x86
};

struct TApplicationInfo {
public:
    TApplicationInfo() :
        Platform(Platform_Windows_x86) {
    }

    EPlatform Platform;
};

class AApplication: public AObject {
    DECLARE_CLASS(AApplication,CLASS_Abstract)
public:
    AApplication(void);
    virtual ~AApplication();

    ASystemInfo* SystemInfo;

    BRenderer *m_pRenderer;
	TArray<BViewport*> Viewports;

    UWorld* m_pWorld;

    class CBox* Box;						// ¹æ
    class CDirectionalLight* DirectionalLight;

    bool bQuit;
    bool bRenderThreadQuit;

    virtual void SetMousePos(float X, float Y, bool isRatio = false) = 0;

    virtual bool CreateApp(TApplicationInfo& Info) = 0;
    virtual void Do() = 0;
    virtual bool DestroyApp() = 0;
    virtual void Initialize() = 0;
};
