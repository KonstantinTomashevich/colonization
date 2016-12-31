#include "EditorLauncher.hpp"
URHO3D_DEFINE_APPLICATION_MAIN (EditorLauncherImpl)

EditorLauncherImpl::EditorLauncherImpl(Urho3D::Context *context) : Colonization::EditorLauncher (context)
{

}

void EditorLauncherImpl::Setup ()
{
    EditorLauncher::Setup ();
}

void EditorLauncherImpl::Start ()
{
    EditorLauncher::Start ();
}

void EditorLauncherImpl::Stop ()
{
    EditorLauncher::Stop ();
}
