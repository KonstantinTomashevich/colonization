function PrintConfiguration ()
    print ("### Angel Script Bindings Generator Configuration:")
    print ("    Path prefix: " .. configuration.pathPrefix)
    print ("    Project dir: " .. configuration.projectDir)
    print ("    Bindings dir: " .. configuration.bindingsDir)
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
return PrintConfiguration
