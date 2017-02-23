#include "Generator.hpp"
#include "Defines.hpp"
#include <Urho3D/IO/Log.h>
#include <Colonization/Utils/Hubs/BindAll.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Generator)
Generator::Generator (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

Generator::~Generator ()
{

}

void Generator::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["WindowWidth"] = 100;
    engineParameters_ ["WindowHeight"] = 100;
    engineParameters_ ["WindowTitle"] = "Generating AngelScript API header...";
}

void Generator::Start ()
{
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    Colonization::BindAll (script);

    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetQuiet (true);
    log->SetTimeStamp (false);
    log->SetLevel (Urho3D::LOG_WARNING);

    log->Open (ANGELSCRIPT_API_HEADER_OUTPUT);
    script->DumpAPI (Urho3D::C_HEADER);
    log->Close ();
    engine_->Exit ();
}

void Generator::Stop ()
{

}
