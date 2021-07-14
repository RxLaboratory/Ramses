![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# ![](../img/icons/gotosettings.svg){: style="width:55px;"} Ramses Administration (Projects and Users)

<figure>
  <img src="/img/client/admin.png"/>
  <figcaption>The administration panel, as shown in the <i>Ramses Client Application</i>.</figcaption>
</figure>

!!! warning
    **Ramses is still under active development**, and this documentation is still being written.

    We strongly invite you to read the [changelog](../changelog.md) to know what's going on, and especially to have a look at the [known issues](../changelog.md). 

In order to use *Ramses*, at least one **user** and one **project** are needed; when using the [*Ramses Client Application*](../components/client/index.md), you can also manage other elements.

Both **users** and **projects** **are represented by folders** in the [*Ramses Tree*](../components/files/index.md), and *Ramses* components will list them from the default folders if they can't get the information from the [*Ramses Client Application*](../components/client/index.md) or [*Ramses Server*](../components/server/index.md).

## ![](../img/icons/users_bl.svg){: style="width:48px;"} Users

**The list of users is available and can be managed in the [*Ramses Client Application*](../components/client/index.md), under [the administration panel](../components/client/admin.md) (for administrator users)**, and by default each user is represented by a folder in the [*Ramses Tree*](../components/files/index.md) as shown below.

```sh
┌ Ramses Folder
│
└─┬─ Users # User files
  │ # The name of the user folders are their individual ID
  │
  ├─┬─ Duduf # Files for a single user (ID)
  │ └─── Config # Configuration files for this user
  │
  ├─┬─ John # ID
  │ └─── Config
  │
  └─── ...
```

!!! tip
    Without using the [*Ramses Client Application*](../components/client/index.md), you can create users by creating their folder in the default location in the [*Ramses Tree*](../components/files/index.md).

Each user is represented by its **name** and **ID**, and has a **role**.  
The **ID** is used in file and folder names in the [*Ramses Tree*](../components/files/index.md), while the **name** is used in the user interface.  
The **role** defines the rights the user has to manage *Ramses*, the projects, the assets, etc. There are four roles listed below.

▹ **[User management in the *Ramses Client Application*](../components/client/users.md)**.

### ![](../img/icons/admininstrator_sl.svg){: style="width:26px;"} Administrator

The *Administrator* has all rights to configure and modify *Ramses*, especially but non exclusively:

- Creating, removing projects.
- Creating, modifying, removing template steps.
- Creating, modifying, removing users.

The *Administrator* has all other rights from other roles too.

!!! Note
    Althought the *administrator* has the right to modify all users, he cannot remove himself nor change his own role. Another administrator has to do it.

### ![](../img/icons/project-leader-head-admin_sl.svg){: style="width:26px;"} Project Admin

The *Project Administrator* has the necessary rights to configure and modify projects, especially but non exclusively:

- Modifying projects.
- Creating, modifying, removing steps.
- Assigning Users to projects.
- Creating, modifying, removing Asset Groups.
- Creating, modifying, removing Sequences.

### ![](../img/icons/leader-head-checker_sl.svg){: style="width:26px;"} Lead

The lead is a user who can manage projects, and has less rights than the *administrator*:

- Creating, modifying, removing shots.
- Creating, modifying, removing assets.
- Assigning users to assets and shots.

### ![](../img/icons/user_sl.svg){: style="width:26px;"} Standard User (User)

The standard user cannot modify nor remove anything from *Ramses*, but interacts with shots and assets.

## ![](../img/icons/cinema-movie-settings_sl.svg){: style="width:48px;"} Projects

__*Projects* represent the final artwork__: a feature film or one of its sequence, an episode in a TV series, a short movie, a music video...

They are made of ***Shots*** which are in turn made of ***Assets***.

*Assets* and *Shots* are produced through different ***Steps*** which are inter-dependent.

*Shots* are grouped in ***Sequences***, and a project contains at least one sequence.

*Assets* are grouped in ***Asset Groups*** (e.g. characters, sets, props...).

!!! note
    It is not mandatory to use sequences, and for short projects (less than a hundred shots), it is often easier to just have a single sequence.

**The list of projects is available and can be managed in the [*Ramses Client Application*](../components/client/projects.md), under [the administration panel](../components/client/admin.md) (for administrator users)**, and by default each project is represented by a folder in the [*Ramses Tree*](../components/files/index.md) as shown below.

```sh
┌ Ramses Folder
│ 
└─┬─ Projects # Project files
  │ # The name of the project folders are their individual ID
  │
  ├─── TI # A project (ID)
  │
  ├─── PPB # Another project (ID)
  │
  └─── ...
```

!!! tip
    Without using the [*Ramses Client Application*](../components/client/index.md), you can create projects by creating their folder in the default location in the [*Ramses Tree*](../components/files/index.md).

Each project is represented by its **name** and **ID**. The **ID** is used in file and folder names in the [*Ramses Tree*](../components/files/index.md), while the **name** is used in the user interface.

The projects also have the characteristics of the final video (**width** and **height** in pixels, **framerate**, **duration**...). They also have a **deadline** defining the date when the project has to be finished and delivered.

**Users** can be assigned to specific projects.

▹ **[Project management in the *Ramses Client Application*](../components/client/projects.md)**.
