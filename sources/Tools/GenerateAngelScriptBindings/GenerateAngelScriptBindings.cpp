#include "GenerateAngelScriptBindings.hpp"
#include "Defines.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/FileSystem.h>
#include <Colonization/Utils/Hubs/BindAll.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (GenerateAngelScriptBindings)
GenerateAngelScriptBindings::GenerateAngelScriptBindings (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

GenerateAngelScriptBindings::~GenerateAngelScriptBindings ()
{

}

void GenerateAngelScriptBindings::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["WindowWidth"] = 100;
    engineParameters_ ["WindowHeight"] = 100;
    engineParameters_ ["WindowTitle"] = "Generating AngelScript API header...";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void GenerateAngelScriptBindings::Start ()
{
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    Colonization::BindAll (script);

    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetQuiet (true);
    log->SetTimeStamp (false);
    log->SetLevel (Urho3D::LOG_WARNING);

    Urho3D::FileSystem *fileSystem = context_->GetSubsystem <Urho3D::FileSystem> ();
    if (fileSystem->DirExists ("Data/"))
    {
        log->Open ("AngelScriptAPI.hpp");
    }
    else
    {
        log->Open (ANGELSCRIPT_API_HEADER_OUTPUT);
    }
    script->DumpAPI (Urho3D::C_HEADER);
    log->Close ();
    engine_->Exit ();
}

void GenerateAngelScriptBindings::Stop ()
{

}
