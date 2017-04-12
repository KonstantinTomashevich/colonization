#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyRequest.hpp"

namespace Colonization
{
DiplomacyRequest::DiplomacyRequest (Urho3D::Context *context) : Urho3D::Object (context),
    id_ ()
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
}
