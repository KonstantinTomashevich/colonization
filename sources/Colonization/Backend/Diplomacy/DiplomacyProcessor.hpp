#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Backend/Diplomacy/DiplomacyRequest.hpp>
#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>

namespace Colonization
{
class DiplomacyProcessor : public Urho3D::Component
{
URHO3D_OBJECT (DiplomacyProcessor, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <DiplomacyWar> > wars_;

    void UpdateDiplomacyRequests (float timeStep);
    Urho3D::Node *GetDiplomacyRequestNodeById (unsigned requestId);
    unsigned GetFreeDiplomacyRequestId ();
    Urho3D::Node *GetOrCreateRequestsNode ();
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    explicit DiplomacyProcessor (Urho3D::Context *context);
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
    bool RemoveWarByHash (Urho3D::StringHash hash);
    Urho3D::PODVector <Urho3D::StringHash> GetWarsOfPlayer (Urho3D::StringHash playerNameHash);
};
}
