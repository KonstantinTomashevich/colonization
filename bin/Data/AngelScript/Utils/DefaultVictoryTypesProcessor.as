void Process (float timeStep, const VictoryTypesProcessorScriptDataAccessor @dataAccessor, VariantMap &out output)
{
    VariantMap victoryProgressData;
    victoryProgressData ["ByColonizedSquare"] = Variant (ProcessVictoryByColonizedSquare (dataAccessor));
    output = victoryProgressData;
}

VariantMap ProcessVictoryByColonizedSquare (const VictoryTypesProcessorScriptDataAccessor @dataAccessor)
{
    VariantMap victoryProgressData;
    victoryProgressData [PLAYER_INFO_VICTORY_TYPE_NAME_KEY] = "by colonized square";
    victoryProgressData [PLAYER_INFO_VICTORY_TYPE_INFO_KEY] = "What makes people rich in New World? " +
                                                              "Many things, but primary one is land. " +
                                                              "${PlayerName} occupied more than 70% of territory!";

    float playersLand = 0.0f;
    float totalLand = 0.0f;
    for (int index = 0; index < dataAccessor.districtsCount; index++)
    {
        const District @district = dataAccessor.GetDistrictByIndex (index);
        if (not district.isSea and not district.isImpassable)
        {
            float farmingSquare = district.farmingSquare;
            float forestsSquare = district.forestsSquare;
            totalLand += (farmingSquare + forestsSquare);
            if (district.hasColony and district.colonyOwnerName == dataAccessor.playerInfo.name)
            {
                playersLand += (farmingSquare + forestsSquare);
            }
        }
    }
    victoryProgressData [PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY] = (playersLand * 100.0f) / (totalLand * 0.7f);
    return victoryProgressData;
}
