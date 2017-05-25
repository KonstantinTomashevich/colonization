ConfigurationUtils = {}
ConfigurationUtils.PrintConfiguration = function ()
    print ("### Angel Script Bindings Generator Configuration:")
    print ("    Path prefix: " .. configuration.pathPrefix)
    print ("    Project dir: " .. configuration.projectDir)
    print ("    Bindings dir: " .. configuration.bindingsDir)
    print ("    Bindings file name: " .. configuration.bindingsFileName)
    print ("    Bindings generator command: " .. configuration.bindingsGeneratorCommand .. "\n")

    print ("    Files: ")
    for index, value in pairs (configuration.files) do
        print ("        [" .. index .. "] " .. value)
    end
    print ("\n")

    print ("    Output hpp file template path: " .. configuration.outputHppFileTemplate)
    print ("    Output cpp file template path: " .. configuration.outputCppFileTemplate)
    print ("\n")
    print ("\n")
end

ConfigurationUtils.LocalFileNameToFilePath = function (fileName)
    return configuration.pathPrefix .. "/" .. configuration.projectDir .. "/" .. fileName
end

ConfigurationUtils.LocalFileNameToBindingsFilePath = function (fileName)
    return configuration.pathPrefix .. "/" .. configuration.projectDir .. "/" .. configuration.bindingsDir .. "/" .. fileName
end
return ConfigurationUtils
