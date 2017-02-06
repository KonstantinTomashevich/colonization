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
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    backgroundScene_ = new Urho3D::Scene (context_);
    backgroundScene_->AddRef ();
    angelScriptGUI_ = backgroundScene_->CreateChild ("script_main", Urho3D::LOCAL)->
            CreateComponent <Urho3D::ScriptInstance> (Urho3D::LOCAL);
    angelScriptGUI_->CreateObject (resourceCache->GetResource <Urho3D::ScriptFile> ("AngelScript/MainMenu/MainMenu.as"), "MainMenu");

    Urho3D::VariantVector executionParameters;
    executionParameters.Push (application_);
    angelScriptGUI_->Execute ("void set_activitiesApplication (ActivitiesApplication @activitiesApplication)", executionParameters);
}

void MainMenuActivity::Update (float timeStep)
{

}

void MainMenuActivity::Stop ()
{
    delete backgroundScene_;
}
}
