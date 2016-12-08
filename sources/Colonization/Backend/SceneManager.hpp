#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Colonization/Backend/GameStateManager.hpp>

namespace Colonization
{
class SceneManager : public Urho3D::Object
{
URHO3D_OBJECT (SceneManager, Object)
protected:
    Urho3D::Scene *scene_;

    void WritePlayersStats ();
    void UpdateMap (bool rewriteDistrictsPolygons);

    void UpdateWaitingForPlayersState ();
    void UpdatePlayingState ();
    void UpdateFinishedState ();

    void PrepareForWaitingForPlayersState ();
    void PrepareForPlayingState ();
    void PrepareForFinishedState ();
public:
    SceneManager (Urho3D::Context *context);
    virtual ~SceneManager ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    Urho3D::Scene *GetScene ();
    void PrepareForGameState (GameStateType gameState);
};
}
