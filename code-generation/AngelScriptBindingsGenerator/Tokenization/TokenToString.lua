if Tokens == nil then
    Tokens = require (scriptDirectory .. "Tokenization/Tokens")
end

function TokenTypeToString (token)
    if token.type == Tokens.Operator then
        return "Operator"
    elseif token.type == Tokens.TypeOrName then
        return "Type Or Name"
    elseif token.type == Tokens.Command then
        return "Command"
    elseif token.type == Tokens.EndOfLine then
        return "End Of Line"
    elseif token.type == Tokens.EndOfFile then
        return "End Of File"
    else
        return "Unknown Token"
    end
end

function TokenToString (token)
    local tokenValue = token.value
    if tokenValue == nil then
        tokenValue = "nil"
    end
    return "\"" .. tokenValue .. "\"(" .. TokenTypeToString (token) .. ")"
end
return TokenToString, TokenTypeToString
