-- TODO: Static functions MUST use wrappers! At the moment static functions binding is INCORRECT!
-- TODO: Maybe add external classes table? They will give information about how they will be inherited. Also print warnings about external classes use.
scriptDirectory = arg [0]:gsub ("Main.lua", "")
Core = require (scriptDirectory .. "/Core/Core")
if Core.LoadCoreScripts () and
    Core.LoadBindables () and
    Core.LoadConfiguration (arg [1]:gsub (".lua", "")) and
    Core.ParseInputFiles () and
    Core.ApplyArguments () and
    Core.PrintParsedBindables () and
    Core.CreateAndPrintFilesToWriteList () and
    Core.WriteBindings () then

    os.exit (0)
else
    os.exit (1)
end
