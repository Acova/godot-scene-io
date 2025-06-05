# Godot Scene IO

Godot Secene IO is a Godot Plugin that allows you to export/import a scene, and it's direct dependencies, between Godot projects.

This is an alpha version, still work in progress, and need a lot of testing, but I have successfully tried a few scenarios with simple scenes.

If you want to use it, you can go to the relases and download the last version.

To use it, once installed, in the Godot editor, right click on the scene you want to export and selecet the option "Export Scene".

If it does not exist, a new folder will be created in the project's rootfolder, called `exported_scenes`. There, a new file with the same name as the exported scene and the extension `.gpackage` will appear. This is your exported scene.

You can copy it into another project, there, in the Godot Editor, right click the `.gpackage` file and select the "Import Scene" option.

The scene an it's dependencies will be exported into the project, in the same paths as in the original project.

# TODO

There are many things I want to improve in this addon, this includes:
- Allow the user to select the path where they want to export the scene.
- Inform the user, when importing a scene, of the new files and folders that will be created, and of the conflicts.
- Related to the previous feature, in the case of a conflict, allow the user to specify a new path for the conflicting file.
- Redirect the scene's dependencies into a new folder, so they can be all imported into the same folder.
- Give, generally, more information to the user about what is happening.