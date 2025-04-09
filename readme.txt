Jill Li
Isen McDonald



OS: Windows

Press P to open SHOP, press button right up conner to close shop.
Press M to open MiniMap, 
Press R_Shift to collect planets,
Press L_Shift to accelerate, 
Press L to laser, 
Press J to shoot, 
Press K to use bomb,
Press Enter to pause,
WASD uses for move,
up and down use to zoom in/out minimap.

How to play:
Collect, destroy enemy, collect the drops, unlock weapon, level up, wait for level2, pass the maze (use bomb to destroy or just solve it!), kill the boss to win!!!

Player is the spaceship.
Resources: Energy, Iron, Invisible point, coin.
Player can kill enemy to get exp, to level up, level up can increase max hp and max energy and max exp request.
When level up, player can full hp again.
With Iron and Coin, player can open shop menu to unlock laser, shield, bomb.
Use laser, shield, accelerate request energy. Shield only cost energy when player collide with other things.
Laser and shield only need to unlock once, but bomb can only buy one, it's finite.

The planets player can collect are not planets, they are monsters' eggs. When game playing a while, player can hear the whisper when player get close to the planet. The planet that has been collected cannot turn to monster.
After 65 seconds (it can be changed), monsters will dash out, player cannot do anything. Then player throw to level 2, a maze. The boss is outside of the maze.
Game music stops, changes to a horror background music.
Player need to finish the maze to get out. When player kill the boss, player win the game, a particle firework appears.
Otherwise a "Gameover" exists.
When player hit the boss, the game music comes back a little, until boss dies, the music is 100%.
(The maze can be destroyed by bomb.)

Enemy:
1. minion: stupid intercepting, can shoot.
2. ex-minion: better intercepting, can shoot.
3. blue_ship: run away from the player, will go back to fortress.
4. fortress: can Heal player when player is close enough and the robot arm attach the player , but it can be angry, then it will shoot player.
5. monster: planet can be collected by player, after wakeup, it will dash to the player to attack. Planet after collected cannot turn to a monster.
6. boss: like monster but much bigger.

Every type, if it can shoot, it has its own projectile, they have different color, and different lethality. 
They gives different exp, different collections, and different ability to drop collections.

Weapon:
1.normal projectile
2.laser
3.bomb
normal projectile use ray-circle collision, laser use rectangle-circle collision, bomb use circle-circle collision.
projectile is fastest, bomb is slowest, so bomb use circle-circle.
Ships and planets have different coefficient damage with different type of weapon.

Collectible items:
1.energy
2.invisible point
3.coin
4.iron
Invisible point when collect 5, player can be invisible 5s.
Coin and iron to unlock laser, shield in shop, buy bomb in shop.
Laser, shield, accelerate actions cost energy, when energy=0, cannot use them.
After 5s use none of them, energy will recharge.
Die enemy drops items, different type enemy drop different type items.
Can do collect action when close to the planet, need 2s, this action gain exp.

Collision detection
Circle-Circle,Rect-Circle,Ray-Circle.
Player cannot pass bricks, it will rebound.

Game world
Bigger game world than the screen.
When player reach the boundary(though it is big)，player will be set to the another boundary of the world. Make's it like Seamless map.

Particles:
Player press qwe, gives engine particle, press left-shift to accelerate, the particle turn into blue.
When player wins, a firework comes out.

Hierarchical transformation：
fortress shooter,fortress robot arm.
arm has three parts.

UI:
HP, Energy, exp, bomb, collections, collect action progress bar, level, minimap, shop.
Shop gives text to describe the things player can unlock or buy.
The Screen has to much things so I delete the score UI.

Advanced method:
robot arm can find player's position and move itself to the player.
blue ship run away from player when player get close to them.
maze wall collide with the player.
animate shader, text shader, number shader.

Extra:
Minimap, level2...





All sprite and audio taken from Website itch.io under CC license

spaceship:
https://fearless-design.itch.io/tiny-ships-free-spaceships
player's sprite taken from Website itch.io under CC license

http://creativecommons.org/publicdomain/zero/1.0/

explosion:
https://cuxrie.itch.io/spaceship-shooter-pixel-art-space-pack

audio:
https://nebula-audio.itch.io/free-sf-electronic-music-pixel-perfect
and the .txt file in the audio file.


https://lmglolo.itch.io/free-8bit-sfx
I only have one request: whatever you do, make your game fun! This content is under CC BY-ND 4.0 https://creativecommons.org/licenses/by-nd/4.0/

Item:
https://grafxkid.itch.io/mini-pixel-pack-3
You can freely use these assets for personal and commercial use. Crediting is optional, but if you'd like to, thank you very much!
I draw this texture follow this style

projectile and other things:
draw by Jill Li