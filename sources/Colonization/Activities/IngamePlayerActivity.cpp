#include <Colonization/BuildConfiguration.hpp>
#include "IngamePlayerActivity.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/Network/Network.h>

namespace Colonization
{
IngamePlayerActivity::IngamePlayerActivity (Urho3D::Context *context) : Activity (context),
    scene_ (0),
    angelScript_ (0),
    serverAdress_ ("localhost"),
    serverPort_ (13534),
    playerName_ ("Human")
{

}

IngamePlayerActivity::~IngamePlayerActivity ()
{

}

void IngamePlayerActivity::Start ()
{
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    scene_ = new Urho3D::Scene (context_);
    angelScript_ = scene_->CreateComponent <Urho3D::ScriptInstance> (Urho3D::LOCAL);
    angelScript_->CreateObject (resourceCache->GetResource <Urho3D::ScriptFile> ("AngelScript/Player/Player.as"), "Player");

    Urho3D::VariantVector executionParameters;
    executionParameters.Push (application_);
    angelScript_->Execute ("void set_launcherApplication (LauncherApplication @launcherApplication)", executionParameters);

    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    Urho3D::VariantMap identity;
    identity ["Name"] = playerName_;
    network->Connect (serverAdress_, serverPort_, scene_, identity);
}

void IngamePlayerActivity::Update (float timeStep)
{

}

void IngamePlayerActivity::Stop ()
{
    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    network->Disconnect ();
    delete scene_;
}

Urho3D::String IngamePlayerActivity::GetServerAdress ()
{
    return serverAdress_;
}

void IngamePlayerActivity::SetServerAdress (Urho3D::String serverAdress)
{
    serverAdress_ = serverAdress;
}

unsigned short IngamePlayerActivity::GetServerPort ()
{
    return serverPort_;
}

void IngamePlayerActivity::SetServerPort (unsigned short serverPort)
{
    serverPort_ = serverPort;
}

Urho3D::String IngamePlayerActivity::GetPlayerName ()
{
    return playerName_;
}

void IngamePlayerActivity::SetPlayerName (Urho3D::String playerName)
{
    playerName_ = playerName;
}
}
