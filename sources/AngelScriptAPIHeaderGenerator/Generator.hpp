#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <AngelScript/angelscript.h>

class Generator : public Urho3D::Application
{
URHO3D_OBJECT (Generator, Application)
public:
    Generator (Urho3D::Context *context);
    virtual ~Generator ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
