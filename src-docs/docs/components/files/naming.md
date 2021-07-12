# File and folder naming scheme

*Ramses* uses a strict and specific naming convention. This naming scheme is based on the _**ID**s_ of the elements, which are defined by the administrator users in the *Ramses Client*.

!!! Extensibility
    We provide [a set of regular expressions](../../dev/files/naming.md) to help developpers manipulate this naming scheme.

## IDs (short names)

In *Ramses*, all elements (users, assets, shots, projects...) have two names: the actual name used in the user interface and displayed anywhere there is enough room, and an *ID* which is no longer than 10 characters (but which may be the same as the actual name). *IDs* are unique (while names may not be unique, although it's good practice to keep them unique too) and they are used as identifiers.

While names can contain a lot of different characters, *IDs* can consist only of alphanumeric characters (without any special character or accent), and the minus `-` character.

| What | How | Where |
|---|---|---|
| **Name** | up to **256 characters** in:<br /> `[a-z]`, `[A-Z]`, `[0-9]`, `-`, and space. | Used in **user interfaces** where there is enough room. |
| **ID** | up to **10 characters** in:<br /> `[a-z]`, `[A-Z]`, `[0-9]`, `-`. | Used in **file and folder** names, and in user interfaces where there is limited room. |

!!! Extensibility
    In the [*API*](../../dev/index.md), the *IDs* are called "short names".

## Naming scheme

This is the complete naming scheme, for all files and folders:

`projectID_A|S|G(_assetID|shotID)_stepID(_resource name)(_stateVersion)(.extension)`

Explanations:

- The name is divided into blocks, separated by the underscore `_` character.
- Optionnal blocks are shown in parenthesis `()`
- The vertical bar `|` characters means "or" and represents a choice in a list of possibilities.

**A file name must include at least 3 blocks**: *projectID*, *Type* (*A*, *S* or *G*), and *stepID* and can include up to 6 blocks if adding the *assetId* or *shotID*, the resource name, and the version.

### Blocks

- `projectID` is the ID of the project.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed.
- `A|S|G` tells if the file or folder refers to an Asset (*A*), a shot (*S*) or a general item (*G*).
- `assetID` is the ID of the asset, if the previous block is *A*.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed.
- `shotID` is the ID of the shot, if the previous block is *S*.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed.
- `stepID` is the ID of the production step.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed.
- `resource name` describes the specific resource this file or folder refers to.  
    Between 1 and 30 characters consisting only of `a-z`, `0-9` and `-` characters and may contain spaces.
    Resource names cannot start with a status ID followed by a number (e.g. *v01* or *wip2*) nor a number alone.
- `stateVersion` contains the state and version number.  
    The state is either the default `v` or the ID of the state (e.g. "wip", "ok", etc.), and the version includes one or more numbers. No other character allowed.
- `extension` is the file extension.  
    The extension may include multiple dots, for example: *.tar.gz*

### Examples

`TI_G_ADMIN_Budget estimation.xls`

`TI_G_RIG_Template.blend`

`TI_A_TRISTAN_RIG.blend`

`TI_A_TRISTAN_RIG_WIP001.blend`

`TI_A_TRISTAN_ANIM_walk cycle.blend`

`TI_A_TRISTAN_ANIM_walk cycle_CHK011.blend`

## Project media files

Media files exported from the project have a specific naming scheme.

`projectID_codec(+colorspace)_widthxheight(p|i)_fps(_language)_(subtitlesst).ext`

### Blocks

- `projectID` is the ID of the project.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed.
- `codec` is the name of the codec.
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed. No dot allowed (e.g. *h.264* must be named *h264*).
- `colorspace` represents the colorspace used if it's not the most common one for this codec.
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `-` characters. No space allowed. No dot allowed.
    Examples: *YUV420*, *sRGB*, *YUV444*, *sRGBA*, *YUV420-REC709*...
- `widthxheight` is the resolution of the file.
- `p|i` tells if the video is interlaced or progressive. If omitted, the video is considered progressive.
- `fps` is the framerate.
    A number, which may contain a dot `.` character, and optionnaly the `fps` suffix.
- `language` is the spoken language. Omitted if there are no dialogs nor any written text.
- `subtitlesst` is the language code for the subtitles followed by the *st* suffix. Omitted if there are no subtitles.
- `ext` is the file extension.
    The extension may include multiple dots, for example: *.tar.gz*

### Examples

`TI_h264_1920x1080_24fps.mp4`

`TI_h264+YUV444-rec709_1920x1080p_24fps_fr_enst.mp4`

`TI_QtAnim+sRGB_1920x1080p_24fps_fr_enst.mp4`