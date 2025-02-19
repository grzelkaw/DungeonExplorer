# DungeonExplorer
<p>Rouge Like game made in C++ using SFML library. It was an final assignment on first year of studies.</p>

# How to start
<p>Download the 'game' folder, inside you have .exe file. Open the .exe file and game is ready to go. In the 'files' folder there are all .cpp and .h source files if you want to check yourself </p>

# Starting screen
<p>On start we can choose our gamemode, single or multiplayer coop. In multiplayer second player uses keys JIKL to move and Numpad to shot. Also have other color than main player to avoid mistake in steering.</p>

![image alt](https://github.com/grzelkaw/DungeonExplorer/blob/main/screenshots/7.png?raw=true)

# Gameplay
<p>
The game revolves around exploring rooms, defeating all enemies, and advancing to the next floor. After each enemy encounter, you gain experience, which allows your hero to level up, increasing stats such as damage, speed, and attack frequency.
Throughout the map, you can find and open chests that may contain gold or stat upgrades. Gold can be spent in special gold rooms to purchase items that provide various effects, such as double-shot, piercing attacks, or healing abilities. The primary objective is to defeat the boss, who typically appears in the final room of each floor, gather his special item, and then repeat the cycle.
</p>
<p>You can also watch gameplay on video here: https://youtu.be/fwEFi-zKaQQ?si=WnwLEPS5Od3dbDAR</p>

![image alt](https://github.com/grzelkaw/DungeonExplorer/blob/main/screenshots/1.png?raw=true)

# Code
<p> Game was programmed in C++ 17 with SFML in version 2.6.2 from 04.2024 to 06.2024. We have here division into classes. Main file combine every other file and run main loop where game is running. 

# Classes
Below description on most of the used classes in project.

<h3> Player </h3>
Player class handle things like steering from keyboard, his statistics, list of picked up items or collision detection.

<h3> Animation </h3>
Handling animation by slicing the sprite in parts, and then going through every frame to represent animation.

<h3> World </h3>
GameTile class is about one 100x100 pixel tile on the world. It provides data like texture, special effects like spikes which deal damage or slime which slow downs player. 
GameWorld class however merges all this single tiles, and put them into the matrix to assemble whole room. In this matricies we input different number where every other number symbols different tile. This class also use DFS algorithm to generate randomly create dungeons using room quota.

![image alt](https://github.com/grzelkaw/DungeonExplorer/blob/main/screenshots/2.png?raw=true)

<h3> Enemy </h3>
Enemy is abstract class who is parent to other various classes like enemyZombie, enemyGhost etc. Enemy have parameters like sprite, stats, collision and child classes have their own movement or attack type.

![image alt](https://github.com/grzelkaw/DungeonExplorer/blob/main/screenshots/4.png?raw=true)

<h3> Object </h3>
Object class is also abstract simmilar to enemy, but this time is about objects that spawn in the rooms. Classes like objectMedKit or objectChest inherit from object chaning only their action on contact with player.

![image alt](https://github.com/grzelkaw/DungeonExplorer/blob/main/screenshots/5.png?raw=true)

<h3> Item </h3>
Abstract class same as object or enemy. Is used to give player different effect after picking item like multi-shot or healing.

![image alt](https://github.com/user-attachments/assets/e98a2257-05a4-4e20-b083-541d7258addb)

# Pause
We can also pause the game, where we can turn off music, or save and load game. Save and Load works by saving our stats and items to txt file, and loading it up when we want.
![image alt](https://github.com/grzelkaw/DungeonExplorer/blob/main/screenshots/8.png?raw=true)

# Summary
<p>That's all for now, I ended this project because of lack of free time. Feel free to download and test the game yourself</p>
<p>Made by Wiktor Grzelka</p>


