#pragma once
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Colonization
{
class District;
typedef bool (*DistrictComparator) (const District *first, const District *second);
namespace DistrictComparators
{
bool HigherLogistics (const District *first, const District *second);
}
}
