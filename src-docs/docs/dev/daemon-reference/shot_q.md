# Shot queries

## getShot

Returns a shot.

### Query attributes

- *getShot*
- *shortName*: **string**. The short name of the shot to retrieve.
- *name*: **string** (optional). The name of the shot to retrieve.

### Reply content

The *Daemon* replies with more information about the shot.

- *shortName*: **string**. The short name of the shot.
- *name*: **string**. The name of the shot.
- *folder*: **string**. The absolute path of the folder containing the shot.
- *sequence*: **string**. The containing sequence name.
- *duration*: **float**. The duration of the shot, in seconds

### Example

Query:  
`getShot&shortName=001*`

Reply:  
```json
{
    "accepted": true,
    "query": "getShot",
    "success": true,
    "message": "Shot list retrived.",
    "content": {
            "shortName": "001",
            "name": "Shot 01",
            "folder": "/path/to/001",
            "duration": 1.25
        }
}
```

## getShots

Returns the list of assets for the current project.

### Query attributes

- *getShots*
- *filter*: **string**. The filter is a way to get a subset of the shots: the daemon will return only shots containing the filter in their name or short name. The filter can contain a wildcard `*`

### Reply content

The *Daemon* replies with the list of Shots.

- *shots*: **array of objects**. The shots. Each shot is an object with:
    - *shortName*: **string**. The short name of the shot.
    - *name*: **string**. The name of the shot.
    - *folder*: **string**. The absolute path of the folder containing the shot.
    - *sequence*: **string**. The containing sequence name.
    - *duration*: **float**. The duration of the shot, in seconds

### Example

Query:  
`getShots&filter=Seq01-*`

Reply:  
```json
{
    "accepted": true,
    "query": "getShots",
    "success": true,
    "message": "Shot list retrived.",
    "content": {
        "shots": [
            {
                "shortName": "Seq01-S1",
                "name": "Shot 01",
                "folder": "/path/to/S1",
                "duration": 1.25
            },
            {
                "shortName": "Seq01-S2",
                "name": "Shot 02",
                "folder": "/path/to/S2",
                "duration": 2.4
            }
        ]
    }
}
```

## getSequences

Returns the list of sequences for the current project.

### Query attributes

- *getSequences*

### Reply content

The *Daemon* replies with the list of sequences.

- *sequences*: **array of objects**. The sequences. Each sequence is an object with:
    - *shortName*: **string**. The short name of the sequence.
    - *name*: **string**. The name of the sequence.

### Example

Query:  
`getSequences`

Reply:  
```json
{
    "accepted": true,
    "query": "getSequences",
    "success": true,
    "message": "Sequence list retrived.",
    "content": {
        "assets": [
            {
                "shortName": "SEQ01",
                "name": "Sequence 01",
            },
            {
                "shortName": "SEQ02",
                "name": "Sequence 02",
            },
            {
                "shortName": "SEQ03",
                "name": "Sequence 03",
            }
        ]
    }
}
```
