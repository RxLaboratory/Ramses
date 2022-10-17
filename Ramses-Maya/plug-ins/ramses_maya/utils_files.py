import ramses

RAMSES = ramses.Ramses.instance()

def get_current_project( filePath ):
    """Returns the RamProject for this file"""
    nm = ramses.RamFileInfo()
    nm.setFilePath( filePath )
    # Set the project and step
    project = None
    if nm.project != '':
        project = RAMSES.project( nm.project )
        RAMSES.setCurrentProject( project )
    # Try to get the current project
    if project is None:
        project = RAMSES.currentProject()

    return project

def get_step_for_file( filePath ):
    """Returns the RamStep for this file"""
    project = get_current_project( filePath )
    nm = ramses.RamFileInfo()
    nm.setFilePath( filePath )
    if nm.step != '':
        return project.step( nm.step )
    return None