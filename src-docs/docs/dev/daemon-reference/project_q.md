# Project queries

## getCurrentProject

Returns the current project.

### Query attributes

- *getCurrentProject*

### Reply content

The *Daemon* replies with the project.

- *shortName*: **string**. The short name of the project.
- *name*: **string**. The name of the project.
- *width*: **int**. The width of the project in pixels.
- *height*: **int**. The width of the project in pixels.
- *framerate*: **float**. The framerate of the project.
- *folder*: **string**. The absolute path of the folder containing the project.

### Example

Query:  
`getCurrentProject`

Reply:  
```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Current project is: Project 01.",
    "content": {
        "shortName": "P1",
        "name": "Project 01",
        "width": 2048,
        "height": 858,
        "framerate": 24.0,
        "folder": "/path/to/P1"
    }
}
```

## getProject

Gets a specific project

### Query attributes

- *getProject*
- *shortName*: **string**. The short name of the project.
- *name*: **string** (optional). The name of the project.

### Reply content

The *Daemon* replies with more information about the project.

- *shortName*: **string**. The short name of the project.
- *name*: **string**. The name of the project.
- *width*: **int**. The width of the project in pixels.
- *height*: **int**. The width of the project in pixels.
- *framerate*: **float**. The framerate of the project.
- *folder*: **string**. The absolute path of the folder containing the project.

### Example

Query:  
`getProject&name=The Project&shortName=PROJ`

Reply:  
```json
{
    "accepted": true,
    "query": "getProject",
    "success": true,
    "message": "Project information retrived.",
    "content": {
            "shortName": "PROJ",
            "name": "The Project",
            "width": 1998,
            "height": 1080,
            "framerate": 60.0,
            "folder": "/path/to/PROJ"
        }
}
```

## getProjects

Returns the list of available projects.

### Query attributes

- *getProjects*

### Reply content

The *Daemon* replies with the list of projects.

- *projects*: **array of objects**. The projects. Each project is an object with:
    - *shortName*: **string**. The short name of the project.
    - *name*: **string**. The name of the project.
    - *width*: **int**. The width of the project in pixels.
    - *height*: **int**. The width of the project in pixels.
    - *framerate*: **float**. The framerate of the project.
    - *folder*: **string**. The absolute path of the folder containing the project.

### Example

Query:  
`getProjects`

Reply:  
```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Project list retrived.",
    "content": {
        "projects": [
            {
                "shortName": "P1",
                "name": "Project 01",
                "width": 2048,
                "height": 858,
                "framerate": 24.0,
                "folder": "/path/to/P1"
            },
            {
                "shortName": "P2",
                "name": "Project 02",
                "width": 1998,
                "height": 1080,
                "framerate": 60.0,
                "folder": "/path/to/P2"
            }
        ]
    }
}
```

## setCurrentProject

Sets the current project

### Query attributes

- *setCurrentProject*
- *shortName*: **string**. The short name of the project.
- *name*: **string** (optional). The name of the project.

### Reply content

The *Daemon* replies with more information about the project.

- *shortName*: **string**. The short name of the project.
- *name*: **string**. The name of the project.
- *width*: **int**. The width of the project in pixels.
- *height*: **int**. The width of the project in pixels.
- *framerate*: **float**. The framerate of the project.
- *folder*: **string**. The absolute path of the folder containing the project.

### Example

Query:  
`setCurrentProject&name=The Project&shortName=PROJ`

Reply:  
```json
{
    "accepted": true,
    "query": "setCurrentProject",
    "success": true,
    "message": "Current project set to: The Project.",
    "content": {
            "shortName": "PROJ",
            "name": "The Project",
            "width": 1998,
            "height": 1080,
            "framerate": 60.0,
            "folder": "/path/to/PROJ"
        }
}
```