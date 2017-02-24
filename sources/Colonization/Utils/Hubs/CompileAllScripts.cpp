#include <Colonization/BuildConfiguration.hpp>
#include "CompileAllScripts.hpp"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/AngelScript/ScriptFile.h>

namespace Colonization
{
bool CompileAllScripts (Urho3D::Context *context, Urho3D::String resourcesDir)
{
    Urho3D::FileSystem *fileSystem = context->GetSubsystem <Urho3D::FileSystem> ();
    Urho3D::Vector <Urho3D::String> scriptFiles;
    fileSystem->ScanDir (scriptFiles, resourcesDir, "*.as", Urho3D::SCAN_FILES, true);
    Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Detected " + Urho3D::String (scriptFiles.Size ()) + " scripts.");

    Urho3D::ResourceCache *resourceCache = context->GetSubsystem <Urho3D::ResourceCache> ();
    bool isAllCompiled = true;
    for (int index = 0; index < scriptFiles.Size (); index++)
    {
        Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Compiling " + scriptFiles.At (index) + "...");
        Urho3D::ScriptFile *script = resourceCache->GetResource <Urho3D::ScriptFile> (scriptFiles.At (index));
        if (!script || !script->IsCompiled ())
        {
            isAllCompiled = false;
        }
    }
    return isAllCompiled;
}
}
