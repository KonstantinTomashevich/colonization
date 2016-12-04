#include "Activity.hpp"

namespace Colonization
{
Activity::Activity (Urho3D::Context *context) : Urho3D::Object (context), application_ (0)
{

}

Activity::~Activity ()
{

}

void *Activity::GetApplication ()
{
    return application_;
}

void Activity::SetApplication (void *application)
{
    application_ = application;
}
}
