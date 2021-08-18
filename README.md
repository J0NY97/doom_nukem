
Graphic:

Y	|	You can look up and down.
Y	|	The areas you go through don’t have a fixed shape, for example the rooms
			can have any number of walls in all possible directions.
Y	|	The floor and ceiling have adjustable heights, which means different
			areas will not be at the same height - and you’ll
				have to manage that smoothly.
Y	|	The floor and ceiling may not be horizontal but inclined planes.
Y	|	The floors and ceilings have textures.
Y	|	There can be a sky instead of a ceiling.
Y	|	Walls can be partially transparent, unveiling the room behind.
Y	|	Decorative elements can be placed on the walls over the main texture
			(paintings, posters... ).
Y	|	Objects or characters can be placed in the world
			as sprites that always face the player.
Y	|	Objects or characters can be placed in the world as multiple sprites
			displayed according to the orientation between the object and the
				player (you’ll see the object or character from behind
					and from the side as you walk around it).
N	|	Each room or area has a light control tool,
			affecting both the walls and the objects within.
Y	|	Text messages can be displayed during the game over the rest of the graphics.
Y	|	There must be a HUD (life, currency, ammunition, carried artifacts...)

Gameplay:

Y	|	The view follows the mouse smoothly,
			making it possible to spin around (a whole 360) and to look up and down.
Y	|	Strafing is possible.
Y	|	Interactions with walls and steps of a reasonable height should be managed properly.
Y	|	The player can run, jump, fall, bend down and get up.
Y	|	The player can fly or swim (same difference...).
Y	|	The objects present may or may not block the passage,
			in proportion to their visual representation.
Y	|	The items around can be picked up - or not, and supply an inventory.
Y	|	The player can interact with the elements of the rooms (walls, objects...),
			whether by simple proximity (walking on a dangerous area)
				or because of a voluntary action (pressing a button).
Y	|	Actions or series of actions can result from interactions, from objects
			being pickedup, or even in a totally independent way. They can possibly be timed.
Y	|	Actions can alter all the elements of the game, all the shapes,
			all the properties (the shape of a room, the height of a ceiling,
				the texture of a wall or an object, the location of the specific
					texture on a door that indicates its "handle", etc).
Y	|	The 2 previous points obviously mean that your game must contain animations,
			various devices like door opening systems,
				keys and ways to use them, elevators and secret passages.
Y	|	Specific characters and / or objects can have their own actions,
			reactions and interactions in the universe.
Y	|	To mimic the Doom and Duke Nukem universes,
			projectiles can be fired and interact with the background,
				the objects, the characters, even the player itself.
N	|	There is a story with a mission, a goal to reach,
			a target (save the earth, get rich,... even something very simple, check out Doom).
Y/N |	There is a beginning and an end for each level.
Y 	|	There are sound effects.
Y 	|	There’s music.

Other considerations:

Y	|	A level editor is mandatory. It can be integrated with the main executable and
		activated by a command line option, or it can be a separate binary. It can be used
		to completely define a level: the areas and the differences in height between them,
		the textures, the actions and interactions, etc.

Y/N	|	Just like in the original games, you must pack in a single file all the elements you’re
		going to need for the game: the level’s design, textures and gameplay elements.
		One file per level is accepted, but it must be self-sufficient. The doom-nukem binary
		and the chosen level file must be self-sufficient.
