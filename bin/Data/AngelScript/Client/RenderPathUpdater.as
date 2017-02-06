class RenderPathUpdater : ScriptObject
{
    void UpdateShader ()
    {
        RenderPath @renderPath = renderer.viewports [0].renderPath;
        RenderPathCommand mapMaskCommand;
        int index = 0;
        bool found = false;
        while (index < renderPath.numCommands and !found)
        {
            RenderPathCommand command = renderPath.get_commands (index);
            if (command.tag == "MapMask")
            {
                mapMaskCommand = command;
                found = true;
            }

            if (!found)
            {
                index++;
            }
        }

        MapMaskUpdater @maskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskCommand.shaderParameters ["MapMinPoint"] = Variant (maskUpdater.mapMinPoint);
        mapMaskCommand.shaderParameters ["MapMaxPoint"] = Variant (maskUpdater.mapMaxPoint);
        renderPath.set_commands (index, mapMaskCommand);
    }

    RenderPathUpdater ()
    {

    }

    ~RenderPathUpdater ()
    {

    }

    void Start ()
    {
        XMLFile @renderPathXML = cache.GetResource ("XMLFile", "RenderPath/Ingame_DeferredHWDepth.xml");
        renderer.viewports [0].renderPath.Load (renderPathXML);
        renderer.viewports [0].renderPath.SetEnabled ("MapMask", true);
        UpdateShader ();
    }

    void Update (float timeStep)
    {
        UpdateShader ();
    }

    void Stop ()
    {

    }
}
