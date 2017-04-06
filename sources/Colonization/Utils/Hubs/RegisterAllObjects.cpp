#include <Colonization/BuildConfiguration.hpp>
#include "RegisterAllObjects.hpp"

#include <Colonization/Backend/ColoniesActionsProcessor.hpp>
#include <Colonization/Backend/ColoniesEvolutionManager.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/Player.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/PlayersPointsCalculator.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/VictoryProgressUpdater.hpp>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/PlayerInfo.hpp>
#include <Colonization/Core/Unit.hpp>

#include <Colonization/Frontend/FogOfWarCalculator.hpp>
#include <Colonization/Frontend/MapMaskUpdater.hpp>

#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>

namespace Colonization
{
void RegisterAllObjects (Urho3D::Context *context)
{
    ColoniesActionsProcessor::RegisterObject (context);
    ColoniesEvolutionManager::RegisterObject (context);
    MessagesHandler::RegisterObject (context);
    PlayersManager::RegisterObject (context);
    PlayersPointsCalculator::RegisterObject (context);
    TradeProcessor::RegisterObject (context);
    UnitsManager::RegisterObject (context);
    VictoryProgressUpdater::RegisterObject (context);

    District::RegisterObject (context);
    GameConfiguration::RegisterObject (context);
    InternalTradeArea::RegisterObject (context);
    Map::RegisterObject (context);
    PlayerInfo::RegisterObject (context);
    Unit::RegisterObject (context);

    FogOfWarCalculator::RegisterObject (context);
    MapMaskUpdater::RegisterObject (context);

    NetworkUpdateCounter::RegisterObject (context);
    NetworkUpdateSmoother::RegisterObject (context);
}
}
