#include <Colonization/BuildConfiguration.hpp>
#include "BindAll.hpp"

#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/BindGameConfiguration.hpp>
#include <Colonization/AngelScriptBinders/BindMap.hpp>
#include <Colonization/AngelScriptBinders/BindNetworkMessageType.hpp>

#include <Colonization/AngelScriptBinders/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindMainMenuActivity.hpp>
#include <Colonization/AngelScriptBinders/BindIngameClientActivity.hpp>
#include <Colonization/AngelScriptBinders/BindUnit.hpp>

#include <Colonization/AngelScriptBinders/BindPlayerActionType.hpp>
#include <Colonization/AngelScriptBinders/BindInternalTradeArea.hpp>
#include <Colonization/AngelScriptBinders/BindPlayerInfo.hpp>
#include <Colonization/AngelScriptBinders/BindActivitiesApplication.hpp>

#include <Colonization/AngelScriptBinders/BindFogOfWarCalculator.hpp>
#include <Colonization/AngelScriptBinders/BindMapMaskUpdater.hpp>

namespace Colonization
{
void BindAll (Urho3D::Script *script)
{
    BindActivity (script);
    BindActivitiesApplication (script);
    BindDistrict (script);
    BindGameConfiguration (script);
    BindMap (script);
    BindNetworkMessageType (script);
    BindHostActivity (script);
    BindMainMenuActivity (script);
    BindIngameClientActivity (script);
    BindUnit (script);
    BindPlayerActionType (script);
    BindInternalTradeArea (script);
    BindPlayerInfo (script);
    BindFogOfWarCalculator (script);
    BindMapMaskUpdater (script);
}
}
