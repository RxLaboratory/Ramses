# File and folder naming scheme

*Ramses* uses a strict and specific naming convention. This naming scheme is based on the *short names* of the elements, which are defined by the administrator users.

We provide [a set of regular expressions](../dev/files/naming.md) to help developpers manipulate this naming scheme.

[TOC]

## Short names (ID)

In *Ramses*, all elements (users, assets, shots, projects...) have two names: the actual name used in the user interface and displayed anywhere there is enough room, and a *short name* which is no longer than 10 characters (but it may be the same as the actual name). *Short names* are unique (while names may not be unique, although it's good practice to keep them unique too) and they are used as identifiers.

While names can contain a lot of different characters, *short names* can consist only of alphanumeric characters (without any special character or accent), and the plus `+` and minus `-` characters.

For the sake of readability, we are going to call these *short names*: _**ID**s_ in the rest of this document.

## Naming scheme

This is the complete naming scheme, for all files and folders:

`projectID_A|S|G(_assetID|shotID)_stepID(_resource name)(_version)(.extension)`

Explanations:

- The name is divided into blocks, separated by the underscore `_` character.
- Optionnal blocks are shown in parenthesis `()`
- The vertical bar `|` characters means "or" and represents a choice in a list of possibilities.

### Blocks

- `projectID` is the ID of the project.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `+-` characters. No space allowed.
- `A|S|G` tells if the file or folder refers to an Asset (*A*), a shot (*S*) or a general item (*G*).
- `assetID` is the ID of the asset, if the previous block is *A*.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `+-` characters. No space allowed.
- `shotID` is the ID of the shot, if the previous block is *S*.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `+-` characters. No space allowed.
- `stepID` is the ID of the production step.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `+-` characters. No space allowed.
- `resource name` describes the specific resource this file or folder refers to.  
    Between 1 and 20 characters consisting only of `a-z`, `0-9` and `+-` characters and may contain spaces.
    Resource names cannot start with a status ID followed by a number (e.g. *v01* or *wip2*) nor a number alone.
- `version` contains the status and version number.  
    May start with either `v` or the ID of the status (e.g. "wip", "ok", etc.), and includes one or more numbers. No other character allowed.
- `extension` is the file extension.  
    The extension may include multiple dots, for example: *.tar.gz*

### Examples

## Master media files and final exports

Final masters have a specific naming scheme.

`projectID_codec(-colorspace)_widthxheight(p|i)_fps(_language)_(subtitlesst).ext`

### Blocks

- `projectID` is the ID of the project.  
    Between 1 and 10 characters consisting only of `a-z`, `0-9` and `+-` characters. No space allowed.
- `codec` is the name of the codec.
    Between 1 and 10 characters consisting only of `a-z` and `0-9` characters. No space allowed. No dot allowed (e.g. *h.264* must be named *h264*).
- `colorspace` represents the colorspace used if it's not the most common one for this codec.
    Between 1 and 10 characters consisting only of `a-z` and `0-9` characters. No space allowed. No dot allowed.
    Examples: *YUV420*, *sRGB*, *YUV444*, *sRGBA*...
- `widthxheight` is the resolution of the file.
- `p|i` tells if the video is interlaced or progressive. If omitted, the video is considered progressive.
- `fps` is the framerate.
    A number, which may contain a dot `.` character, and optionnaly the `fps` suffix.
- `language` is the spoken language. Omitted if there are no dialogs nor any written text.
- `subtitlesst` is the language code for the subtitles followed by the *st* suffix. Omitted if there are no subtitles.
- `ext` is the file extension.
    The extension may include multiple dots, for example: *.tar.gz*

### Examples