#include "Defines.hpp"
#include "CheckScripts.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptFile.h>

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
    Colonization::RegisterAllObjects (context_);
    Colonization::BindAll (script);

    Urho3D::FileSystem *fileSystem = context_->GetSubsystem <Urho3D::FileSystem> ();
    bool isCompiled = false;
    if (fileSystem->DirExists ("Data/"))
    {
        isCompiled = Colonization::CompileAllScripts (context_);
    }
    else
    {
        isCompiled = Colonization::CompileAllScripts (context_, CHECK_SCRIPTS_RESOURCE_DIR);
    }

    if (isCompiled)
    {
        engine_->Exit ();
    }
    else
    {
        ErrorExit ("Compilation failed! See CheckScripts.log for more information.");
    }
}

void CheckScripts::Stop ()
{

}
