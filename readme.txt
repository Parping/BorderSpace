All sprite and audio taken from Website itch.io under CC license

spaceship:
https://fearless-design.itch.io/tiny-ships-free-spaceships
player's sprite taken from Website itch.io under CC license

explosion:
https://cuxrie.itch.io/spaceship-shooter-pixel-art-space-pack

audio:
https://nebula-audio.itch.io/free-sf-electronic-music-pixel-perfect
this audio is too big so I use the audio demo chord.wav instead.

https://lmglolo.itch.io/free-8bit-sfx
I only have one request: whatever you do, make your game fun! This content is under CC BY-ND 4.0 https://creativecommons.org/licenses/by-nd/4.0/

Item:
https://grafxkid.itch.io/mini-pixel-pack-3
You can freely use these assets for personal and commercial use. Crediting is optional, but if you'd like to, thank you very much!
I draw this texture follow this style

projectile:
draw by myself


			------------------------------	
	
	Void - Fleet Pack 1 (1.0)

	Commissioned from: Baldur (https://twitter.com/the__baldur)
	Distributed by Foozle (www.foozle.io)

			------------------------------

	License: (Creative Commons Zero, CC0)
	http://creativecommons.org/publicdomain/zero/1.0/

	This content is free to use and modify for all projects, including commercial projects.
	Attribution not required.  If you would like to support, consider a voluntary donation.

			------------------------------

	Donate:   https://foozlecc.itch.io/
	Patreon:  https://www.patreon.com/bePatron?u=48464594

	Follow on YouTube and Twitter for updates:
	https://www.youtube.com/c/FoozleCC
	http://twitter.com/FoozleCC

OS: Windows


Part1:
Add a new function into player_obj.
AddVelocity( velocity )
velocity changed only, addvelocity cannot where the player face.
Player can only change its direction by A and D
Q & E is moved to current_direction+-45 degree.

Part2:
press J to shoot
add cooldown timer in game.
when cooldown, create a new projectile obj at player current position, with a quick speed. the direction is the same as the player.

Part3:
in projectile obj, there's a timer, when create the projectile, the timer is counting time. if finished, the projectile set to die.

Part4:
use same code as A2

Part5:
in projectile obj, raytocircle(), calculate the intersection, to get intersection, we need t. and compare it with projectile current_t and last_t, to see if projectile is pass or reach the enemy_object. 
if so, projectile and enemy get collide, and both of them die.

Part6:
add camera vec3, position is player position, make it into matrix, and add the transformation into the view matrix.
change the background sprite, change the vertex, and change  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT).

Part7.
change scale to vec2, update GetScale(),SetScale(). SetScale() now accept a vec2 to change obj scale.
in Render():
 glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

Part8
add ghost into game_obj
In Render(), send 1 or 0 to sprite_fragment_shader.glsl.
if ghost=0, normal mode as usual.
If ghost=1, add the transformation into the current color, and change the alpha to make it better to see.

if collection is collected by player, make it in ghost mode.