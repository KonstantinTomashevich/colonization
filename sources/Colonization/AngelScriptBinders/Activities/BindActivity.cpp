#include <Colonization/BuildConfiguration.hpp>
#include "BindActivity.hpp"
#include <Colonization/AngelScriptBinders/Templates/BindActivityInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>

namespace Colonization
{

void BindActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <Activity> (engine, "Activity");
    BindActivityInterface <Activity> (script, "Activity");
}
}
