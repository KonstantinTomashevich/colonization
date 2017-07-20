#include "TestApplication.hpp"
#include "DiplomacyInternalPlayer.hpp"

#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyWarRequest.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyPeaceRequest.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyOfferType.hpp>

#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (TestApplication)
TestApplication::TestApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestApplication::~TestApplication ()
{

}

void TestApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Test.log";
    engineParameters_ ["WindowTitle"] = "Test";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void TestApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    scene->CreateComponent <Colonization::GameConfiguration> ()->ResetToDefault ();

    scene->CreateComponent <Colonization::MessagesHandler> ();
    scene->CreateComponent <Colonization::NetworkUpdateSmoother> ();
    Colonization::PlayersManager *playersManager  = scene->CreateChild ("players")->CreateComponent <Colonization::PlayersManager> ();
    Colonization::DiplomacyProcessor *diplomacyProcessor = scene->CreateChild ("diplomacy")->CreateComponent <Colonization::DiplomacyProcessor> ();

    DiplomacyInternalPlayer *firstPlayer = new DiplomacyInternalPlayer (context_, "First", Urho3D::Color::BLACK, scene);
    DiplomacyInternalPlayer *secondPlayer = new DiplomacyInternalPlayer (context_, "Second", Urho3D::Color::WHITE, scene);

    playersManager->AddInternalPlayer (firstPlayer);
    playersManager->AddInternalPlayer (secondPlayer);

    Urho3D::VariantMap updateEventData;
    updateEventData [Urho3D::SceneUpdate::P_TIMESTEP] = Urho3D::Variant (1.0f);
    {
        Urho3D::SharedPtr <Colonization::DiplomacyWarRequest>
                warRequest (new Colonization::DiplomacyWarRequest (context_));
        warRequest->SetAttacker (Urho3D::StringHash (firstPlayer->GetName ()));
        warRequest->SetDefender (Urho3D::StringHash (secondPlayer->GetName ()));
        diplomacyProcessor->AddDiplomacyRequest (warRequest);

        diplomacyProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        playersManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);

        if (diplomacyProcessor->GetWarsCount () != 1)
        {
            ErrorExit ("Expected 1 war at the moment! But got " + Urho3D::String (diplomacyProcessor->GetWarsCount ()) + "!");
        }

        Colonization::DiplomacyWar *war = diplomacyProcessor->GetWarByIndex (0);
        if (war->GetAttackerNameHashByIndex (0) != Urho3D::StringHash (firstPlayer->GetName ()) &&
                war->GetDefenderNameHashByIndex (0) != Urho3D::StringHash (secondPlayer->GetName ()))
        {
            ErrorExit ("Error, expected that first player will be attacker and second will be defender!");
        }
    }

    for (int iteration = 0; iteration < 3; iteration++)
    {
        Urho3D::SharedPtr <Colonization::DiplomacyPeaceRequest>
                peaceRequest (new Colonization::DiplomacyPeaceRequest (context_));
        peaceRequest->SetEnemy (Urho3D::StringHash (firstPlayer->GetName ()));
        peaceRequest->SetPeaceRequester (Urho3D::StringHash (secondPlayer->GetName ()));
        peaceRequest->SetWarHash (diplomacyProcessor->GetWarByIndex (0)->GetHash ());
        diplomacyProcessor->AddDiplomacyRequest (peaceRequest);

        float untilCheck = peaceRequest->GetTimeUntilAutodecline () * 1.1f;
        do
        {
            untilCheck -= updateEventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
            diplomacyProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
            playersManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        } while (untilCheck > 0.0f);

        if (iteration != 2 && diplomacyProcessor->GetWarsCount () != 1)
        {
            ErrorExit ("Iteration " + Urho3D::String (iteration) + ". Expected 1 war, but got " +
                       Urho3D::String (diplomacyProcessor->GetWarsCount ()) + ".");
        }
        else if (iteration == 2 && diplomacyProcessor->GetWarsCount () != 0)
        {
            ErrorExit ("Iteration 2. Expected 0 wars, but got " + Urho3D::String (diplomacyProcessor->GetWarsCount ()) + ".");
        }
    }

    engine_->Exit ();
}

void TestApplication::Stop ()
{

}
