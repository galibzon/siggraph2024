# About UNUSED_PassTemplates.azasset

This file was originally named `PassTemplates.azasset`, and was discoverable
by the asset system at the path `Passes/PassTemplates.azasset`. It had the purpose
to register all the original Pass Templates that come with the **Atom/Feature/Common** Gem
along with the Pass Templates required by this project.

This file was necessary before we started writing C++.

With C++, we can register our custom Pass Templates without needing to copy the
original files from the **Atom/Feature/Common** Gem. And this explains why the name
changed to `UNUSED_PassTemplates.azasset`. We leave it here as a reference, so the developer
can follow the first part of original Siggraph2024 presentation, which avoided C++.
