# ![](../img/icons/users_bl.svg){: style="width:32px;"} Users

Each person working with *Ramses* is a user.

There are three different kind of users, which are called ***roles*** in *Ramses*: 

- ***Administrator***
- ***Lead***
- ***Standard User***

Each role defines different rights to interact with the production pipeline. Read below to learn mode.

## Roles

### ![](../img/icons/admininstrator_sl.svg){: style="width:26px;"} Administrator (Admin)

The *Administrator* has all rights to configure and modify *Ramses*, especially but non exclusively:

- Creating, removing projects.
- Creating, modifying, removing template steps.
- Creating, modifying, removing users.

The *Administrator* had all other rights from other roles too.

!!! Note
    Althought the *administrator* has the right to modify all users, he cannot remove himself nor change his own role. Another administrator has to do it.

### Project Administrator (Proj Admin)

The *Project Administrator* has the necessary rights to configure and modify projects, especially but non exclusively:

- Modifying projects.
- Creating, modifying, removing steps.
- Assigning Users to steps.
- Creating, modifying, removing Asset Groups.
- Creating, modifying, removing Sequences.

### ![](../img/icons/leader-head-checker_sl.svg){: style="width:26px;"} Leader (Lead)

The leader is a user who can manage projects, and has less rights than the *administrator*:

- Creating, modifying, removing shots.
- Creating, modifying, removing assets.
- Assigning users to assets and shots.

### ![](../img/icons/user_sl.svg){: style="width:26px;"} Standard User (User)

The standard user cannot modify nor remove anything from *Ramses*, but interacts with shots and assets.

## Other attributes

As any other object in *Ramses*, the users are defined by their:

- ***Name***
- ***Short Name***
- ***User Folder*** which is by default in the *users* subfolder of the main *Ramses* folder. It contains the user's specific files, documents, tests... Anything the user wants to keep for herself or himself.

The user also has these additional attributes:

- ***Role***, see above.
- ***Password***, used to log in.

