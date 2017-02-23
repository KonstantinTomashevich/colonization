#include <Colonization/BuildConfiguration.hpp>
#include "Activity.hpp"
#include <Colonization/Utils/Activities/ActivitiesApplication.hpp>

namespace Colonization
{
Activity::Activity (Urho3D::Context *context) : Urho3D::Object (context), application_ (0)
{

}

Activity::~Activity ()
{

}

ActivitiesApplication *Activity::GetApplication()
{
    return application_;
}

void Activity::SetApplication (ActivitiesApplication *application)
{
    application_ = application;
}
}
