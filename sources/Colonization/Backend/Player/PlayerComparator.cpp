#include <Colonization/BuildConfiguration.hpp>
#include "PlayerComparator.hpp"

namespace Colonization
{
namespace PlayerComparators
{
bool HigherPoints (const Player *first, const Player *second)
{
    return (first->GetPoints () > second->GetPoints ());
}
}
}
