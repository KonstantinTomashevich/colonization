#include <Colonization/BuildConfiguration.hpp>
#include "DistrictComparator.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
namespace DistrictComparators
{
bool HigherLogistics (const District *first, const District *second)
{
    return (first->GetLogisticsEvolutionPoints () > second->GetLogisticsEvolutionPoints ());
}
}
}
