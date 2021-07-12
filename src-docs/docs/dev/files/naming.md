# Regular Expressions for files and folder naming.

Ramses uses [specific file and folder naming conventions](../../components/files/naming.md). Here are useful *regular expressions* to manipulate them.

!!! note
    All names used by Ramses should be compated ignoring the case. Thus all regular expressions should use the `i` flag.

[TOC]

## Short Names (IDs)

    /^(?!(?:PUB|WIP|V)?[0-9]+)[a-zA-Z0-9+-]{1,10}$/gi

*Short names*, or *ID*s are made exclusively of `a-z`, `A-Z, `0-1` and `+-` characters, and cannot be more than 10 characters.

The negative part of the regular expression, `(?!(?:PUB|WIP|V)?[0-9]+)`, is used to differenciate regular blocks (name, short name, etc.) from versions.

## Names

    /^(?!(?:PUB|WIP|V)?[0-9]+)[ a-zA-Z0-9+-]{1,256}$/gi

*Names* made exclusively of `a-z`, `A-Z`, `0-1`, `+-` and the space characters, and cannot be more than 256 characters.

!!! warning
    *Names* should never be used in file names, as it may generate paths which may be very long.

The negative part of the regular expression, `(?!(?:PUB|WIP|V)?[0-9]+)`, is used to differenciate regular blocks (name, short name, etc.) from versions.


## Status and versions

    /(PUB|WIP|V)?([0-9]+)/gi

The first matching group contains the status (and may be empty), and the second group is the version number.

!!! note
    The string in the first matching group should be dynamically built using the list of available status *ID*s (*short names*). By default, it should at least contain the three strings shown in this example: *WIP*, *PUB*, and *V*.

## Extract version from a file name

    /(?:(_(v|wip|pub)?([0-9]+)))(?:\.[a-z0-9.]+)?$/gi

This regular expressions matches everything including the last underscore, the version block and the file extension if any.

The first group contains the underscore and the version block; it can be used to be replaced by an empty string in file names to quickly remove the version block from the name.

The second and third groups contain the state and the version.

## Resource names

    /(?!(?:PUB|WIP|V)?[0-9]+)[a-z0-9+\s-]+/gi

Resource names can be made of `a-z`, `0-9`, `+-` and white spaces but must not start with a version scheme.

## File names

This is the regular expression for validating, matching and decomposing file names.

    /^([a-z0-9+-]{1,10})_(?:([ASG])_((?!(?:PUB|WIP|V)?[0-9]+)[a-z0-9+-]{1,10}))(?:_((?!(?:PUB|WIP|V)?[0-9]+)[a-z0-9+-]{1,10}))?(?:_((?!(?:PUB|WIP|V)?[0-9]+)[a-z0-9+\s-]+))?(?:_(PUB|WIP|V)?([0-9]+))?(?:\.([a-z0-9.]+))?$/gi


Here is the list of matching groups:

1. Project ID
2. Type: *A* (Asset) or *S* (Shot) or *empty* (for general items)
3. Asset or Shot ID or *empty* (for general items)
4. Type: *G* (General) or *empty*
5. Step ID
6. Resource name or empty
7. Status or empty
8. Version number or empty
9. Extension or empty

If any string does not match this regular expression, it means it does not follow the Ramses naming scheme.

## Master media file names

    /^([a-z0-9+-]{1,10})_([a-z0-9]{1,10})(?:-([a-z0-9]{1,10})?)_(\d+)x(\d+)(i|p)?_(\d+\.?\d*)(?:fps)?(?:_([a-z]+))?(?:_([a-z]+)st)\.([a-z0-9.]+)/gi

Here is the list of matching groups:

1. Project ID
2. Codec ID
3. Colorspace ID
4. Width
5. Height
6. *p* (progressive), *i* (interlaced), or empty
7. Framerate
8. Language or empty
9. Subtitles language or empty
10. Extension