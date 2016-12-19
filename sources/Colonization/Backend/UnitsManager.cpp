#include <Colonization/BuildConfiguration.hpp>
#include "UnitsManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Backend/SceneManager.hpp>

namespace Colonization
{
void UnitsManager::SettleColonizator (ColonizatorsUnit *unit)
{
    assert (unit);
    assert (unit->position_);

    District *colony = unit->position_;
    if (!colony->hasColony_)
    {
        colony->hasColony_ = true;
        colony->colonyOwnerName_ = unit->ownerPlayer_;
    }

    if (colony->hasColony_ && colony->colonyOwnerName_ != unit->ownerPlayer_)
    {
        Urho3D::Log::Write (Urho3D::LOG_WARNING, "Can't settle colonizator of " + unit->ownerPlayer_ +
                            " in " + colony->name_ + ". Because there is a colony of " + colony->colonyOwnerName_ + "!");
        unitsContainer_->RemoveAndDeleteUnit (unit);
    }
    else
    {
        float mansPercent = Urho3D::Random (0.4f, 0.6f);
        colony->mansCount_ += unit->colonizatorsCount_ * 1.0f * mansPercent;
        colony->womenCount_ += unit->colonizatorsCount_ * 1.0f * (1.0f - mansPercent);
        unitsContainer_->RemoveAndDeleteUnit (unit);
    }
}

UnitsManager::UnitsManager (Urho3D::Context *context) : Urho3D::Object (context),
    unitsContainer_ (new UnitsContainer (context))
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (UnitsManager, Update));
}

UnitsManager::~UnitsManager ()
{
    delete unitsContainer_;
}

void UnitsManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    float sailSpeed = context_->GetGlobalVar ("sailSpeed").GetFloat ();
    float marchSpeed = context_->GetGlobalVar ("marchSpeed").GetFloat ();
    float embarkationSpeed = context_->GetGlobalVar ("embarkationSpeed").GetFloat ();
    float disembarkationSpeed = context_->GetGlobalVar ("disembarkationSpeed").GetFloat ();

    for (int index = 0; index < unitsContainer_->GetUnitsCount (); index++)
    {
        Unit *unit = unitsContainer_->GetUnitByIndex (index);
        if (!unit->way_.Empty ())
        {
            if (unit->position_ == unit->way_.At (0))
            {
                unit->way_.Remove (unit->way_.At (0));
                unit->wayToNextDistrictProgressInPercents_ = 0.0f;

                if (unit->way_.Empty () && unit->unitType_ == UNIT_COLONIZATORS)
                    SettleColonizator ( (ColonizatorsUnit *) unit);
            }

            float distance = (unit->position_->unitPosition_ - unit->way_.At (0)->unitPosition_).Length ();
            float speed;

            if (unit->position_->isSea_ && unit->way_.At (0)->isSea_)
                speed = sailSpeed;
            else if (!unit->position_->isSea_ && !unit->way_.At (0)->isSea_)
                speed = marchSpeed;
            else if (!unit->position_->isSea_ && unit->way_.At (0)->isSea_)
                speed = embarkationSpeed;
            else if (unit->position_->isSea_ && !unit->way_.At (0)->isSea_)
                speed = disembarkationSpeed;

            float addition = (100.0f * speed * timeStep) / distance;
            unit->wayToNextDistrictProgressInPercents_ += addition;

            if (unit->wayToNextDistrictProgressInPercents_ >= 100.0f)
            {
                unit->position_ = unit->way_.At (0);
                unit->way_.Remove (unit->way_.At (0));
                unit->wayToNextDistrictProgressInPercents_ = 0.0f;

                if (unit->way_.Empty () && unit->unitType_ == UNIT_COLONIZATORS)
                    SettleColonizator ( (ColonizatorsUnit *) unit);
            }
        }
    }

    // TODO: To be continued...
}

UnitsContainer *UnitsManager::GetUnitsContainer ()
{
    return unitsContainer_;
}
}

