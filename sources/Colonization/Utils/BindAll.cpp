#include <Colonization/BuildConfiguration.hpp>
#include "BindAll.hpp"

#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/BindMap.hpp>
#include <Colonization/AngelScriptBinders/BindNetworkMessageType.hpp>

#include <Colonization/AngelScriptBinders/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindMainMenuActivity.hpp>
#include <Colonization/AngelScriptBinders/BindIngamePlayerActivity.hpp>
#include <Colonization/AngelScriptBinders/BindUnit.hpp>

#include <Colonization/AngelScriptBinders/BindPlayerActionType.hpp>
#include <Colonization/AngelScriptBinders/BindInternalTradeArea.hpp>
#include <Colonization/AngelScriptBinders/BindPlayerInfo.hpp>
#include <Colonization/AngelScriptBinders/BindActivitiesApplication.hpp>

namespace Colonization
{
void BindAll (Urho3D::Script *script)
{
    BindActivity (script);
    BindActivitiesApplication (script);
    BindDistrict (script);
    BindMap (script);
    BindNetworkMessageType (script);
    BindHostActivity (script);
    BindMainMenuActivity (script);
    BindIngamePlayerActivity (script);
    BindUnit (script);
    BindPlayerActionType (script);
    BindInternalTradeArea (script);
    BindPlayerInfo (script);
}
}
