shared abstract class ScriptObjectWithBeforeStop : ScriptObject
{
    ScriptObjectWithBeforeStop ()
    {

    }

    ~ScriptObjectWithBeforeStop ()
    {

    }

    void Start ()
    {
        SubscribeToEvent ("NodeRemoved", "OnNodeRemoved");
    }

    void OnNodeRemoved (StringHash eventType, VariantMap &eventData)
    {
        Node @removedNode = eventData ["Node"].GetPtr ();
        if (node is removedNode)
        {
            Scene @lastScene = eventData ["Scene"].GetPtr ();
            BeforeStop (lastScene, removedNode);
        }
    }

    void BeforeStop (Scene @lastScene, Node @lastNode)
    {
        //! Should be implemented in inheritors.
    }
}
