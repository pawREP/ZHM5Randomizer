## ZHM5Randomizer
--------------------
ZHM5Randomizer is a smart item randomizer for Hitman 3 (2021) that allows randomization of world, hero, npc and stash inventories. It's easy to use, customize and extend.

All main missions from Hitman 1, 2 and 3 as well as all side missions are supported. Escalations are currently not randomized. 

#### How to install / use:
- Download the latest release here: [Download](https://github.com/pawREP/ZHM5Randomizer/releases/latest "Download")
- Extract all the files into `..\Program Files\Epic Games\HITMAN3\Retail`.
- Optionally, configure the randomizer by modifying `ZHM5Randomizer.ini`.
- Launch the game, start a new mission and enjoy.

#### Limitations / Bugs / Troubleshooting 
- Loading save files is not supported.
- Restarting missions from the pause menu is not supported. You have to visit the main menu and restart the mission from there.
- Re-planning missions from the pause menu is not supported, unless the difficulty is changed.
- While all missions are completable, some mission objectives might not be. For example, not all clues are fully functional in whittleton creek.
- If you are experiencing `Application Error (0xC0000142)`, you are probably starting the game directly by double clicking the exe. Launch the game through steam/epic to fix this issue.
- If you get an error about missing `vcruntime140.dll`, you have to download and install the latest [msvc redistributable](https://aka.ms/vs/16/release/vc_redist.x64.exe).

#### How to extend the randomizer with custom randomization logic
The randomizer was build with easy extendablity in mind. Randomizers have to implement the `RandomizationStrategy` interface and they have to be added as part of a randomizer collection in `RandomisationMan::configureRandomizerCollection()`. The central functionality of any randomizer is implemented in `RepositoryID* RandomizationStrategy::randomize(const RepositoryID*)`. This function gets called by a hook into the game code whenever the game tries to spawn an item. The input id, represents the item the game attempted to spawn, the returned id is the, potentially randomized, id the game will spawn instead. If randomizers implement `RandomisationStrategy::initialize`, they will be notified when a new game scenario is loaded and they will be provided with the default item pool for this scenario. This can be very helpful when writing stateful randomizers where knowledge of the complete item pool is required before `randomize` is called for the first time. `WorldInventoryRandomisation` is an example where this is the case.
