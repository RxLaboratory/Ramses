![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/10/11)

# Scene setup / General parameters

The Maya Add-on is able to automatically setup the maya scenes for the assets and the shots, to select the right camera, set the correct rendering resolution, set the correct timeline start and end time...

Click the ![](../../img/icons/ramsetupscene.png){ style="width:32px"} ***Scene Setup*** button to setup the scene.

![](/img/maya/scenesetup.png)

These parameters are retrieved from the shot or asset and project data; you can also customize these settings for each step. You can add these settings via the Ramses Application, using the [step editor](../client/steps.md)'s general settings, like in this example:

![](/img/client/stepedition-settings.png){style="width:200px"}

This text uses the [*Yaml*](https://yaml.org/) syntax.

These are the available settings:

```yaml
shot:
  handle_in: 24 # The handle to add before the shot start time, in frames
  handle_out: 24 # The handle to add after the shot end time, in frames
  first_image_number: 101 # The number of the first frame in the timeline (including the handle_in)
color_management:
  ocio_path: "C:\Path\To\config.ocio"
  rendering_space: "ACEScg"
  display_space: "sRGB"
  view_transform: "ACES 1.0 SDR-video"
```

All parameters are optional and can be omitted to let the scene use its own/default settings.

You can use the `ocio_path` parameter to set a specific *OCIO* file for a specific step, although it's not recommended to use multiple *OCIO* configs, and one should just set a single *OCIO* config and never change it.