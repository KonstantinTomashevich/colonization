--- Defaults.
_tab = "    "
_className = "Default_className"

--- Elemental types for correct setters generation.
_elementalTypes = {}
_elementalTypes ["float"] = 1
_elementalTypes ["double"] = 1
_elementalTypes ["bool"] = 1
_elementalTypes ["int"] = 1
_elementalTypes ["unsigned"] = 1
_elementalTypes ["long"] = 1
_elementalTypes ["unsigned int"] = 1
_elementalTypes ["unsigned long"] = 1

--- Current read mode.
_processMode = "Configuration"
function UpdateProcessMode (readedLine, fallbackToConfiguration)
    if readedLine:find ("vars") == 1 then
        _processMode = "Vars"
        Log ("New read mode: " .. _processMode .. ".")
    elseif readedLine:find ("header") == 1 then
        _processMode = "Header"
        Log ("New read mode: " .. _processMode .. ".")
    elseif readedLine:find ("object") == 1 then
        _processMode = "Object"
        Log ("New read mode: " .. _processMode .. ".")
    elseif readedLine:find ("bindings") == 1 then
        _processMode = "Bindings"
        Log ("New read mode: " .. _processMode .. ".")
    elseif (fallbackToConfiguration) then
        _processMode = "Configuration"
    end
end

--- Readed vars list.
_vars = {}

--- Files
_headerFile = nil
_objectFile = nil
_bindingsFile = nil
