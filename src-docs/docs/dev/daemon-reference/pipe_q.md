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
    - *pipeFiles*: **list** of **object**. The pipe files, objects with:
        - *colorSpace*: **string**. The short name of the color space
        - *shortName*: **string**. The short name pipe file.
        - *fileType*: **object**. The file type, an object with:
            - *name*: **string**
            - *shortName*: **string**
            - *extensions*: **list** of **string**

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
                "pipeFiles": [
                    {
                        "colorSpace": "",
                        "shortName": "Preview",
                        "fileType": {
                            "name": "JPEG Image",
                            "shortName": "jpg",
                            "extensions": [".jpg", ".jpeg"]
                            }
                    },
                    {
                        "colorSpace": "",
                        "shortName": "Publish",
                        "fileType": {
                            "name": "Maya Binary",
                            "shortName": "mb",
                            "extensions": [".mb"]
                            }
                    }
                ]
            },
            {
                "inputStepShortName": "STORY",
                "outputStepShortName": "CD",
                "pipeFiles": [
                    {
                        "colorSpace": "",
                        "shortName": "Preview",
                        "fileType": {
                            "name": "JPEG Image",
                            "shortName": "jpg",
                            "extensions": [".jpg", ".jpeg"]
                            }
                    },
                    {
                        "colorSpace": "",
                        "shortName": "Publish",
                        "fileType": {
                            "name": "Adobe PDF",
                            "shortName": "pdf",
                            "extensions": [".pdf"]
                            }
                    }
                ]
            }
        ]
    }
}
```

