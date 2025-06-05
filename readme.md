# 🚀 Godot Scene IO

[![Godot](https://img.shields.io/badge/Godot-Plugin-blue?logo=godot-engine)](https://godotengine.org/)
[![Status](https://img.shields.io/badge/status-alpha-orange)](#)
[![License](https://img.shields.io/badge/license-MIT-green)](#)

**Godot Scene IO** is a Godot plugin that lets you easily **export/import scenes and their direct dependencies between Godot projects**.

> ⚠️ **Alpha Version:**  
> This plugin is a work in progress and needs more testing. It works for simple scenes, but please report any issues!

---

## ✨ Features

- Export a scene and its dependencies as a `.gpackage` file
- Import scenes into other Godot projects
- Keeps original folder structure

---

## 📦 Installation

1. Download the latest release from the [Releases](#) page.
2. Install the plugin in your Godot project.

---

## 🚚 Usage

1. **Export a Scene:**  
   In the Godot editor, right-click the scene you want to export and select **Export Scene**.

2. **Find Your Export:**  
   A new folder called `exported_scenes` will appear in your project root. Your exported scene will be saved as a `.gpackage` file.

3. **Import into Another Project:**  
   Copy the `.gpackage` file into another project. In the Godot editor, right-click the file and select **Import Scene**.

4. **Done!**  
   The scene and its dependencies will be imported, preserving their original paths.

---

## 📝 TODO

- [ ] Allow users to select export paths
- [ ] Show a summary of files/folders to be created or overwritten on import
- [ ] Handle conflicts and let users choose new paths for conflicting files
- [ ] Option to redirect dependencies into a new folder
- [ ] Provide more feedback and information during import/export

---

## 🙏 Contributing

Feedback, bug reports, and pull requests are welcome!