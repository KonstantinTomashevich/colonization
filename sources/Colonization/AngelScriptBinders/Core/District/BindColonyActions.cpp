#include <Colonization/BuildConfiguration.hpp>
#include "BindColonyActions.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Core/District/ColonyActions.hpp>

namespace Colonization
{
Urho3D::StringHash get_COLONY_ACTION_ID ()
{
    return COLONY_ACTION_ID;
}

Urho3D::StringHash get_COLONY_ACTION_PROGRESS ()
{
    return COLONY_ACTION_PROGRESS;
}

Urho3D::StringHash get_ColonyActions_BUILD_WAR_SHIP ()
{
    return ColonyActions::BUILD_WAR_SHIP;
}

Urho3D::StringHash get_ColonyActions_BuildWarShip_TARGET_DISTRICT ()
{
    return ColonyActions::BuildWarShip::TARGET_DISTRICT;
}

void BindColonyActions(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_COLONY_ACTION_ID ()", asFUNCTION (get_COLONY_ACTION_ID), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_COLONY_ACTION_PROGRESS ()", asFUNCTION (get_COLONY_ACTION_PROGRESS), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_ColonyActions_BUILD_WAR_SHIP ()", asFUNCTION (get_ColonyActions_BUILD_WAR_SHIP), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_ColonyActions_BuildWarShip_TARGET_DISTRICT ()", asFUNCTION (get_ColonyActions_BuildWarShip_TARGET_DISTRICT), asCALL_CDECL);
}
}
