configuration = {}
configuration.pathPrefix = "."
configuration.projectDir = "TestProject"
configuration.bindingsDir = "Bindings"
configuration.bindingsFileName = "Bindings"
configuration.bindingsGeneratorCommand = "//@ASBindGen"

configuration.files = {
    "Object1.hpp",
    "ModuleX/ObjectX.hpp"
}

configuration.outputHppFileTemplate = "Templates/BindingsTepmplate.hpp"
configuration.outputCppFileTemplate = "Templates/BindingsTepmplate.cpp"
return configuration
