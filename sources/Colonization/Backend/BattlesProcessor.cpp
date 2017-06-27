#include <Colonization/BuildConfiguration.hpp>
#include "BattlesProcessor.hpp"
#include <Urho3D/Core/Context.h>

#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/Player/Player.hpp>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Unit/Unit.hpp>
#include <Colonization/Core/Unit/UnitEvents.hpp>

#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{

bool BattlesProcessor::AddUnitToBattleIfNeeded (Unit *unit, District *unitPosition, Player *unitPlayer,
                                                DiplomacyProcessor *diplomacyProcessor, UnitsManager *unitsManager)
{
    Urho3D::PODVector <Urho3D::StringHash> battlesInDistrict = GetCurrentBattlesInDistrict (unitPosition->GetHash ());
    Urho3D::PODVector <Urho3D::StringHash> warsOfPlayer = diplomacyProcessor->GetWarsOfPlayer (
                Urho3D::StringHash (unitPlayer->GetName ()));

    if (!battlesInDistrict.Empty () && !warsOfPlayer.Empty ())
    {
        for (int index = 0; index < battlesInDistrict.Size (); index++)
        {
            Battle *battle = GetBattleByHash (battlesInDistrict.At (index));
            if (warsOfPlayer.Contains (battle->GetWarHash ()))
            {
                DiplomacyWar *war = diplomacyProcessor->GetWarByHash (battle->GetWarHash ());
                assert (war);
                bool isAttackerInWar = war->IsAttacker (Urho3D::StringHash (unitPlayer->GetName ()));

                Unit *firstAttackersUnit = unitsManager->GetUnitByHash (battle->GetAttackerUnitHashByIndex (0));
                bool isFirstAttackerUnitAttackerInWar = war->IsAttacker (
                            Urho3D::StringHash (firstAttackersUnit->GetOwnerPlayerName ()));

                if (isAttackerInWar == isFirstAttackerUnitAttackerInWar)
                {
                    battle->AddAttackerUnitHash (unit->GetHash ());
                }
                else
                {
                    battle->AddDefenderUnitHash (unit->GetHash ());
                }

                unit->SetIsInBattle (true);
                NetworkUpdateCounter *counter = unit->GetNode ()->GetComponent <NetworkUpdateCounter> ();
                if (!counter)
                {
                    counter = CreateNetworkUpdateCounterForComponent (unit);
                }
                counter->AddUpdatePoints (100.0f);
                return true;
            }
        }
    }
    return false;
}

void BattlesProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (BattlesProcessor, Update));
    SubscribeToEvent (scene, EVENT_UNIT_CREATED, URHO3D_HANDLER (BattlesProcessor, OnUnitPositionChangedOrCreated));
    SubscribeToEvent (scene, EVENT_UNIT_POSITION_CHANGED, URHO3D_HANDLER (BattlesProcessor, OnUnitPositionChangedOrCreated));
}

BattlesProcessor::BattlesProcessor (Urho3D::Context *context) : Urho3D::Component (context),
    battles_ ()
{

}

BattlesProcessor::~BattlesProcessor ()
{

}

void BattlesProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <BattlesProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void BattlesProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{

}

void BattlesProcessor::OnUnitPositionChangedOrCreated (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    UnitsManager *unitsManager = node_->GetScene ()->GetChild ("units")->GetComponent <UnitsManager> ();
    DiplomacyProcessor *diplomacyProcessor = node_->GetScene ()->GetChild ("diplomacy")->GetComponent <DiplomacyProcessor> ();
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();

    Unit *unit = unitsManager->GetUnitByHash (eventData [UnitPositionChanged::UNIT_HASH].GetStringHash ());
    assert (unit);
    District *district = map->GetDistrictByHash (unit->GetPositionHash ());
    assert (district);
    Player *player = playersManager->GetPlayerByNameHash (Urho3D::StringHash (unit->GetOwnerPlayerName ()));

    AddUnitToBattleIfNeeded (unit, district, player, diplomacyProcessor, unitsManager);
}

int BattlesProcessor::GetBattlesCount () const
{
    return battles_.Size ();
}

void BattlesProcessor::UpdateBattlesList ()
{
    // Reload battles array from child nodes.
    assert (node_);
    battles_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> battlesNodes;
    node_->GetChildrenWithTag (battlesNodes, TAG_BATTLE);
    for (int index = 0; index < battlesNodes.Size (); index++)
    {
        Urho3D::Node *battleNode = battlesNodes.At (index);
        if (battleNode->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            Battle *battle = battleNode->GetComponent <Battle> ();
            if (battle)
            {
                battles_.Push (Urho3D::SharedPtr <Battle> (battle));
            }
        }
    }
}

Battle *BattlesProcessor::GetBattleByIndex (int index) const
{
    assert (index < battles_.Size ());
    return battles_.At (index);
}

Battle *BattlesProcessor::GetBattleByHash (Urho3D::StringHash battleHash) const
{
    for (int index = 0; index < battles_.Size (); index++)
    {
        Battle *battle = battles_.At (index);
        if (battle->GetHash () == battleHash)
        {
            return battle;
        }
    }
    return 0;
}

Urho3D::PODVector <Urho3D::StringHash> BattlesProcessor::GetCurrentBattlesOfWar (Urho3D::StringHash warHash) const
{
    Urho3D::PODVector <Urho3D::StringHash> currentBattlesOfWar;
    for (int index = 0; index < battles_.Size (); index++)
    {
        Battle *battle = battles_.At (index);
        if (battle->GetWarHash () == warHash)
        {
            currentBattlesOfWar.Push (battle->GetHash ());
        }
    }
    return currentBattlesOfWar;
}

Urho3D::PODVector <Urho3D::StringHash> BattlesProcessor::GetCurrentBattlesInDistrict (Urho3D::StringHash districtHash) const
{
    Urho3D::PODVector <Urho3D::StringHash> currentBattlesInDistrict;
    for (int index = 0; index < battles_.Size (); index++)
    {
        Battle *battle = battles_.At (index);
        if (battle->GetDistrictHash () == districtHash)
        {
            currentBattlesInDistrict.Push (battle->GetHash ());
        }
    }
    return currentBattlesInDistrict;
}
}
