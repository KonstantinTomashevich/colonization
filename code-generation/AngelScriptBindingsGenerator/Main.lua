scriptDirectory = arg [0]:gsub ("Main.lua", "")
Core = require (scriptDirectory .. "/Core/Core")
if Core.LoadCoreScripts () and
    Core.LoadBindables () and
    Core.LoadConfiguration (arg [1]:gsub (".lua", "")) and
    Core.ParseInputFiles () and
    Core.PrintParsedBindables () and
    Core.CreateAndPrintFilesToWriteList () and
    Core.WriteBindings () then

    return 0
else
    return 1
end
