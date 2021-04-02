# Pipeline

*Ramses* is able to handle your whole *pipeline* from the pre-production to the post-production.

The pipeline describes the way the production of the assets and the shots goes through each step. It actually describes each connection, each *pipe* between each step, keeping important information like the file type used and its color space for example.

## Pipes

Each *pipe* is a connection between two steps.

It keeps the file type used to export from and import to these steps, as long as the color space of the file. *Ramses Addons* can use this information to automate the process.

*Pipes* also handle *triggers* which run scripts in your applications when a *state* changes in the corresponding steps (when a file runs through the *pipe*).

*Pipes* also automate the production management and are able to automatically update statuses, notify users of changes and of the disponibility of new materials for example.

The *pipes* are managed within the *Ramses client* and its *Pipeline editor*.