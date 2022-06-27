![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/06/27)

# The Maya pipeline

With Ramses and its Maya Add-on, the Maya pipeline is easy-to-use but you have access to all parameters when publishing and importing files. Using presets and/or the *Ramses Client Application* you can completely automate the pipeline according to your specific needs.

**▹ Read more about the [pipeline](../../pipeline/pipeline.md).**

**▹ Configure the [pipeline in the *Ramses Client Application*](../../pipeline/pipeline.md).**

## Setting the publishing parameters

Use the ![](../../img/icons/rampublishsettings.png){ style="width:28px"} ***Publish settings*** button from the *Add-on*'s shelf to access all available parameters for publishing in Maya. From this dialog, you can build your own presets, or set some settings as defaults for any specific [step](../../pipeline/pipeline.md) in your [pipeline](../../pipeline/pipeline.md).

![Publish settings: Format](/img/maya/publishsettings_format.png)  

In the first tab, select the step for which your setting the publishing parameters, and optionally select an existing preset.

Presets are stored in a folder inside the Maya module folder. You can save or load a preset with `Edit ► Save preset...` and `Edit ► Load preset...`. When a preset is saved in the default folder, it is listed in the box in this tab.

*Ramses* provides a few presets by default, they're good examples of what you can do.

- `Animation.abc` can be used for publishing animation and cameras as a lightweight alembic file.
- `Arnold.ass` is a simple preset to publish the selected nodes as an *Arnold Scene Source*.
- `Geometry.abc` is meant for publishing mesh nodes without animation. Shaders are automatically published separately as a *Maya Binary*.
- `Rig.ma` can be used to publish rigged characters and props, and more generally any kind of Maya scene, as a *Maya ASCII* file.
- `Shaders.mb` publishes only the shaders from the selected nodes.
- `Simple.mb` is a general preset which can be used with any kind of scene.

You can safely delete these presets from the preset folder if you don't need them.

The text area on the right shows the preset as a text. This text can be saved or used directly in the publish settings of any step in the *Ramses Client Application*, to be automatically used when publishing files for the given step. In this case, this settings window will not be shown anymore when publishing files, unless the user checks the *Edit publish settings* box when saving the scene.

If the *Ramses Client Application* is running, you don't necessarily need to use presets; clicking on the `Save settings` button will set the current settings to the selected step. They can then also be [edited directly in the *Ramses Client Application*](../../components/client/steps.md).

This text uses the [*Yaml*](https://yaml.org/) syntax, which can easily be edited manually.

### Pre-Publish settings

Before publishing, *Ramses* can run some checks and clean the scene.

![Pre-Publish](../../img/maya/publishsettings_prebublish.png)

- **Import all references**, to avoid nested references
- **Remove namespaces**, to simplify node names when importing
- **Remove all hidden nodes**
- **Delete all histories**
- Leave only **a single shape per transform node**. Extra shapes should not be needed.
- **Remove all empty groups**
- **Remove all keyframes** and animation
- **Types**: you can choose to keep (white list) or remove (black list) specific node types.  
  To check the types of the nodes to complete the list, use the Mel/Python command `nodeType`.
- **Freeze transformations of all the nodes**, except those containing specific strings in their name.

### Maya scene

![](../../img/maya/publishsettings_maya.png)

- Select either ***Binary*** or ***ASCII file***.
- **All hidden node visibilities** can be automatically **locked**.
- **All transformations can be locked**.
- You can select **how joints will be displayed** in the published file.

### Maya scene - Shader library

![](../../img/maya/publishsettings_mayashaders.png)

- Select either ***Binary*** or ***ASCII file***.

When publishing a shader library, *Ramses* keeps the list of nodes the shaders are applied to, and is able to re-assign them when importing the library in any scene.

### Alembic

![](../../img/maya/publishsettings_alembic.png)

- You can choose to **keep only renderable nodes** and remove all other shapes.
- Use the **handles** control to add some time before and after the scene's time range.
- You can change the **frame step** to make the exported file lighter or more detailed (e.g. for precise motion blur).
- You can automatically **filter Euler rotations**.

### Arnold Scene Source

There are no options for this format.

## Importing assets

When importing or replacing assets, there are a few options. The same way publish settings can use presets or be set directly in the *Ramses Client Application*, *Ramses* will look for presets and default settings when importing the assets.

![](../../img/maya/importsettings_preset.png)

In the first tab, you can optionally select an existing preset.

Presets are stored in a folder inside the Maya module folder. You can save or load a preset with `Edit ► Save preset...` and `Edit ► Load preset...`. When a preset is saved in the default folder, it is listed in the box in this tab.

*Ramses* provides a few presets by default, they're good examples of what you can do.

- `Alembic - Reference and shaders` imports an alembic file as a reference, along with *Maya* shader libraries.
- `Alembic and shaders` imports an alembic file, and *Maya* shader libraries as a reference.
- `Maya - Lock transform` imports a Maya file, and locks the transformations of the imported asset.
- `Maya - Reference` references a Maya file.
- `Maya` simply imports a Maya file.

You can safely delete these presets from the preset folder if you don't need them.

### Format settings

For each format to import, you can set some settings.

![](../../img/maya/importsettings_format.png)

The text on the right can be copied to any [pipe type](../../components/client/pipetypes.md) in the *Ramses Client Application*. When importing an asset, *Ramses* will first look for these settings, and if they're found, it won't show the settings dialog unless the user checked the box `Edit import options` when importing the asset.

- Files can be imported as **references**.
- **All transformations** of all nodes can be *locked*.
- When importing a shader library, **shaders can automatically be applied** on the selected nodes.
  *Ramses* will use the node names to apply the shaders: the selected nodes must have the *exact same name* as the nodes in the scene used to publish the shaders.