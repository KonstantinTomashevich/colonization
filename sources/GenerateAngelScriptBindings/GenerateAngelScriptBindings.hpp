#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <AngelScript/angelscript.h>

class GenerateAngelScriptBindings : public Urho3D::Application
{
URHO3D_OBJECT (GenerateAngelScriptBindings, Application)
public:
    explicit GenerateAngelScriptBindings (Urho3D::Context *context);
    virtual ~GenerateAngelScriptBindings ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
