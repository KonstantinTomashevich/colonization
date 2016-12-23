#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/InternalTradeArea.hpp>

namespace Colonization
{
class PlayersManager;
class TradeProcessor : public Urho3D::Object
{
URHO3D_OBJECT (TradeProcessor, Object)
protected:
    Urho3D::PODVector <InternalTradeArea *> tradeAreas_;
    float beforeTradeAreasUpdate_;

    void UpdateTradeAreas (float updateDelay);
    InternalTradeArea *CreateTradeArea (District *start, Urho3D::PODVector <District *> &unscannedList);
    void ProcessTradeAreaDistrict (District *district, Urho3D::PODVector <District *> &areaDistricts, Urho3D::PODVector <District *> &unscannedList);
    void ProcessTradeAreaIncome (PlayersManager *playersManager, Map *map, InternalTradeArea *tradeArea, float updateDelay);
    void ClearTradeAreas ();

public:
    TradeProcessor (Urho3D::Context *context);
    virtual ~TradeProcessor ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    int GetTradeAreasCount ();
    InternalTradeArea *GetTradeAreaByIndex (int index);
};
}
