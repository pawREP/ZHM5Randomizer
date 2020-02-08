## ZHM5Randomizer
--------------------
ZHM5Randomizer is a smart item randomizer for hitman 2 (2018) that allows randomization of world, hero, npc and stash inventories. It's easy to use, customize and extend.

#### How to install / use:
- Download the latest release here: [Download](https://github.com/pawREP/ZHM5Randomizer/releases/latest "Download")
- Extract all the files into `..\HITMAN2\Retail` (yes, even if you are using DX12).
- Optionally, configure the randomizer by modifying `ZHM5Randomizer.ini`.
- Start the game as usual, start a new mission and enjoy.

#### Limitations / Bugs
- Loading save files is not supported.
- Restarting missions from the pause menu is not supported. You have to visit the main menu and restart the mission from there.
- Re-planning missions from the pause menu is not supported, unless the difficulty is changed.
- While all missions are completable, some mission objectives might not be. For example, not all clues are fully functional in whittleton creek.

#### How to extend the randomizer with custom randomization logic
The randomizer was build with easy extendablity in mind. Randomizers have to implement the `RandomizationStrategy` interface and they have to be added as part of a randomizer collection in `RandomisationMan::configureRandomizerCollection()`. The central functionality of any randomizer is implemented in `RepositoryID* RandomizationStrategy::randomize(const RepositoryID*)`. This function gets called by a hook into the game code whenever the game tries to spawn an item. The input id, represents the item the game attempted to spawn, the returned id is the, potentially randomized, id the game will spawn instead. If randomizers implement `RandomisationStrategy::initialize`, they will be notified when a new game scenario is loaded and they will be provided with the default item pool for this scenario. This can be very helpful when writing stateful randomizers where knowledge of the complete item pool is required before `randomize` is called for the first time. `WorldInventoryRandomisation` is an example where this is the case.
