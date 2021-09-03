![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/users_bl.svg){: style="width:55px;"} User management

![](/img/client/user_management.png)

This panel is used to list and manage all users of *Ramses*.

## List

![](/img/client/userlist.png)

The list on the left lists all the users, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each user can be displayed. The ![](/img/icons/explore.svg){: style="width:18px;"} ***folder*** button on the top right corner for each user in the list opens the user folder in your file explorer.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to create a new user. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected users from *Ramses*[^1].

You can use the `[Shift + A]` shortcut key to quickly add new users.

You can use the `[DEL]` or `[Shift + X]` shortcut key to quickly remove selected users.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

!!! note
    The *Ramses Daemon* can't be removed: it is a default user needed by *Ramses* automations, but you can rename it if you wish.

!!! warning
    The default *Administrator* user should be renamed and its password changed as soon as *Ramses* has been installed!

!!! note
    With the default installation of *Ramses*, there's also an additionnal user: *Nicolas "Duduf" Dufresne*, who's the main developper of *Ramses*. This user is just a quick reminder that *Ramses* is free and that **[Duduf](https://patreon.com/duduf) and [RxLab](http://donate.rxlab.info) really need your support**. You can safely keep or remove it. Just. Donate. First. Please! *Ramses* relies **only** on your donations to exist.

## Edition

![](/img/client/useredition.png)

Except for the password, there's no need to validate any change; changes are saved instantly.

- ***Name*** is the user's display name and can be changed any time (although it's not really advisable).
- ***ID*** is the **unique** identifier which is used in the files and folders (see [*Ramses Tree*](../files/index.md)). It can't be changed after the user folder has been created. Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated to the user. It is public.
- The ***Current [role](../../pipeline/administration.md)*** can never be changed by the user itself, and has to be changed by an administrator ([read this](../../pipeline/administration.md) for more information about roles).
- To update your own password, input your ***Current password*** and repeat your ***New password*** twice. Don't forget to click the ***Change password*** button to validate!
- You can change any other user password, just set the ***New password*** twice and ignore the *Current password* field. Don't forget to click the ***Change password*** button to validate!
- You can change the ***folder*** where the user files are stored.
    - Leave the field empty to set back the default folder inside the [*Ramses Tree*](../files/index.md).
    - The red ![](/img/icons/remove_sl.svg){: style="width:18px;"} button moves the user's folder to a `_trash` subfolder.
    - The blue ![](/img/icons/set-folder_sl.svg){: style="width:18px;"} button opens a folder selector to change the folder location.
    - The green ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} button opens the folder in your file explorer.

Passwords are stored encrypted in the [*Ramses Server*](../server/index.md) and there's no way to retrieve them in case a user forgets it. In this case, an administrator has to set a new password for the user.

▹ **[More details about Users and *Ramses*](../../pipeline/administration.md)**.

!!! note
    You cannot change your own role.

!!! tip
    To change a user *ID*, you need to remove its folder.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.