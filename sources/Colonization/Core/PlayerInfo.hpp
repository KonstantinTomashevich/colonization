#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class PlayerInfo : public Urho3D::Component
{
URHO3D_OBJECT (PlayerInfo, Component)
protected:
    Urho3D::String name_;
    float points_;

public:
    PlayerInfo (Urho3D::Context *context);
    virtual ~PlayerInfo ();
    static void RegisterObject (Urho3D::Context *context);

    Urho3D::String GetName ();
    void SetName (Urho3D::String name);

    float GetPoints ();
    void SetPoints (float points);
};
}
