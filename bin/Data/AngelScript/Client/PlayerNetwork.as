class PlayerNetwork : ScriptObject
{
    PlayerNetwork ()
    {
        
    }
    
    ~PlayerNetwork ()
    {
        
    }
    
    void Start ()
    {
        SubscribeToEvent ("NetworkMessage", "HandleNetworkMessage");
    }
    
    void Update (float timeStep)
    {
        
    }
    
    void Stop ()
    {
        
    }
    
    void HandleNetworkMessage (StringHash eventType, VariantMap &eventData)
    {
        if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_SEND_PLAYER_STATS)
            HandlePlayerStatsMessage (eventData);
    }
    
    void HandlePlayerStatsMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        float points = buffer.ReadFloat ();
        float gold = buffer.ReadFloat ();
        node.parent.vars ["gold"] = gold;
        node.parent.vars ["points"] = points;
    }
};
