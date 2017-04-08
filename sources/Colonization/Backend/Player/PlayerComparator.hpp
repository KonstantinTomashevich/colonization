#pragma once
#include <Colonization/Backend/Player/Player.hpp>

namespace Colonization
{
class Player;
typedef bool (*PlayerComparator) (const Player *first, const Player *second);
namespace PlayerComparators
{
bool HigherPoints (const Player *first, const Player *second);
}
}
