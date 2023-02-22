![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/10/11)

# The Maya pipeline

With Ramses and its Maya Add-on, the Maya pipeline is easy-to-use but you have access to all parameters when publishing and importing files. Using presets and/or the *Ramses Client Application* you can completely automate the pipeline according to your specific needs.

**▹ Read more about the [pipeline](../../pipeline/pipeline.md).**

The Maya Add-on can export (publish), import and update the assets across all your Maya projects, using different file formats (*maya*, *alembic*, etc.); but note that sometimes it's easier and quicker to just save a project from a step as a new project for the next step, instead of publishing and importing almost everything.

This is a typical Maya production pipeline, using *Ramses* (or not).

| Step | Import | Publish |
| --- | --- | --- |
| **Modeling** | `.obj`, `.fbx`, etc. from a sculpting application like ZBrush | • `.obj`, `.fbx`, `.abc`... for a texturing application.<br />• `.mb`, `.ma` or `.abc` for the rigging and the shading step.<br />• `.mb` with just the shaders if the mesh is published as an alembic for the rigging step. |
| **Rigging** | • `.mb`, `.ma` or `.abc` from the modeling step, optionally as a *reference*.<br />• `.mb` with just the shaders if the mesh was published as an alembic from the modelling step, optionally as a *reference*. | • `.ma` for the layout step. |
| **Shading** | • `.mb`, `.ma` or `.abc` from the modeling step.<br />• `.mb` with just the shaders if the mesh was published as an alembic from the modelling step. | • `.mb` with just the shaders. |
| **Layout** | • `.mb`, `.ma` or `.abc` of the assets from the modeling step.<br />• `.mb` with just the shaders if the mesh was published as an alembic from the modelling step.<br />• `.ma` from the rigging step as a *reference*. | Nothing to publish, the project can be saved as a new animation project, and a new lighting project. |

**▹ Configure the [pipeline in the *Ramses Client Application*](../../pipeline/pipeline.md).**

- [Scene setup](maya-scenesetup.md)
- [Publish settings](maya-publish.md)
- [Import settings](maya-import.md)