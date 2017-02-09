#include <Colonization/BuildConfiguration.hpp>
#include "BindAll.hpp"

#include <Colonization/AngelScriptBinders/Activities/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindIngameClientActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindMainMenuActivity.hpp>

#include <Colonization/AngelScriptBinders/Backend/BindNetworkMessageType.hpp>
#include <Colonization/AngelScriptBinders/Backend/BindPlayerActionType.hpp>

#include <Colonization/AngelScriptBinders/Core/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/Core/BindGameConfiguration.hpp>
#include <Colonization/AngelScriptBinders/Core/BindInternalTradeArea.hpp>
#include <Colonization/AngelScriptBinders/Core/BindMap.hpp>
#include <Colonization/AngelScriptBinders/Core/BindPlayerInfo.hpp>
#include <Colonization/AngelScriptBinders/Core/BindUnit.hpp>

#include <Colonization/AngelScriptBinders/Frontend/BindFogOfWarCalculator.hpp>
#include <Colonization/AngelScriptBinders/Frontend/BindMapMaskUpdater.hpp>
#include <Colonization/AngelScriptBinders/Utils/BindActivitiesApplication.hpp>

namespace Colonization
{
void BindAll (Urho3D::Script *script)
{
    BindActivity (script);
    BindHostActivity (script);
    BindIngameClientActivity (script);
    BindMainMenuActivity (script);

    BindNetworkMessageType (script);
    BindPlayerActionType (script);

    BindDistrict (script);
    BindGameConfiguration (script);
    BindMap (script);
    BindInternalTradeArea (script);
    BindPlayerInfo (script);
    BindUnit (script);

    BindFogOfWarCalculator (script);
    BindMapMaskUpdater (script);
    BindActivitiesApplication (script);
}
}
