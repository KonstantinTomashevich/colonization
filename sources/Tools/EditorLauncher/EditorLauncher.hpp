#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/AngelScript/ScriptFile.h>

class EditorLauncher : public Urho3D::Application
{
URHO3D_OBJECT (EditorLauncher, Application)
public:
    explicit EditorLauncher (Urho3D::Context* context);
    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();

private:
    void HandleScriptReloadStarted (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleScriptReloadFinished (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleScriptReloadFailed (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Urho3D::String scriptFileName_;
    Urho3D::SharedPtr <Urho3D::ScriptFile> scriptFile_;
};
