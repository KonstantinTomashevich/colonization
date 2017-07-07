#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/Battle/Battle.hpp>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
class DiplomacyProcessor;
class UnitsManager;
class Player;
class DiplomacyWar;

// TODO: New battles created only when unit changes position. But battles will be created if new war declared too!
//@ASBindGen Function ReturnHandleArray
Urho3D::PODVector <Battle *> GetBattlesInDistrict (Urho3D::Scene *scene, Urho3D::StringHash districtHash);
class BattlesProcessor : public Urho3D::Component
{
URHO3D_OBJECT (BattlesProcessor, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <Battle> > battles_;

    void OnUnitPositionChangedOrCreated (Urho3D::VariantMap &eventData);
    bool AddUnitToBattleIfNeeded (Unit *unit, District *unitPosition, Player *unitPlayer,
                                  DiplomacyProcessor *diplomacyProcessor, UnitsManager *unitsManager);
    bool CreateNewBattleIfNeeded (Unit *unit, District *district, Player *unitPlayer,
                                  DiplomacyProcessor *diplomacyProcessor, UnitsManager *unitsManager);

    void SortAttackersAndDefendersInBattle (DiplomacyWar *war, bool isNewUnitPlayerAttackerInWar,
                                            Urho3D::Vector <Urho3D::StringHash> &playersList,
                                            Urho3D::PODVector <Urho3D::StringHash> &willBeAttackers,
                                            Urho3D::PODVector <Urho3D::StringHash> &willBeDefenders);

    void InitNewBattle (Battle *battle, Urho3D::HashMap <Urho3D::StringHash, Urho3D::PODVector <Unit *> > &unitsInDistrict,
                        Urho3D::PODVector <Urho3D::StringHash> &willBeAttackers,
                        Urho3D::PODVector <Urho3D::StringHash> &willBeDefenders);

    /// Returns false if battle was ended.
    bool ProcessBattle (Battle *battle, float timeStep);
    void ReconstructBattleAttackersAndDefenders (UnitsManager *unitsManager, Battle *battle,
                                                 Urho3D::PODVector <Unit *> &attackers,
                                                 Urho3D::PODVector <Unit *> &defenders);
    float CalculateUnitsAttackForce (Urho3D::PODVector <Unit *> &units, GameConfiguration *configuration, bool isNavalBattle);

    void ApplyDamage (Battle *battle, GameConfiguration *configuration, float fullDamage,
                      Urho3D::PODVector <Unit *> &units, bool isAttackers, float damagedUnitUpdatePoints);
    Battle *CreateBattle (Urho3D::StringHash warHash, Urho3D::StringHash districtHash);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    explicit BattlesProcessor (Urho3D::Context *context);
    virtual ~BattlesProcessor ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnUnitCreated (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnUnitPositionChanged (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnTradersUnitLossesGold (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    int GetBattlesCount () const;
    void UpdateBattlesList ();
    Battle *GetBattleByIndex (int index) const;
    Battle *GetBattleByHash (Urho3D::StringHash battleHash) const;
    Urho3D::PODVector <Urho3D::StringHash> GetCurrentBattlesOfWar (Urho3D::StringHash warHash) const;
    Urho3D::PODVector <Urho3D::StringHash> GetCurrentBattlesInDistrict (Urho3D::StringHash districtHash) const;
};
}
