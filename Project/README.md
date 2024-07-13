# Building The Project
https://www.docs.o3de.org/docs/welcome-guide/setup/setup-from-github/
It is Assumed you already downloaded O3DE from github,
and registered it in your system:
```
C:\GIT\O3DE> .\scripts\o3de.bat register --this-engine
```

For your convenience two batch files are provided:
./myconfigure.bat and ./mybuild.bat

# ./myconfigure.bat
Configures the project with CMAKE and generates a Visual Studio 17 solution file.

# ./mybuild.bat
Builds the AssetProcessor and the Editor in `profile` configuration. Starts the AssetProcessor automatically.

# Some command line arguments for the Editor. For easy copy/paste
```
--rhi=vulkan
--r_renderPipelinePath=Passes/Mobile/MobileRenderPipeline.azasset
--r_renderPipelinePath=Passes/Siggraph2024Gem/OneTriangleRenderPipeline.azasset
--r_renderPipelinePath=Passes/Siggraph2024Gem/TriforceRenderPipeline.azasset
--r_renderPipelinePath=Passes/Siggraph2024Gem/NormalVectorDisplayRenderPipeline.azasset
--r_renderPipelinePath=Passes/Siggraph2024Gem/StickerRenderPipeline.azasset
```
