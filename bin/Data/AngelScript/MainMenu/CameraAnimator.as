class CameraAnimator : ScriptObject
{
    protected ValueAnimation @positionAnimation;
    protected ValueAnimation @rotationAnimation;

    CameraAnimator ()
    {
        positionAnimation = ValueAnimation ();
        rotationAnimation = ValueAnimation ();
    }

    ~CameraAnimator ()
    {

    }

    void Start ()
    {
        positionAnimation.name = "Position";
        positionAnimation.valueType = VAR_VECTOR3;
        positionAnimation.SetKeyFrame (0.0f, Variant (Vector3 (5, 10, 5)));
        positionAnimation.SetKeyFrame (10.0f, Variant (Vector3 (12, 10, -12)));
        positionAnimation.SetKeyFrame (20.0f, Variant (Vector3 (-10, 10, -10)));
        positionAnimation.SetKeyFrame (30.0f, Variant (Vector3 (-15, 10, 13)));
        positionAnimation.SetKeyFrame (40.0f, Variant (Vector3 (5, 10, 5)));

        rotationAnimation.name = "Rotation";
        rotationAnimation.valueType = VAR_QUATERNION;
        rotationAnimation.SetKeyFrame (0.0f, Variant (Quaternion (45.0f, 0.0f, 0.0f)));
        rotationAnimation.SetKeyFrame (10.0f, Variant (Quaternion (45.0f, 120.0f, 0.0f)));
        rotationAnimation.SetKeyFrame (20.0f, Variant (Quaternion (45.0f, 150.0f, 0.0f)));
        rotationAnimation.SetKeyFrame (30.0f, Variant (Quaternion (45.0f, 60.0f, 0.0f)));
        rotationAnimation.SetKeyFrame (40.0f, Variant (Quaternion (45.0f, 0.0f, 0.0f)));

        ObjectAnimation @objectAnimation = ObjectAnimation ();
        objectAnimation.AddAttributeAnimation ("Position", positionAnimation);
        objectAnimation.AddAttributeAnimation ("Rotation", rotationAnimation);
        node.objectAnimation = objectAnimation;
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }
}
