#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Network/Connection.h>
#include <Colonization/Backend/Player.hpp>

namespace Colonization
{
class PlayersManager : Urho3D::Object
{
URHO3D_OBJECT (PlayersManager, Object)
protected:
    Urho3D::HashMap <Urho3D::StringHash, Player *> players_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::StringHash> connectionHashToNameHashMap_;
    Urho3D::Vector <Urho3D::Pair <float, Urho3D::Connection *> > connectionsWithoutId_;

    bool DeleteIdentificatedConnection(Urho3D::Connection *connection);
public:
    PlayersManager (Urho3D::Context *context);
    virtual ~PlayersManager ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandlePlayerConnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandlePlayerDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Player *GetPlayer (Urho3D::StringHash nameHash);
    Player *GetPlayer (Urho3D::Connection *connection);
    void PlayerIdentified (Urho3D::Connection *connection, Urho3D::String name);
    void DisconnectPlayer (Urho3D::StringHash nameHash);
    void DisconnectPlayer (Urho3D::Connection *connection);
};
}
