#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyRequest.hpp"

namespace Colonization
{
DiplomacyRequest::DiplomacyRequest (Urho3D::Context *context) : Urho3D::Object (context),
    id_ (),
    isFinished_ (false)
{

}

DiplomacyRequest::~DiplomacyRequest ()
{

}

unsigned DiplomacyRequest::GetId () const
{
    return id_;
}

void DiplomacyRequest::SetId (unsigned id)
{
    id_ = id;
}

bool DiplomacyRequest::IsFinished ()
{
    return isFinished_;
}
}
