#include <Colonization/BuildConfiguration.hpp>
#include "BindAll.hpp"

#include <Colonization/AngelScriptBinders/Activities/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/HostActivity/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/HostActivity/BindHostActivityEvents.hpp>
#include <Colonization/AngelScriptBinders/Activities/HostActivity/BindHostActivityConstants.hpp>
#include <Colonization/AngelScriptBinders/Activities/HostActivity/BindGameStateType.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindIngameClientActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindMainMenuActivity.hpp>

#include <Colonization/AngelScriptBinders/Core/Diplomacy/BindDiplomacyWar.hpp>
#include <Colonization/AngelScriptBinders/Core/District/BindDistrictEnums.hpp>
#include <Colonization/AngelScriptBinders/Core/District/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/Core/District/BindColonyActions.hpp>
#include <Colonization/AngelScriptBinders/Core/District/BindDistrictUtils.hpp>
#include <Colonization/AngelScriptBinders/Core/BindGameConfiguration.hpp>
#include <Colonization/AngelScriptBinders/Core/InternalTradeArea/BindDistrictProductionInfoKeys.hpp>
#include <Colonization/AngelScriptBinders/Core/InternalTradeArea/BindTradeDistrictProcessingInfo.hpp>
#include <Colonization/AngelScriptBinders/Core/InternalTradeArea/BindInternalTradeArea.hpp>
#include <Colonization/AngelScriptBinders/Core/BindMap.hpp>
#include <Colonization/AngelScriptBinders/Core/PlayerInfo/BindPlayerInfo.hpp>
#include <Colonization/AngelScriptBinders/Core/PlayerInfo/BindPlayerInfoConstants.hpp>
#include <Colonization/AngelScriptBinders/Core/PlayerInfo/BindVictoryTypeByPointsConstants.hpp>
#include <Colonization/AngelScriptBinders/Core/BindUnit.hpp>

#include <Colonization/AngelScriptBinders/Backend/BindDiplomacyInfoType.hpp>
#include <Colonization/AngelScriptBinders/Backend/BindDiplomacyOfferType.hpp>
#include <Colonization/AngelScriptBinders/Backend/BindDiplomacyRequestPlayerStatus.hpp>
#include <Colonization/AngelScriptBinders/Backend/BindGetUnitsInDistrict.hpp>
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
    BindHostActivityEvents (script);
    BindHostActivityConstants (script);
    BindGameStateType (script);
    BindIngameClientActivity (script);
    BindMainMenuActivity (script);

    BindDiplomacyWar (script);
    BindGameConfiguration (script, false);
    BindDistrictEnums (script);
    BindDistrict (script);
    BindColonyActions (script);
    BindDistrictUtils (script);
    BindUnit (script, false);
    BindMap (script);
    BindGameConfigurationInterface (script, "GameConfiguration");
    BindDistrictProductionInfoKeys (script);
    BindTradeDistrictProcessingInfo (script);
    BindInternalTradeArea (script);
    BindPlayerInfo (script);
    BindPlayerInfoConstants (script);
    BindVictoryTypeByPointsConstants (script);
    BindUnitInterface (script, "Unit");

    BindDiplomacyInfoType (script);
    BindDiplomacyOfferType (script);
    BindDiplomacyRequestPlayerStatus (script);
    BindGetUnitsInDistrict (script);
    BindNetworkMessageType (script);
    BindPlayerActionType (script);
    BindVictoryTypesProcessorScriptDataAccessor (script);

    BindFogOfWarCalculator (script);
    BindMapMaskUpdater (script);
    BindActivitiesApplication (script);
}
}
