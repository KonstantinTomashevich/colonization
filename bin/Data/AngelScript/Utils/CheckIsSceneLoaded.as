shared bool CheckIsSceneLoaded (Scene @scene_)
{
    if (scene_.vars ["ReplicatedNodesCount"].GetInt () != 0)
    {
        Array <Node @> children = scene_.GetChildren (true);
        int replicated = 0;
        for (int index = 0; index < children.length; index++)
            if (children [index].id < FIRST_LOCAL_ID)
                replicated++;
        return (replicated == scene_.vars ["ReplicatedNodesCount"].GetInt ());
    }
    else
        return false;
}
