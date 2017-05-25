scriptDirectory = arg [1]
Core = require (scriptDirectory .. "Core")
if Core.LoadCoreScripts () and
    Core.LoadBindables () and
    Core.LoadConfiguration (arg [2]:gsub (".lua", "")) and
    Core.ParseInputFiles () and
    Core.PrintParsedBindables () and
    Core.CreateAndPrintFilesToWriteList () then
    return 0
else
    return 1
end
