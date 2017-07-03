#pragma once
#include <Urho3D/Container/Vector.h>
#include <Colonization/BuildConfiguration.hpp>

namespace Colonization
{
namespace VectorUtils
{
template <class T> int GetElementIndex (Urho3D::PODVector <T> &podVector, T element)
{
    for (int index = 0; index < podVector.Size (); index++)
    {
        if (podVector.At (index) == element)
        {
            return index;
        }
    }
    return -1;
}

template <class T> int GetElementIndex (Urho3D::Vector <T> &vector, T element)
{
    for (int index = 0; index < vector.Size (); index++)
    {
        if (vector.At (index) == element)
        {
            return index;
        }
    }
    return -1;
}
}
}
