#pragma once
#include <Colonization/Backend/Diplomacy/DiplomacyRequest.hpp>

namespace Colonization
{
// TODO: Later (after alliances implementation) reimplement to support aliances.
class DiplomacyWarRequest : public DiplomacyRequest
{
URHO3D_OBJECT (DiplomacyWarRequest, DiplomacyRequest)
protected:
    Urho3D::StringHash attacker_;
    Urho3D::StringHash defender_;

public:
    DiplomacyWarRequest (Urho3D::Context *context);
    virtual ~DiplomacyWarRequest ();
    static void RegisterObject (Urho3D::Context *context);

    Urho3D::StringHash GetAttacker () const;
    void SetAttacker (const Urho3D::StringHash &attacker);
    Urho3D::StringHash GetDefender () const;
    void SetDefender (const Urho3D::StringHash &defender);

    virtual void OnAddition ();
    virtual bool UpdatePlayerStatus (Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status);
    virtual bool TimeUpdate (float timeStep);
};
}
