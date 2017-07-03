#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>

namespace Colonization
{
class UnitsManager;
class PlayersManager;

class TradeProcessor : public Urho3D::Component
{
URHO3D_OBJECT (TradeProcessor, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <InternalTradeArea> > tradeAreas_;
    float tradeAreasUpdateDelay_;
    float untilTradeAreasUpdate_;

    void UpdateTradeAreas (float updateDelay);
    float UpdateTradeArea (InternalTradeArea *tradeArea, Map *map, District *start,
                           Urho3D::PODVector <District *> &unscannedList, GameConfiguration *configuration);
    void ProcessTradeAreaDistrict (Map *map, District *district, Urho3D::PODVector <District *> &areaDistricts,
                                   Urho3D::PODVector <District *> &unscannedList, GameConfiguration *configuration);

    void ProcessTradeAreaIncome (PlayersManager *playersManager, Map *map, InternalTradeArea *tradeArea,
                                 GameConfiguration *configuration, float updateDelay);
    float CalculateUnsoldGoodsCost (GameConfiguration *configuration, District *district);
    void SendTrader (Map *map, UnitsManager *unitsManager, GameConfiguration *configuration, District *district, float goodsCost);
    void ClearTradeAreas ();
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    explicit TradeProcessor (Urho3D::Context *context);
    virtual ~TradeProcessor ();
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    int GetTradeAreasCount () const;
    InternalTradeArea *GetTradeAreaByIndex (int index) const;

    float GetTimeUntilTradeAreasUpdate () const;
    float GetTradeAreasUpdateDelay () const;
    void SetTradeAreasUpdateDelay (float tradeAreasUpdateDelay);
};
}
