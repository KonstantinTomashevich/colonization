#include <Colonization/BuildConfiguration.hpp>
#include "HostActivity.hpp"
#include <Urho3D/Network/Network.h>
#include <Urho3D/Core/Context.h>

namespace Colonization
{
HostActivity::HostActivity (Urho3D::Context *context) : Activity (context),
    serverPort_ (13534)
{

}

HostActivity::~HostActivity ()
{

}

unsigned short HostActivity::GetServerPort()
{
    return serverPort_;
}

void HostActivity::SetServerPort (unsigned short serverPort)
{
    serverPort_ = serverPort;
}

void HostActivity::Start ()
{
    context_->GetSubsystem <Urho3D::Network> ()->StartServer (serverPort_);
    gameStateManager_ = new GameStateManager (context_);
}

void HostActivity::Update (float timeStep)
{
    gameStateManager_->Update (timeStep);
}

void HostActivity::Stop ()
{
    delete gameStateManager_;
    context_->GetSubsystem <Urho3D::Network> ()->StopServer ();
}
}
