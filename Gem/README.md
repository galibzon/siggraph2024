# Using this Gem In Other Projects
As a regular O3DE Gem, this Gem can be used with other projects.

1. Register this Gem.
Assumes O3DE is located at `C:\GIT\o3de`, and this Gem located at: `C:\GIT\Siggraph2024\Gem`
```
> cd C:\GIT\o3de
> .\scripts\o3de.bat register -gp C:\GIT\Siggraph2024\Gem
```

2. You can now use the Project Manager (aka O3DE.exe) to add this Gem to another project.
3. Or manually in `<PROJECT ROOT>/project.json` add `Siggraph2024Gem` in `"gem_names": [...]` list. 
