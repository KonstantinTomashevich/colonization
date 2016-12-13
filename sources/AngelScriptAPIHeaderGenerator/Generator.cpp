#include "Generator.hpp"
#include "Defines.hpp"
#include <Urho3D/IO/Log.h>

#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/BindMap.hpp>
#include <Colonization/AngelScriptBinders/BindNetworkMessageType.hpp>
#include <Colonization/AngelScriptBinders/BindGameStateType.hpp>
#include <Colonization/AngelScriptBinders/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindMainMenuActivity.hpp>
#include <Colonization/AngelScriptBinders/BindIngamePlayerActivity.hpp>
#include <Colonization/AngelScriptBinders/BindAngelScriptDirectMemoryAccess.hpp>
#include <Colonization/AngelScriptBinders/BindUnits.hpp>
#include <Colonization/AngelScriptBinders/BindUnitsContainer.hpp>
#include <Colonization/AngelScriptBinders/BindPlayerActionType.hpp>

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

    Colonization::BindActivity (script);
    Colonization::BindDistrict (script);
    Colonization::BindMap (script);
    Colonization::BindNetworkMessageType (script);
    Colonization::BindGameStateType (script);
    Colonization::BindHostActivity (script);
    Colonization::BindMainMenuActivity (script);
    Colonization::BindIngamePlayerActivity (script);
    Colonization::BindAngelScriptDirectMemoryAccess (script);
    Colonization::BindUnits (script);
    Colonization::BindUnitsContainer (script);
    Colonization::BindPlayerActionType (script);

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
