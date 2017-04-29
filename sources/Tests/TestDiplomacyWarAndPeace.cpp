#include "TestDiplomacyWarAndPeace.hpp"
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
#include <Colonization/Backend/MessagesHandler/NetworkMessageType.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestDiplomacyWarAndPeaceApplication)
namespace Tests
{
TestDiplomacyWarAndPeaceApplication::TestDiplomacyWarAndPeaceApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestDiplomacyWarAndPeaceApplication::~TestDiplomacyWarAndPeaceApplication ()
{

}

void TestDiplomacyWarAndPeaceApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestDiplomacyWarAndPeace.log";
    engineParameters_ ["WindowTitle"] = "TestDiplomacyWarAndPeace";
}

void TestDiplomacyWarAndPeaceApplication::Start ()
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

void TestDiplomacyWarAndPeaceApplication::Stop ()
{

}

DiplomacyInternalPlayer::DiplomacyInternalPlayer (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Scene *scene) :
    Colonization::Player (context, name, color, 0, scene),
    declinesCount_ (0)
{

}

DiplomacyInternalPlayer::~DiplomacyInternalPlayer ()
{

}

void DiplomacyInternalPlayer::SendMessage (int messageId, bool reliable, bool inOrder, const Urho3D::VectorBuffer &message, unsigned contentId)
{
    if (messageId == Colonization::STC_NETWORK_MESSAGE_DIPLOMACY_OFFER)
    {
        Urho3D::VectorBuffer messageData;
        messageData.SetData (message.GetBuffer ());
        Urho3D::StringHash offerType = messageData.ReadStringHash ();

        if (offerType == Colonization::DIPLOMACY_OFFER_PEACE)
        {
            unsigned offerId = messageData.ReadUInt ();
            if (declinesCount_ != 1)
            {
                Urho3D::VectorBuffer actionData;
                actionData.WriteInt (static_cast <int> (Colonization::PLAYER_ACTION_RESPONCE_TO_DIPLOMACY_OFFER));
                actionData.WriteUInt (offerId);

                if (declinesCount_ == 0)
                {
                    actionData.WriteUInt (static_cast <unsigned> (Colonization::DRPSTATUS_DECLINED));
                    declinesCount_++;
                }
                else
                {
                    actionData.WriteUInt (static_cast <unsigned> (Colonization::DRPSTATUS_ACCEPTED));
                }

                Urho3D::Pair <Colonization::PlayerActionType, Urho3D::Variant> action;
                action.first_ = Colonization::PLAYER_ACTION_RESPONCE_TO_DIPLOMACY_OFFER;
                action.second_ = Urho3D::Variant (actionData);
                AddAction (action);
            }
            else
            {
                declinesCount_++;
            }
        }
    }
}

void DiplomacyInternalPlayer::Disconnect (int wait)
{

}

bool DiplomacyInternalPlayer::IsInternal() const
{
    return true;
}
}
