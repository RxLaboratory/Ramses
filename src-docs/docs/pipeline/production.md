![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# ![](../img/icons/check_sl.svg){: style="width:55px;"} Production Tracking

<figure>
  <img src="/img/client/assettracking.png"/>
  <figcaption>The asset table, as shown in the <i>Ramses Client Application</i>.</figcaption>
</figure>

!!! warning
    **Ramses is still under active development**, and this documentation is still being written.

*Ramses* keeps track of the whole production for you, by tracking each **asset** and each **shot** at each **step** of the production. This allows for a precise information about the production of the whole project, and *Ramses* can summarize all this information in nice graphics.

*Ramses* measures the **time spent** on the production of each asset and each shot, and the user can set a **completion ratio** along with setting the **state** of the production for each step. These values are closely related to the estimations used for [**production planning**](planning.md) and *Ramses* is able to measure the **lateness** of the production in real time.

To help users track the production, users can **comment** each task, and each task can be **assigned to a specific user**.

## ![](../img/icons/check_sl.svg){: style="width:48px;"} States

*States* are part of **assets** and **shots** **status**, along with **comments** and **estimations**. They also define a percentage of completion of the work for the corresponding **step**, to be used by default when setting a production status to the specific state.

They can be configured globally to be used everywhere there's a need to show a statuss.

Each state is represented by its **name** and **ID**. The **ID** is used in file and folder names in the [*Ramses Tree*](../components/files/index.md), while the **name** is used in the user interface. They also have a color to be used to display them in the user interface.

By default, *Ramses* includes a few predefined states, and some of them cannot be removed as they are needed by *Ramses* automations. Others are examples and can be customized or removed.

These are the predefined states:

| ID | Name | Completion | Mandatory | Description |
| --- | --- | ---: | :---: | --- |
| NO | Nothing to do | 0% | Yes | There's nothing to do at this step. |
| STB | Stand by | 0% | Yes | Something is missing, let's wait a bit. |
| TODO | Ready to do | 0% | Yes | This step is ready, one can work on this. |
| QST | Question | 20% | No | More information is needed to continue working on this. |
| WIP | Work in progress | 50% | Yes | This is currently in production. |
| CHK | Waiting for approval | 80% | No | This is ready to be reviewed. |
| RDR | Rendering | 90% | No | This is currently rendering, soon we'll be able to watch this beautiful work! |
| CBB | Could be better | 95% | No | This works like that, but if possible it can still be improved. |
| OK | Finished | 100% | Yes | This has been reviewed and validated. |

▹ **[State management in the *Ramses Client Application*](../components/client/states.md)**.

## ![](../img/icons/assets_sl.svg){: style="width:48px;"} Assets

An *Asset* is produced independently from the shots and can be used in several shots. They are the characters, sets, props, and any other re-usable item in the production.

**The list of assets is available and can be managed in the [*Ramses Client Application*](../components/client/index.md), under [the project panel](../components/client/project.md) (for project admin users)**, and by default each asset is represented by a folder in the [*Ramses Tree*](../components/files/index.md) as shown below.

```sh
# (inside the "Projects" folder of the Ramses main folder)
┌ PROJECTID # Root
│
└─┬─ 04-ASSETS 
  ├─┬─ Characters # An asset group
  │ │
  │ ├─┬─ PROJECTID_A_TRISTAN # An Asset
  │ │ ├─── PROJECTID_A_TRISTAN_DESIGN # An asset production step
  │ │ ├─── PROJECTID_A_TRISTAN_MOD
  │ │ ├─── PROJECTID_A_TRISTAN_RIG
  │ │ └─── PROJECTID_A_TRISTAN_TEX
  │ │
  │ └─── PROJECTID_A_ISOLDE # An Asset
  │
  ├─── Props # A Group of Assets
  │
  └─── Sets # A Group of Assets
```

!!! tip
    Without using the [*Ramses Client Application*](../components/client/index.md), you can create assets by creating their folder in the default location in the project subfolder of the [*Ramses Tree*](../components/files/index.md).

Each asset is represented by its **name** and **ID**. The **ID** is used in file and folder names in the [*Ramses Tree*](../components/files/index.md), while the **name** is used in the user interface.

They're grouped in **Asset Groups** to make them easier to sort, and produced through **Asset Production Steps**.

They also have **tags** to be able to search for specific assets.

▹ **[Asset management in the *Ramses Client Application*](../components/client/assets.md)**.

## ![](../img/icons/shots.svg){: style="width:48px;"} Shots

A *Shot* is produced using **assets** and stored in a **Sequence**.

**The list of shots is available and can be managed in the [*Ramses Client Application*](../components/client/index.md), under [the project panel](../components/client/project.md) (for project admin users)**, and by default each shot is represented by a folder in the [*Ramses Tree*](../components/files/index.md) as shown below.

```sh
# (inside the "Projects" folder of the Ramses main folder)
┌ PROJECTID # Root
│
└─┬─ 05-SHOTS
  │
  ├─┬─ PROJECTID_S_01-001 # A Shot
  │ ├─── PROJECTID_S_01-001_ANIM # A shot production step
  │ ├─── PROJECTID_S_01-001_LIGHT
  │ └─── PROJECTID_S_01-001_COMP
  │
  ├─── PROJECTID_S_01-002 # A Shot
  │
  ├─── PROJECTID_S_01-003 # A Shot
  │
  └─── PROJECTID_S_02-001 # A Shot
```

!!! tip
    Without using the [*Ramses Client Application*](../components/client/index.md), you can create shots by creating their folder in the default location in the project subfolder of the [*Ramses Tree*](../components/files/index.md).

Each shot is represented by its **name** and **ID**. The **ID** is used in file and folder names in the [*Ramses Tree*](../components/files/index.md), while the **name** is used in the user interface.

Each shot contains a list of **assets** which are needed for its production.

Shots have a **duration**, usually shown either in seconds or frames, and the addition of all their duration is the duration of the project.

▹ **[Shot management in the *Ramses Client Application*](../components/client/shots.md)**.

## ![](../img/icons/check_sl.svg){: style="width:48px;"} Production status of assets and shots

All **assets** and **shots** have a production status, which is summarized in an asset table and a shot table in the [*Ramses Client Application*](../components/client/index.md).

This status consists of a **state**, a **comment** and a **completion ratio**.

*Ramses* also associates the **time spent** for the production of the item and the **estimation** to the status (read the [*Planning*](planning) section for more information about estimations).

All this information is added and summarized at the **step** level to show the completion for each step, each asset and each shot. *Ramses* also keeps track of the whole production at the **project** level with general statistics.

*Ramses* is also able to extract a **lateness** value from all the available information, available at all levels (shots, assets, steps and whole project).

All the **history** of successive states, comments, completion ratio, etc. is always available.

The combination of an asset or a shot and a step can be **assigned to a specific user**.

For use with [*pipeline* automations](pipeline.md), status are also associated with specific **versions** of the files, and store the **publication** status.

When using [*Ramses Add-ons*](../components/addons/index.md), the state of a specific version is kept in the [file name](../components/files/naming.md) of the corresponding version file, thanks to the [*Ramses naming scheme*](../components/files/naming.md)

▹ **[Production tracking in the *Ramses Client Application*](../components/client/production.md)**.