#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyRequest.hpp"
#include <Urho3D/Core/Context.h>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
DiplomacyRequest::DiplomacyRequest (Urho3D::Context *context) : Urho3D::Component (context),
    requestId_ ()
{

}

DiplomacyRequest::~DiplomacyRequest ()
{

}

void DiplomacyRequest::RegisterObject (Urho3D::Context *context)
{
    URHO3D_ACCESSOR_ATTRIBUTE ("Id", GetRequestId, SetRequestId, unsigned, 0, Urho3D::AM_DEFAULT);
}

unsigned DiplomacyRequest::GetRequestId () const
{
    return requestId_;
}

void DiplomacyRequest::SetRequestId (unsigned id)
{
    requestId_ = id;
}
}
