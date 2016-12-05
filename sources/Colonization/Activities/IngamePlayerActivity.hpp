#pragma once
#include <Colonization/Utils/Activity.hpp>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/AngelScript/ScriptInstance.h>

namespace Colonization
{
class IngamePlayerActivity : public Activity
{
URHO3D_OBJECT (IngamePlayerActivity, Activity)
protected:
    Urho3D::Scene *scene_;
    Urho3D::ScriptInstance *angelScript_;

    Urho3D::String serverAdress_;
    unsigned short serverPort_;
    Urho3D::String playerName_;
public:
    IngamePlayerActivity (Urho3D::Context *context);
    virtual ~IngamePlayerActivity ();

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();

    Urho3D::String GetServerAdress ();
    void SetServerAdress (Urho3D::String serverAdress);

    unsigned short GetServerPort ();
    void SetServerPort (unsigned short serverPort);

    Urho3D::String GetPlayerName ();
    void SetPlayerName (Urho3D::String playerName);
};
}
