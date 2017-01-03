#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/InternalTradeArea.hpp>

namespace Colonization
{
class PlayersManager;
class TradeProcessor : public Urho3D::Component
{
URHO3D_OBJECT (TradeProcessor, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <InternalTradeArea> > tradeAreas_;
    float untilTradeAreasUpdate_;

    void UpdateTradeAreas (float updateDelay);
    void UpdateTradeArea (InternalTradeArea *tradeArea, Map *map, District *start, Urho3D::PODVector <District *> &unscannedList);
    void ProcessTradeAreaDistrict (Map *map, District *district, Urho3D::PODVector <District *> &areaDistricts, Urho3D::PODVector <District *> &unscannedList);
    void ProcessTradeAreaIncome (PlayersManager *playersManager, Map *map, InternalTradeArea *tradeArea, float updateDelay);
    void ClearTradeAreas ();

public:
    TradeProcessor (Urho3D::Context *context);
    virtual ~TradeProcessor ();
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    int GetTradeAreasCount ();
    InternalTradeArea *GetTradeAreaByIndex (int index);
};
}
