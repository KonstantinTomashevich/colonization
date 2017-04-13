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

    void Init (Urho3D::StringHash attacker, Urho3D::StringHash defender);
    virtual void UpdatePlayerStatus (DiplomacyActionsExecutor *diplomacyActionsExecutor, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status);
    virtual void TimeUpdate (DiplomacyActionsExecutor *diplomacyActionsExecutor, float timeStep);
};
}
