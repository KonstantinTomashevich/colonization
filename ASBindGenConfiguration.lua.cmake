configuration = {}
configuration.pathPrefix = "./sources/"
configuration.projectDir = "Colonization"
configuration.bindingsDir = "AngelScriptBindings"
configuration.bindingsFileName = "Bindings"
configuration.bindingsGeneratorCommand = "//@ASBindGen"

configuration.files = {
${COLONIZATION_HEADERS_LIST}
}

configuration.outputHppFileTemplate = "./sources/Colonization/Bindings.hpp.template"
configuration.outputCppFileTemplate = "./sources/Colonization/Bindings.cpp.template"
return configuration
