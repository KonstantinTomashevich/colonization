#pragma once
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/Player.hpp>

namespace Colonization
{
class PlayersPointsCalculator : public Urho3D::Component
{
URHO3D_OBJECT (PlayersPointsCalculator, Component)
protected:
    float CalculateColonyPoints (District *district, GameConfiguration *configuration);
    float CalculateUnitPoints (Unit *unit, GameConfiguration *configuration);
    void UpdatePlayerPoints (Player *player, GameConfiguration *configuration, Map *map, UnitsManager *unitsManager);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    PlayersPointsCalculator (Urho3D::Context *context);
    virtual ~PlayersPointsCalculator ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};
}
