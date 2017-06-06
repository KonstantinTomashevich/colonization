scriptDirectory = arg [0]:gsub ("Main.lua", "")
require (scriptDirectory .. "Globals")
require (scriptDirectory .. "ProcessConfiguration")
require (scriptDirectory .. "ProcessVars")
require (scriptDirectory .. "ProcessHeader")
require (scriptDirectory .. "ProcessObject")
require (scriptDirectory .. "ProcessBindings")

_inputFileName = arg [1]
_outputHeaderFileName = arg [2]
_outputObjectFileName = arg [3]
_outputBindingsObjectFileName = arg [4]
if _inputFileName == nil or _outputHeaderFileName == nil or _outputObjectFileName == nil or _outputBindingsObjectFileName == nil then
    Log ("Incorrect input arguments!\nExpected arguments:\n1th -- input file name,\n"..
        "2th -- output header file name,\n3th -- output C++ object file name,\n" ..
        "4th -- output bindings C++ file name!");
    return 1
end

if arg [5] ~= nil then
    printOutputFile = io.open (arg [5], "w+")
    Log = function (toPrint)
        if toPrint ~= nil then
            printOutputFile:write (toPrint .. "\n")
        end
    end
else
    Log = function (toPrint)
        print (toPrint)
    end
end

Log ("Input scaffold: " .. _inputFileName .. "\nOutput header: " .. _outputHeaderFileName ..
    "\nOutput object: " .. _outputObjectFileName ..
    "\nOutput bindings: " .. _outputBindingsObjectFileName)
Log ("WARNING: Currently, only one level Urho3D arrays are supported!")

-- Load files
_headerFile = io.open (_outputHeaderFileName, "w+")
_objectFile = io.open (_outputObjectFileName, "w+")

-- If output binding file name starts with "ASBindGenCommand=", detect it as ASBindGen command name
-- and write ASBindGen comments instead of bindings object.
if _outputBindingsObjectFileName:find ("ASBindGenCommand=") == 1 then
    _asBindGenCommand = _outputBindingsObjectFileName:sub (("ASBindGenCommand="):len () + 1, _outputBindingsObjectFileName:len ())
else
    _bindingsFile = io.open (_outputBindingsObjectFileName, "w+")
end

for readedLine in io.lines (_inputFileName) do
    if _processMode == "Configuration" then

        if readedLine:find ("tab:") == 1 then
            ReadTab (readedLine)
        elseif readedLine:find ("className:") == 1 then
            ReadClassName (readedLine)
        else
            UpdateProcessMode (readedLine, true)
        end

    elseif _processMode == "Vars" then
        UpdateProcessMode (readedLine, false)
        if _processMode == "Vars" and readedLine:len () > 10 then
            ReadVar (readedLine)
        end

    elseif _processMode == "Header" then
        UpdateProcessMode (readedLine, false)
        if _processMode == "Header" then
            ProcessHeaderLine (readedLine)
        end

    elseif _processMode == "Object" then
        UpdateProcessMode (readedLine, false)
        if _processMode == "Object" then
            ProcessObjectLine (readedLine)
        end

    elseif _processMode == "Bindings" and _asBindGenCommand == nil then
        UpdateProcessMode (readedLine, false)
        if _processMode == "Bindings" then
            ProcessBindingsLine (readedLine)
        end
    end
end

_headerFile:close ()
_objectFile:close ()
if _bindingsFile ~= nil then
    _bindingsFile:close ()
end

if printOutputFile ~= nil then
    printOutputFile:close ()
end
