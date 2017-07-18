#include <Colonization/BuildConfiguration.hpp>
#include "IngameClientActivity.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/Network/Network.h>

namespace Colonization
{
IngameClientActivity::IngameClientActivity (Urho3D::Context *context) : Activity (context),
    scene_ (0),
    angelScript_ (0),
    isAdmin_ (false),
    serverAdress_ ("localhost"),
    serverPort_ (13534),
    playerName_ ("Human"),
    playerColor_ (Urho3D::Color::GRAY)
{

}

IngameClientActivity::~IngameClientActivity ()
{

}

void IngameClientActivity::Start ()
{
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    scene_ = new Urho3D::Scene (context_);
    // Add ref because scene is used in AngelScript too.
    scene_->AddRef ();
    angelScript_ = scene_->CreateChild ("script_main", Urho3D::LOCAL)->CreateComponent <Urho3D::ScriptInstance> (Urho3D::LOCAL);
    angelScript_->CreateObject (resourceCache->GetResource <Urho3D::ScriptFile> ("AngelScript/Client/Client.as"), "Client");
    assert (angelScript_->GetScriptFile ()->IsCompiled ());

    Urho3D::VariantVector executionParameters;
    executionParameters.Push (GetApplication ());
    angelScript_->Execute ("void set_activitiesApplication (ActivitiesApplication @activitiesApplication)", executionParameters);

    executionParameters.Clear ();
    executionParameters.Push (playerName_);
    angelScript_->Execute ("void set_playerName (String playerName)", executionParameters);

    executionParameters.Clear ();
    executionParameters.Push (isAdmin_);
    angelScript_->Execute ("void set_isAdmin (bool isAdmin)", executionParameters);

    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    Urho3D::VariantMap identity;
    identity ["Name"] = playerName_;
    identity ["Color"] = playerColor_;
    network->Connect (serverAdress_, serverPort_, scene_, identity);
}

void IngameClientActivity::Update (float timeStep)
{

}

void IngameClientActivity::Stop ()
{
    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    network->Disconnect ();
    delete scene_;
}

Urho3D::String IngameClientActivity::GetServerAdress ()
{
    return serverAdress_;
}

void IngameClientActivity::SetServerAdress (Urho3D::String serverAdress)
{
    serverAdress_ = serverAdress;
}

unsigned short IngameClientActivity::GetServerPort ()
{
    return serverPort_;
}

void IngameClientActivity::SetServerPort (unsigned short serverPort)
{
    serverPort_ = serverPort;
}

bool IngameClientActivity::IsAdmin () const
{
    return isAdmin_;
}

void IngameClientActivity::SetIsAdmin (bool isAdmin)
{
    isAdmin_ = isAdmin;
}

Urho3D::String IngameClientActivity::GetPlayerName ()
{
    return playerName_;
}

void IngameClientActivity::SetPlayerName (Urho3D::String playerName)
{
    playerName_ = playerName;
}

Urho3D::Color IngameClientActivity::GetPlayerColor ()
{
    return playerColor_;
}

void IngameClientActivity::SetPlayerColor (Urho3D::Color playerColor)
{
    playerColor_ = playerColor;
}
}
