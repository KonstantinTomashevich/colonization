require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

_bindings_varsGettersAndSettersItemTemplate =
[[    engine->RegisterObjectMethod ("${className}", "${var.bindingsType} get_${var.shortName} () const", asMETHOD (${className}, Get${var.prettyName}), asCALL_THISCALL);
    engine->RegisterObjectMethod ("${className}", "void set_${var.shortName} (${var.bindingsType} ${var.shortName})", asMETHOD (${className}, Set${var.prettyName}), asCALL_THISCALL);

]]

_bindings_varsArrayGettersAndSettersItemTemplate =
[[    engine->RegisterObjectMethod ("${className}", "Array <${var.arrayValueBindingsType}> @get_${var.shortName} () const", asFUNCTION (${className}_Get${var.prettyName}), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("${className}", "void set_${var.shortName} (Array <${var.arrayValueBindingsType}> @${var.shortName})", asFUNCTION (${className}_Set${var.prettyName}), asCALL_CDECL_OBJFIRST);

]]

_bindings_varsArrayGettersAndSettersFunctionsItemTemplate =
[[Urho3D::CScriptArray *${className}_Get${var.prettyName} (${className} *object)
{
    ${var.type} array = object->Get${var.prettyName} ();
    return ${var.toArrayConversionFunction} <${var.arrayValueTypeWithoutPtr}> (array, "Array <${var.arrayValueBindingsType}>");
}

void ${className}_Set${var.prettyName} (${className} *object, Urho3D::CScriptArray *array)
{
    object->Set${var.prettyName} (${var.arrayConversionFunction} <${var.arrayValueTypeWithoutPtr}> (array));
}
]]

function ProcessBindingsLine (readedLine)
    if readedLine:find ("//@Insert getters and setters bindings") ~= nil then
        WriteBindingsGettersAndSetters ()
    elseif readedLine:find ("//@Insert arrays getters and setters") ~= nil then
        WriteArraysGettersAndSetters ()
    else
        _bindingsFile:write (readedLine .. "\n")
    end
end

function WriteBindingsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        local shortenedName = var.name:sub (1, var.name:len () - 1)
        if IsArrayType (var.type) then
            _bindingsFile:write (ProcessTemplate (_bindings_varsArrayGettersAndSettersItemTemplate, ConstructVarTemplateVars (var)));
        else
            _bindingsFile:write (ProcessTemplate (_bindings_varsGettersAndSettersItemTemplate, ConstructVarTemplateVars (var)));
        end
    end
end

function WriteArraysGettersAndSetters ()
    for index, var in ipairs (_vars) do
        if IsArrayType (var.type) then
            _bindingsFile:write (ProcessTemplate (_bindings_varsArrayGettersAndSettersFunctionsItemTemplate, ConstructVarTemplateVars (var)));
        end
    end
end
