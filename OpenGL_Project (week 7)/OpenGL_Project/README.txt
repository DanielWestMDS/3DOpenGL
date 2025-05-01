C++ version 17 or newer needed

Controls:
WASD to move
Right click to look around in editor
OBJ file selector contains meshes you can select
Instantiate an object with the selected mesh by pressing the "Add Object" button
The Play Button will leave the editor mode and begin running the game
Return to the editor mode by pressing 'esc'
Wireframe and Visible collision are toggleable via the buttons window

Left click to select an object in the scene
Selected Object tab contains editable information for the selected object
Setting "Win Condition?" checkbox to true will cause the game to be won if the player overlaps the object's hitbox
All settings are saved to a json file 
Scenes are saved between play sessions as long as the "Save Scene" button is pressed

don't mind the height map

REUSED CONTENT: Mostly reused from Advanced Graphics for rendering but I did make CLineRenderer class to draw collisions in debug mode.
File headers that are all // were probably reused but some have been edited. Classes with /**** file headers were created for this assessment

referenced this to make my debug line renderer class
https://stackoverflow.com/questions/14486291/how-to-draw-line-in-opengl

used this a lot
https://www.reactphysics3d.com/documentation/index.html#eventlistenertriggers

used this sometimes
https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

Assets (obj files and textures) provided from Advanced Graphics paper.