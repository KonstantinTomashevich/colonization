class RenderPathUpdater : ScriptObject
{
    void UpdateMaterial (Material @material)
    {
        MapMaskUpdater @maskUpdater = scene.GetComponent ("MapMaskUpdater");
        material.shaderParameters ["DefaultColor"] = Variant (Color (0.5f, 0.5f, 0.5f, 1.0f));
        material.shaderParameters ["MapMinPoint"] = Variant (maskUpdater.mapMinPoint);
        material.shaderParameters ["MapMaxPoint"] = Variant (maskUpdater.mapMaxPoint);
        material.shaderParameters ["FogOfWarEnabled"] = Variant (1);
        material.textures [TU_ENVIRONMENT] = maskUpdater.fogOfWarMaskTexture;
    }

    void UpdateAllMaterials ()
    {
        Array <Resource @> materials = cache.GetResources (StringHash ("Material"));
        for (uint index = 0; index < materials.length; index++)
        {
            Material @material = materials [index];
            if (material.shaderParameters ["UnderMapMask"].GetInt () == 1)
            {
                UpdateMaterial (material);
            }
        }
    }

    RenderPathUpdater ()
    {

    }

    ~RenderPathUpdater ()
    {

    }

    void Start ()
    {
        UpdateAllMaterials ();
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }
}
