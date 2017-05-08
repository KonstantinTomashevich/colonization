#include <Colonization/BuildConfiguration.hpp>
#include "BindTradersUnit.hpp"
#include <Colonization/AngelScriptBinders/Core/Unit/UnitGettersAndSetters.hpp>
#include <Colonization/AngelScriptBinders/Templates/BindUnitInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/TradersUnit.hpp>

namespace Colonization
{
void BindTradersUnit (Urho3D::Script *script, bool bindInterface)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    if (!bindInterface)
    {
        Urho3D::RegisterComponent <TradersUnit> (engine, "TradersUnit");
    }
    else
    {
        BindUnitInterface <TradersUnit> (script, "TradersUnit");
        engine->RegisterObjectMethod ("TradersUnit", "float get_tradeGoodsCost () const", asMETHOD (TradersUnit, GetTradeGoodsCost), asCALL_THISCALL);
        engine->RegisterObjectMethod ("TradersUnit", "void set_tradeGoodsCost (float tradeGoodsCost)", asMETHOD (TradersUnit, SetTradeGoodsCost), asCALL_THISCALL);
    }
}
}
