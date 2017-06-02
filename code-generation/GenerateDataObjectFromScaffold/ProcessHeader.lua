require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

_header_varsListItemTemplate =
[[    ${var.type} ${var.name};
]]

_header_varsGettersAndSettersItemTemplate =
[[    ${var.type} Get${var.prettyName} () const;
    void Set${var.prettyName} (${var.setterArgType} ${var.shortName});

]]

_header_varsGettersAndSettersWithASBindGenItemTemplate =
[[    //@${asBindGenCommand} Function OverrideName=get_${var.shortName}
    ${var.type} Get${var.prettyName} () const;
    //@${asBindGenCommand} Function OverrideName=set_${var.shortName}
    void Set${var.prettyName} (${var.setterArgType} ${var.shortName});

]]

_header_varsGettersAndSettersArrayAttributeTemplate =
[[    Urho3D::VariantVector Get${var.prettyName}Attribute () const;
    void Set${var.prettyName}Attribute (const Urho3D::VariantVector &${var.shortName});

]]

function ProcessHeaderLine (readedLine)
    if readedLine:find ("//@Insert var list") ~= nil then
        WriteHeaderVarsList ()
    elseif readedLine:find ("//@Insert vars getters and setters") ~= nil then
        WriteHeaderVarsGettersAndSetters ()
    else
        _headerFile:write (readedLine .. "\n")
    end
end

function WriteHeaderVarsList ()
    for index, var in ipairs (_vars) do
        _headerFile:write (ProcessTemplate (_header_varsListItemTemplate, ConstructVarTemplateVars (var)));
    end
end

function WriteHeaderVarsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        local template = _header_varsGettersAndSettersItemTemplate
        if _asBindGenCommand ~= nil then
            template = _header_varsGettersAndSettersWithASBindGenItemTemplate
        end
        _headerFile:write (ProcessTemplate (template, ConstructVarTemplateVars (var)));

        if IsArrayType (var.type) then
            _headerFile:write (ProcessTemplate (_header_varsGettersAndSettersArrayAttributeTemplate, ConstructVarTemplateVars (var)));
        end
    end
end
