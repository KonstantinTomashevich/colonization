#include <Colonization/BuildConfiguration.hpp>
#include "RegisterAllObjects.hpp"

#include <Colonization/Backend/ColoniesManager.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/Player.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/PlayerInfo.hpp>
#include <Colonization/Core/Unit.hpp>

#include <Colonization/Frontend/FogOfWarCalculator.hpp>

namespace Colonization
{
void RegisterAllObjects (Urho3D::Context *context)
{
    ColoniesManager::RegisterObject (context);
    MessagesHandler::RegisterObject (context);
    PlayersManager::RegisterObject (context);
    TradeProcessor::RegisterObject (context);
    UnitsManager::RegisterObject (context);

    District::RegisterObject (context);
    GameConfiguration::RegisterObject (context);
    InternalTradeArea::RegisterObject (context);
    Map::RegisterObject (context);
    PlayerInfo::RegisterObject (context);
    Unit::RegisterObject (context);

    FogOfWarCalculator::RegisterObject (context);
}
}
