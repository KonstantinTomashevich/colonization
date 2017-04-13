#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Backend/Diplomacy/DiplomacyActionsExecutor.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyRequest.hpp>
#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>

namespace Colonization
{
class DiplomacyProcessor : public Urho3D::Component
{
URHO3D_OBJECT (DiplomacyProcessor, Component)
protected:
    DiplomacyActionsExecutor *diplomacyActionsExexutor_;
    Urho3D::Vector <Urho3D::SharedPtr <DiplomacyRequest> > requests_;
    Urho3D::Vector <Urho3D::SharedPtr <DiplomacyWar> > wars_;

    void UpdateDiplomacyRequests (float timeStep);
    Urho3D::Vector <Urho3D::SharedPtr <DiplomacyRequest> >::Iterator GetDiplomacyRequestIteratorById (unsigned requestId);
    unsigned GetFreeDiplomacyRequestId ();
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    DiplomacyProcessor (Urho3D::Context *context);
    virtual ~DiplomacyProcessor ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    void AddDiplomacyRequest (DiplomacyRequest *request);
    void UpdateDiplomacyRequestPlayerStatus (unsigned requestId, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status);

    int GetWarsCount () const;
    void UpdateWarsList ();
    DiplomacyWar *CreateWar ();
    DiplomacyWar *GetWarByIndex (int index);
    DiplomacyWar *GetWarByHash (Urho3D::StringHash hash);
};
}
