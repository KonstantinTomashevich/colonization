#include <Colonization/BuildConfiguration.hpp>
#include "MainMenuActivity.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/AngelScript/ScriptFile.h>

namespace Colonization
{
MainMenuActivity::MainMenuActivity (Urho3D::Context *context) : Activity (context),
    backgroundScene_ (0),
    angelScriptGUI_ (0)
{

}

MainMenuActivity::~MainMenuActivity ()
{

}

void MainMenuActivity::Start ()
{
    // TODO: Maybe create beatiful scene later? After first prototype of course.
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    backgroundScene_ = new Urho3D::Scene (context_);
    angelScriptGUI_ = backgroundScene_->CreateComponent <Urho3D::ScriptInstance> (Urho3D::LOCAL);
    angelScriptGUI_->CreateObject (resourceCache->GetResource <Urho3D::ScriptFile> ("AngelScript/MainMenu.as"), "MainMenu");
}

void MainMenuActivity::Update (float timeStep)
{

}

void MainMenuActivity::Stop ()
{
    delete backgroundScene_;
}
}
