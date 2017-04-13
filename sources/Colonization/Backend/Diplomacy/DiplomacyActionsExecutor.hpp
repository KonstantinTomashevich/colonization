#pragma once
#include <Urho3D/Scene/Scene.h>

namespace Colonization
{
class DiplomacyActionsExecutor
{
protected:
    Urho3D::Scene *scene_;

public:
    DiplomacyActionsExecutor (Urho3D::Scene *scene);
    virtual ~DiplomacyActionsExecutor ();
};
}
