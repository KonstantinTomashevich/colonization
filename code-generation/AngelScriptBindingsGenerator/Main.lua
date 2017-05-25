scriptDirectory = arg [1]
Core = require (scriptDirectory .. "Core")
Core.LoadCoreScripts ()
Core.LoadBindables ()
Core.LoadConfiguration (arg [2]:gsub (".lua", ""))
Core.ParseInputFiles ()
Core.PrintParsedBindables ()
