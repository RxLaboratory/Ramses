# Pipe queries

## getPipes

Returns the list of pipes for the current project.

### Query attributes

- *getPipes*

### Reply content

The *Daemon* replies with the list of Pipes.

- *pipes*: **array of objects**. The pipes. Each pipe is an object with:
    - *inputStepShortName*: **string**. The short name of the input step.
    - *outputStepShortName*: **string**. The short name of the output step.
    - *fileType*: **object**. The file type, an object with:
        - *name*: **string**. The name of the file type
        - *shortName*: **string**. The short name of the file type.
        - *extensions*: *array of string*. The extensions, including the leading `.`.

### Example

Query:  
`getPipes`

Reply:  
```json
{
    "accepted": true,
    "query": "getPipes",
    "success": true,
    "message": "Pipeline retrieved.",
    "content": {
        "pipes": [
            {
                "inputStepShortName": "TEX",
                "outputStepShortName": "MOD",
                "fileType": {
                    "name": "Alembic geometry data",
                    "shortName": "abc",
                    "extensions": [".abc"]
                    }
            },
            {
                "inputStepShortName": "STORY",
                "outputStepShortName": "CD",
                "fileType": {
                    "name": "JPEG Image",
                    "shortName": "jpg",
                    "extensions": [".jpg", ".jpeg"]
                    }
            }
        ]
    }
}
```

