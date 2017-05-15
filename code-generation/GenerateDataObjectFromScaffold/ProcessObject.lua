require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

_object_varsInitializationItemTemplate =
[[    ${var.name} (${var.default}),
]]

_object_varsAttributesItemTemplate =
[[    ${var.registrationMacro} ("${var.description}", Get${var.prettyName}, Set${var.prettyName}, ${var.type}, ${var.attributeDefault}, Urho3D::AM_DEFAULT);
]]

_object_varsArrayAttributesItemTemplate =
[[    ${var.registrationMacro} ("${var.description}", Get${var.prettyName}Attribute, Set${var.prettyName}Attribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, ${var.shortName}StructureElementsNames, Urho3D::AM_DEFAULT);
]]

_object_varsGetterAndSetterItemTemplate =
[[${var.type} ${className}::Get${var.prettyName} () const
{
    return ${var.name};
}

void ${className}::Set${var.prettyName} (${var.setterArgType} ${var.shortName})
{
    ${var.name} = ${var.shortName};
}

]]

_object_varsArrayGetterAndSetterItemTemplate =
[[Urho3D::VariantVector ${className}::Get${var.prettyName}Attribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (${var.name}.Size ()));
    for (int index = 0; index < ${var.name}.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (${var.name}.At (index)));
    }
    return variantVector;
}

void ${className}::Set${var.prettyName}Attribute (const Urho3D::VariantVector &${var.shortName})
{
    ${var.name}.Clear ();
    if (!${var.shortName}.Empty ())
    {
        int requestedSize = ${var.shortName}.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < ${var.shortName}.Size ())
                {
                    ${var.name}.Push (${var.shortName}.At (index + 1).Get${var.arrayValueBindingsType} ());
                }
                else
                {
                    ${var.name}.Push (${var.arrayValueType} ());
                }
            }
        }
    }
}

]]

function ProcessObjectLine (readedLine)
    if readedLine:find ("//@Insert vars initialization") ~= nil then
        WriteObjectVarsInitialization ()
    elseif readedLine:find ("//@Insert attributes registration") ~= nil then
        WriteObjectAttributesRegistration ()
    elseif readedLine:find ("//@Insert vars getters and setters")~= nil then
        WriteObjectVarsGettersAndSetters ()
    else
        _objectFile:write (readedLine .. "\n")
    end
end

function WriteObjectVarsInitialization ()
    local code = ""
    for index, var in ipairs (_vars) do
        code = code .. ProcessTemplate (_object_varsInitializationItemTemplate, ConstructVarTemplateVars (var))
    end
    code = code:sub (1, code:len () - 2)
    _objectFile:write (code .. "\n")
end

function WriteObjectAttributesRegistration ()
    for index, var in ipairs (_vars) do
        if IsArrayType (var.type) then
            _objectFile:write (ProcessTemplate (_object_varsArrayAttributesItemTemplate, ConstructVarTemplateVars (var)))
        else
            _objectFile:write (ProcessTemplate (_object_varsAttributesItemTemplate, ConstructVarTemplateVars (var)))
        end
    end
end

function WriteObjectVarsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        _objectFile:write (ProcessTemplate (_object_varsGetterAndSetterItemTemplate, ConstructVarTemplateVars (var)))
        if IsArrayType (var.type) then
            _objectFile:write (ProcessTemplate (_object_varsArrayGetterAndSetterItemTemplate, ConstructVarTemplateVars (var)))
        end
    end
end
