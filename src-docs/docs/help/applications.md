# ![](../img/icons/applications_sl.svg){: style="width:32px;"} Applications

In order to manage the assets and the files they use, *Ramses* keeps track of the *Applications* used in the production.

For each *Application*, *Ramses* keeps also three lists of [file types](filetypes.md):

- **Native types** are the file types used natively by the applications; the ones which type can be created with a *save* command and which store data in a lossless manner.
- **Import types** are all the file types the application can import.
- **Export types** are all the file types the application can create, usually with an *export* or *bake* command for example.

The [*Ramses client*](/components/#ramses-client-and-daemon) can open applications and open files in applications; [*Ramses Add-ons*](/components/#ramses-add-ons) should be able to save and manage the versionning in an application, as long as import and export assets.

## Attributes

The applications are defined by their:

- ***Name***
- ***Short name***

The applications also have these additional attributes (read above):

- ***Native types***
- ***Import types***
- ***Export Types***