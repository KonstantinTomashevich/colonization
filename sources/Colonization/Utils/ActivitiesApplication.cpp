#include <Colonization/BuildConfiguration.hpp>
#include "ActivitiesApplication.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/AngelScript/Script.h>

#include <Launcher/AngelScriptBinders/BindActivitiesApplication.hpp>
#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/BindMap.hpp>
#include <Colonization/AngelScriptBinders/BindNetworkMessageType.hpp>
#include <Colonization/AngelScriptBinders/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindMainMenuActivity.hpp>
#include <Colonization/AngelScriptBinders/BindIngamePlayerActivity.hpp>
#include <Colonization/AngelScriptBinders/BindUnit.hpp>
#include <Colonization/AngelScriptBinders/BindPlayerActionType.hpp>
#include <Colonization/AngelScriptBinders/BindInternalTradeArea.hpp>
#include <Colonization/AngelScriptBinders/BindPlayerInfo.hpp>

#include <Colonization/Backend/ColoniesManager.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/Player.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/PlayerInfo.hpp>
#include <Colonization/Core/Unit.hpp>
#include <Colonization/Activities/MainMenuActivity.hpp>

namespace Colonization
{
ActivitiesApplication::ActivitiesApplication (Urho3D::Context *context) : Urho3D::Application (context),
    currentActivities_ (),
    activitiesToSetup_ (),
    activitiesToStop_ ()
{

}

ActivitiesApplication::~ActivitiesApplication ()
{

}

void ActivitiesApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Colonization.log";
    engineParameters_ ["WindowTitle"] = "Colonization";
}

void ActivitiesApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Init random
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());

    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (ActivitiesApplication, Update));
    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Start ();

    // Register objects.
    ColoniesManager::RegisterObject (context_);
    MessagesHandler::RegisterObject (context_);
    PlayersManager::RegisterObject (context_);
    TradeProcessor::RegisterObject (context_);
    UnitsManager::RegisterObject (context_);

    District::RegisterObject (context_);
    InternalTradeArea::RegisterObject (context_);
    Map::RegisterObject (context_);
    PlayerInfo::RegisterObject (context_);
    Unit::RegisterObject (context_);

    // Register AngelScript subsystem and run bindings.
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    BindActivity (script);
    BindActivitiesApplication (script);
    BindDistrict (script);
    BindMap (script);
    BindNetworkMessageType (script);
    BindHostActivity (script);
    BindMainMenuActivity (script);
    BindIngamePlayerActivity (script);
    BindUnit (script);
    BindPlayerActionType (script);
    BindInternalTradeArea (script);
    BindPlayerInfo (script);

    Urho3D::SharedPtr <MainMenuActivity> mainMenuActivity (new MainMenuActivity (context_));
    SetupActivityNextFrame (mainMenuActivity);
}

void ActivitiesApplication::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    if (!activitiesToStop_.Empty ())
        while (!activitiesToStop_.Empty ())
        {
            Urho3D::SharedPtr <Activity> activity = activitiesToStop_.Front ();
            activitiesToStop_.Remove (activity);
            assert (activity.NotNull ());
            currentActivities_.Remove (activity);
            activity->Stop ();
        }

    if (!activitiesToSetup_.Empty ())
        while (!activitiesToSetup_.Empty ())
        {
            Urho3D::SharedPtr <Activity> activity = activitiesToSetup_.Front ();
            activitiesToSetup_.Remove (activity);
            assert (activity.NotNull ());
            currentActivities_.Push (activity);
            activity->SetApplication (this);
            activity->Start ();
        }

    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Update (timeStep);
}

void ActivitiesApplication::Stop ()
{
    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Stop ();
}

void ActivitiesApplication::SetupActivityNextFrame (Activity *activity)
{
    assert (activity);
    activitiesToSetup_.Push (Urho3D::SharedPtr <Activity> (activity));
}

void ActivitiesApplication::StopActivityNextFrame (Activity *activity)
{
    assert (activity);
    activitiesToStop_.Push (Urho3D::SharedPtr <Activity> (activity));
}

int ActivitiesApplication::GetActivitiesCount ()
{
    return currentActivities_.Size ();
}

Activity *ActivitiesApplication::GetActivityByIndex (int index)
{
    assert (index < currentActivities_.Size ());
    return currentActivities_.At (index);
}
}

