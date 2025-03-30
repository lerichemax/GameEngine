WIP



# Overview
In this repository you will find :
 - A [`2D Game Engine`](#gameEngine)
 - An implementation of the classic arcade game QBert
 - A test project aimed at being a sandbox
## Requirements
There is currently no build for this project. Visual studio 2022 is required to open, build and run the solution.

The solution only compiles on 64bits for now.

# Game Engine <a id='gameEngine'></a>
## ECS
Each "object" in a scene is an entity to which components can be added. This components contain data that can then be manipulated by systems.
Entities, components and systems are managed by separated managers and a Registry class exists to oversee them all. Creating new entities and adding components to them require calls to the Registry. 
Entities can be wrapped in a GameObject object to allow simple adding or removing of components or children entities.

## Serializer
The engine supports serialization to a JSON or binary format. This is used for the prefab and the reflection system but will be extended to support a save system.

## Reflection system
The refelection system allows the auto serialization of a class without requiring the user to override a Serialize adn Deserialize function himself.

 ![Screenshot 2025-03-30 145752](https://github.com/user-attachments/assets/268e8463-97be-4532-b466-67b70eedc098)

 By calling the macro SERIALIZE_CLASS(_class_, _parent_) the class and all its member variables declared using the property MACRO will be reflected and serialized.

## Guide 
### Requirements
To start using this engine, one must:
1. create a new project in the same solution.
2. Add the following additional includes directories to the project :
   
    ![Screenshot 2025-01-06 153726](https://github.com/user-attachments/assets/0df4dddb-4b0e-4aac-a200-900e80278a00)
3. Make sure "Use Standard Conforming Preprocessor" is set to "Yes" :

   ![Screenshot 2025-01-06 154127](https://github.com/user-attachments/assets/4a6cc45a-f67b-4d67-8926-7e1f34d3976d)

The project should compile.

Make sure you include PCH.h in all cpp file you create.

### Getting started
- Firstly, create a class deriving from NapoleonEngine, add a constructor and override the pure virtual function.

  ![Screenshot 2025-01-06 154403](https://github.com/user-attachments/assets/026c944f-2ecd-449d-885a-f19457ebfa5b)

- From the constructor, call the parent and pass as arguments, the dimensions of the window, the name of the project and weather or not you want prefabs to be created (more on prefabs below).

  ![Screenshot 2025-01-06 155613](https://github.com/user-attachments/assets/dd371bb4-7421-4e83-bb61-57e204278013)

- In the main function, simply create an object of the previously created class and call Run();

  ![Screenshot 2025-01-06 154601](https://github.com/user-attachments/assets/7ee12803-1173-4c39-a69b-6845f2e8df39)

- Create a class derived from Scene, add a constructor and override the Initialize function.

  ![Screenshot 2025-01-06 155217](https://github.com/user-attachments/assets/e562dfb7-f23a-49c9-8b1a-cb4f4dc52635)

- In the constructor, pass the name you want the scene to have.
- In Intialize, one can add elements to the scene.
- In MainGame::InitGame, you will register your scenes with the SceneManager and set your start scene as active.

  ![Screenshot 2025-01-06 155355](https://github.com/user-attachments/assets/a1923172-800f-4b3d-9568-25efced3010d)

- In MainGame::CreatePrefabs, one can create a prefab by calling on the PrefabManager, then create game objects in it and add components to it. Finally, call Save on the prefab manager and give it a name. Note that the function CreatePrefab will only be called if the fourth argument in the constructor is true. It should be called only when you make a change to a prefab.

  ![Screenshot 2025-03-30 133925](https://github.com/user-attachments/assets/2ae49d8f-c999-4ee2-903b-80ae1e0b0b0b)


- Saved prefabs can then be instantiated by calling the global Instantiate function.

  ![Screenshot 2025-03-30 134300](https://github.com/user-attachments/assets/b8d150e1-2617-4c72-81f7-d107af78e297)

- The rest of the objects necessary to your scene can be added in the Scene::Initialize overriden fucntion.
