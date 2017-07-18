#pragma once
#include <Colonization/Backend/Diplomacy/DiplomacyRequest.hpp>

namespace Colonization
{
// TODO: Later (after alliances implementation) reimplement to support full alliance peace and separate peace.
class DiplomacyPeaceRequest : public DiplomacyRequest
{
URHO3D_OBJECT (DiplomacyPeaceRequest, DiplomacyRequest)
public:
    explicit DiplomacyPeaceRequest (Urho3D::Context *context);
    virtual ~DiplomacyPeaceRequest ();
    static void RegisterObject (Urho3D::Context *context);

    Urho3D::StringHash GetWarHash () const;
    void SetWarHash (const Urho3D::StringHash &warHash);

    Urho3D::StringHash GetPeaceRequester () const;
    void SetPeaceRequester (const Urho3D::StringHash &peaceRequester);

    Urho3D::StringHash GetEnemy () const;
    void SetEnemy (const Urho3D::StringHash &enemy);

    float GetTimeUntilAutodecline () const;
    void SetTimeUntilAutodecline (float untilAutodecline);

    virtual void OnAddition ();
    virtual bool UpdatePlayerStatus (Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status);
    virtual bool TimeUpdate (float timeStep);
    virtual Urho3D::String GetRequestTypeTag () const;

private:
    Urho3D::StringHash warHash_;
    Urho3D::StringHash peaceRequester_;
    Urho3D::StringHash enemy_;
    float untilAutodecline_;

    void ProcessAcceptedResult ();
    void ProcessDeclinedResult ();
};
}
