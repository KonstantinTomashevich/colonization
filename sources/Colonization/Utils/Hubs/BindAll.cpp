#include <Colonization/BuildConfiguration.hpp>
#include "BindAll.hpp"

#include <Colonization/AngelScriptBinders/Activities/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindIngameClientActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindMainMenuActivity.hpp>

#include <Colonization/AngelScriptBinders/Core/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/Core/BindGameConfiguration.hpp>
#include <Colonization/AngelScriptBinders/Core/BindInternalTradeArea.hpp>
#include <Colonization/AngelScriptBinders/Core/BindMap.hpp>
#include <Colonization/AngelScriptBinders/Core/BindPlayerInfo.hpp>
#include <Colonization/AngelScriptBinders/Core/BindUnit.hpp>

#include <Colonization/AngelScriptBinders/Backend/BindNetworkMessageType.hpp>
#include <Colonization/AngelScriptBinders/Backend/BindPlayerActionType.hpp>
#include <Colonization/AngelScriptBinders/Backend/BindVictoryTypesProcessorScriptDataAccessor.hpp>

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

    BindDistrict (script);
    BindGameConfiguration (script);
    BindUnit (script, false);
    BindMap (script);
    BindInternalTradeArea (script);
    BindPlayerInfo (script);
    BindUnitInterface (script, "Unit");

    BindNetworkMessageType (script);
    BindPlayerActionType (script);
    BindVictoryTypesProcessorScriptDataAccessor (script);

    BindFogOfWarCalculator (script);
    BindMapMaskUpdater (script);
    BindActivitiesApplication (script);
}
}
