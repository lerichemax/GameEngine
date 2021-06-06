Engin Specifics and design:

PREFABS:

The prefabs manager allows the user to save a gameobject and all its children and components as prefab.
It can then instantiate it from anywhere in the program.
The prefabs manager stores pointers to the prefabs. 
When instantiate is called, a pointer to a cloned prefab object is returned.
The clone function is a virtual constructor.

OBSERVERS:

In order to allow several object to be observed by the same observer without creating dangling pointers
	when their subject's destructor is called, the observers are added to an observer manager
	which will handle their deletion

LAYERED RENDERING:

Renderer components are assigned a layer that will determine their order of rendering.
Scene has a pointer to a SceneRenderer class;
SceneRenderer as an array of *nbr of layers* vectors that hold pointers to each RendererComponent
	in the scene.
The group are then renderered one after the other.

SOUND:

The Sound system is accessed through a Service locator.
An event queue is used to play the sounds in order.
Sound systems derived from an interface.

INPUT:

The input manager can register inputs from keyboard, mouse and as much as four controllers.
The user can add a command to each input and a key condition to trigger it (pressed, released or held)
The user can also check each key or action to see if it is pressed/triggered or not.

TIMER

Instead of passing DeltaTime as argument for every update function, this value is accessible globally 
	through a singleton.
The timer class holds it and calculate it before the update is called.
The maximum deltatime is 0.02f. 
This maximum value is meant to prevent delta time to rise indefinitely when the use stops the execution
	to debug.


REPOSITORY:
https://github.com/LordPoneymor/GameEngine