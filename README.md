# Open Frameworks projects

OpenFrameworks (personal) sketchbook

## Prerequisite

- [OpenFrameworks](https://github.com/openframeworks/openFrameworks) installed
- [ProjectGenerator](https://github.com/openframeworks/projectGenerator) installed

## Build some apps

This repo doesn't contains binaries but everything to build them, currently theses apps are built from linux using g++, using VS code ([Codium build](https://vscodium.com/)).

You have to clone this repo in oF/apps/.
Foreach project you want to run, you have to create a configure the apps (Make config and vscode tasks and workspace files in my case) with openFrameworks project generator. Moreover project generator will link oF addons and alert you if there are missing on your oF installation.

If you use Vscode to edit and build apps you have to open code \*.code-workspace file (do not open c++ files directly, build tasks will be not initialized).

## Other dependencies

Some projects require extra apps such as gisicle, blender.
