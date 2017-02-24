#undef NDEBUG
#include "Defines.hpp"
#include "CheckScripts.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/FileSystem.h>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>
#include <Colonization/Utils/Hubs/BindAll.hpp>
#include <Colonization/Utils/Hubs/CompileAllScripts.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (CheckScripts)
CheckScripts::CheckScripts (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

CheckScripts::~CheckScripts ()
{

}

void CheckScripts::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["WindowWidth"] = 100;
    engineParameters_ ["WindowHeight"] = 100;
    engineParameters_ ["WindowTitle"] = "Checking scripts...";
    engineParameters_ ["LogName"] = "CheckScripts.log";
}

void CheckScripts::Start ()
{
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);
    Colonization::RegisterAllObjects (context_);
    Colonization::BindAll (script);

    Urho3D::FileSystem *fileSystem = context_->GetSubsystem <Urho3D::FileSystem> ();
    if (fileSystem->DirExists ("Data/"))
    {
        assert (Colonization::CompileAllScripts (context_));
    }
    else
    {
        assert (Colonization::CompileAllScripts (context_, CHECK_SCRIPTS_RESOURCE_DIR));
    }
    engine_->Exit ();
}

void CheckScripts::Stop ()
{

}
