#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/Battle.hpp>

namespace Colonization
{
class BattlesProcessor : public Urho3D::Component
{
URHO3D_OBJECT (BattlesProcessor, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <Battle> > battles_;

    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    BattlesProcessor (Urho3D::Context *context);
    virtual ~BattlesProcessor ();
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    int GetBattlesCount () const;
    void UpdateBattlesList ();

    Battle *GetBattleByIndex (int index) const;
    Battle *GetBattleByHash (Urho3D::StringHash battleHash) const;
    Urho3D::PODVector <Urho3D::StringHash> GetCurrentBattlesOfWar (Urho3D::StringHash warHash) const;
};
}
