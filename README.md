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
The engine is based on an Entity-Component-System.

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

  ![Screenshot 2025-01-06 160353](https://github.com/user-attachments/assets/c3501830-268e-470e-83dc-88f76f8a7ea6)

