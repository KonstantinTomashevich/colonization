#pragma once
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/AngelScript/ScriptInstance.h>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
class IngameClientActivity : public Activity
{
URHO3D_OBJECT (IngameClientActivity, Activity)
protected:
    Urho3D::Scene *scene_;
    Urho3D::ScriptInstance *angelScript_;

    Urho3D::String serverAdress_;
    unsigned short serverPort_;
    bool isAdmin_;
    Urho3D::String playerName_;
    Urho3D::Color playerColor_;
public:
    //@ASBindGen Constructor UseUrho3DScriptContext_arg0
    IngameClientActivity (Urho3D::Context *context);
    virtual ~IngameClientActivity ();

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();

    //@ASBindGen Function OverrideName=get_serverAdress
    Urho3D::String GetServerAdress ();
    //@ASBindGen Function OverrideName=set_serverAdress
    void SetServerAdress (Urho3D::String serverAdress);

    //@ASBindGen Function OverrideName=get_serverPort
    unsigned short GetServerPort ();
    //@ASBindGen Function OverrideName=set_serverPort
    void SetServerPort (unsigned short serverPort);

    //@ASBindGen Function OverrideName=get_isAdmin
    bool IsAdmin () const;
    //@ASBindGen Function OverrideName=set_isAdmin
    void SetIsAdmin (bool isAdmin);

    //@ASBindGen Function OverrideName=get_playerName
    Urho3D::String GetPlayerName ();
    //@ASBindGen Function OverrideName=set_playerName
    void SetPlayerName (Urho3D::String playerName);

    //@ASBindGen Function OverrideName=get_playerColor
    Urho3D::Color GetPlayerColor ();
    //@ASBindGen Function OverrideName=set_playerColor
    void SetPlayerColor (Urho3D::Color playerColor);
};
}
