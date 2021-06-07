# Asset Queries

## getAsset

Returns an asset.

### Query attributes

- *getAsset*
- *shortName*: **string**. The short name of the asset to retrieve.
- *name*: **string** (optional). The name of the asset to retrieve.

### Reply content

The *Daemon* replies with the details about the asset.

- *shortName*: **string**. The short name of the asset.
- *name*: **string**. The name of the asset.
- *folder*: **string**. The absolute path of the folder containing the asset.
- *tags*: **array of strings**. The tags
- *group*: **string**. The containing group name.

### Example

Query:  
`getAsset&shortName=TRI&name=Tristan`

Reply:  
```json
{
    "accepted": true,
    "query": "getAsset",
    "success": true,
    "message": "Asset retrived.",
    "content": {
            "shortName": "TRI",
            "name": "Tristan",
            "folder": "/path/to/TRI",
            "tags": [
                "tag1",
                "tag2",
                "tag3"
            ],
            "group": "Characters"
        }
}
```

## getAssets

Returns the list of assets for the current project.

### Query attributes

- *getAssets*

### Reply content

The *Daemon* replies with the list of assets.

- *assets*: **array of objects**. The assets. Each asset is an object with:
    - *shortName*: **string**. The short name of the asset.
    - *name*: **string**. The name of the asset.
    - *folder*: **string**. The absolute path of the folder containing the asset.
    - *tags*: **array of strings**. The tags
    - *group*: **string**. The containing group name.

### Example

Query:  
`getAssets`

Reply:  
```json
{
    "accepted": true,
    "query": "getAssets",
    "success": true,
    "message": "Asset list retrived.",
    "content": {
        "assets": [
            {
                "shortName": "A1",
                "name": "Asset 01",
                "folder": "/path/to/A1",
                "tags": [
                    "tag1",
                    "tag2",
                    "tag3"
                ],
                "group": "Asset Group"
            },
            {
                "shortName": "A2",
                "name": "Asset 02",
                "folder": "/path/to/A2",
                "tags": [
                    "tag1",
                    "tag2",
                    "tag3"
                ],
                "group": "Asset Group"
            }
        ]
    }
}
```

## getAssetGroups

Returns the list of asset groups for the current project.

### Query attributes

- *getAssetGroups*

### Reply content

The *Daemon* replies with the list of asset groups.

- *assetGroups*: **array of objects**. The asset groups. Each asset group is an object with:
    - *shortName*: **string**. The short name of the asset.
    - *name*: **string**. The name of the asset.
    - *folder*: **string**. The absolute path of the folder containing the asset.

### Example

Query:  
`getAssetGroups`

Reply:  
```json
{
    "accepted": true,
    "query": "getAgetAssetGroupsssets",
    "success": true,
    "message": "Asset group list retrived.",
    "content": {
        "assets": [
            {
                "shortName": "CHAR",
                "name": "Characters",
                "folder": "/path/to/CHAR"
            },
            {
                "shortName": "PROPS",
                "name": "Props",
                "folder": "/path/to/PROPS"
            },
            {
                "shortName": "SETS",
                "name": "Sets",
                "folder": "/path/to/SETS"
            }
        ]
    }
}
```
