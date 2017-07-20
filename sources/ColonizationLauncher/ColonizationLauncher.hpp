#pragma once
#include <Colonization/Utils/Activities/ActivitiesApplication.hpp>

class ColonizationLauncher : public Colonization::ActivitiesApplication
{
URHO3D_OBJECT (ColonizationLauncher, ActivitiesApplication)
public:
    explicit ColonizationLauncher (Urho3D::Context *context);
    virtual ~ColonizationLauncher ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
