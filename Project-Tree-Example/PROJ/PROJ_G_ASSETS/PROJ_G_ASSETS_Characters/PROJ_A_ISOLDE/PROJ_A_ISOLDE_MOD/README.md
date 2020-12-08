# Modeling folder for the *Isolde* asset

The name of the asset follows this convention:
    projShortname_ramType_objectShortName_stepShortName_optionalComment.extension
    ...which is only used for Assets and Shots.

    eg: PROJ_A_ISOLDE_RIG_facialTest.blend
        PROJ_S_012_ANIM_crowd.blend
        PROJ_S_012_ANIM.blend
        PROJ_G_SCENAR.txt

The comment is optional and only serves to create secondary versions of the object, for testing or separating work.
For example when animating a shot, you could create a secondary file for the animation of a ball, another file for the animation of a crowd, another one for the animation of the main character... and then pull them into the main file.

The version number is not to be found on the working file but rather in the version files located in the subfolder "ramses_versions"

The subfolder "ramses_publish" only contains the latest publish and its associated files (such as an alembic for the modeling step).
Publish files have the same name as their working file and therefore have no version number. Their versions will be found in the "ramses_versions" folder.
Secondary files (those with comments) can also be published.