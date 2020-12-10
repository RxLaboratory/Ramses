## Ramses

### serverUrl: *string*

The url to the php server


### useSSL: *boolean*

Wether to use SSL to connect to the php server
For addons, true when client app is available and respondonding.


| createProject<br />▹ *RamProject* | *string*: **projectName**<br />*string*: **projectShortName**<br />*string*: **containingFolder**=folderPath | Creates a new instance of a project. |
| createUser<br />▹ *RamUser* | *string*: **userName**<br />*string*: **userShortName**<br />*string*: **containingFolder**=folderPath"/users" | Creates a new instance of a project. |
| createStep<br />▹ *RamStep* | 

| *constructor* | *integer*: **port**=1818<br />*string*: **url**=""<br />*boolean*: **connect**=true | |


## Project

- addStep(RamStep:step)
- addShot(RamShot:shot)
- createShot(string:shotName)
- createShots(int[]:range)
- addAsset(RamAsset:asset)

### createStep( string:stepName, string: stepShortName) ▸ *RamStep*

Creates a new instance of a step.

### createShot( string: shotShortName ) ▸ *RamShot*

Creates a new instance of a shot.