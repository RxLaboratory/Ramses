# Maya scripting and automation with Ramses

Ramses makes it easy to automate some work in Maya, using Python.

You can write some scripts to automate tasks, and you can also connect scripts to Rames *events* to automatically run scripts during these events, for example each time a scene is opened in Maya.

Here are a few examples.

## Examples

### Show an alert when the user opens a scene with Ramses

This script will be run each time a scene is opened with Ramses; the condition at the beginning of the functions makes sure the alert is shown only if it's a rendering or lighting step.

```py
# Affiche un message à l'ouverture d'une scène de render ou de lighting
from maya import cmds

def on_open(file_path, item, step):
    
    step_id = step.shortName()
    
    if step_id in ("Render", "Light"):
        cmds.confirmDialog(
            title="Attention",
            message="Les render layers FX et Végétaux sont en 4K parce qu'on est des fous.\nN'oublie pas de les désactiver si tu n'en as pas besoin !",
            button=["OK"],
            icon="warning"
            )
```

### Update all assets

This is a script which will update all the assets in all the scenes of a given step. It can just be run from Maya as any other script (if the Ramses add-on is correctly installed of course).

```py
"""
    Automatically updates ALL items in ALL the scenes of a given step
    This is given as an example and SHOULD NOT be used in production,
    Unless you really know what you're doing.
"""

from maya import cmds # pylint: disable=import-error
import ramses as ram # pylint: disable=import-error

# Set a log file
log_file = "D:/Ramses_auto_update_log.txt"

# Get the Ramses instance
RAMSES = ram.Ramses.instance()

# A simple function to log stuff
def log( msg ):
    """Log to log_file and the console"""
    ram.log("Auto-update: " + msg)
    with open(log_file,"a", encoding='utf8') as f:
        f.write(msg + "\n")

# Everything in a function makes it easier to manipulate
def auto_update( step_id = 'layout' ):
    """The main auto update function."""

    # Get the current project
    project = RAMSES.currentProject()

    # Reinit the log
    with open(log_file, 'w', encoding='utf8') as f:
        f.write("Starting auto-update...\n")

    # If there's no project, can't do nothing!
    if not project:
        log("No current project")
        return

    # Get the step
    step = project.step(step_id)

    # If the step doesn't exist, can't do nothing!
    if not step:
        log("Invalid step (" + step_id + ")")
        return

    # List all the items
    items = []
    if step.stepType() == ram.StepType.SHOT_PRODUCTION:
        items = project.shots()
    elif step.stepType() == ram.StepType.ASSET_PRODUCTION:
        items = project.assets()

    # Nothing to update
    if len(items) == 0:
        log("Empty step, or unsupported step type (pre or post production)")
        return

    # For all items
    # A counter
    i = 0
    for ram_item in items:

        # Open scene (try both ma and mb)
        path = ram_item.stepFilePath(extension="mb",step=step_id)
        if path == "":
            path = ram_item.stepFilePath(extension="ma",step=step_id)
        if path == "":
            continue

        i = i+1
        log("Opening " + str(ram_item))
        log("File: " + path)

        # Maya open command
        cmds.file( path, open=True )
        log("Scene loaded")

        # Use the ramses command to update all imported items
        cmds.ramUpdate( updateAll=True )
        log("Updated!")

        # Save as a new version
        cmds.ramSaveVersion( updateStatus=False )
        log("Saved scene!\n")

    # Finished!
    log("Updated " + str(i) + " scenes.")

if __name__ == "__main__":
    # Run the function
    # Just change the step id if you wish
    auto_update('Rig')

```