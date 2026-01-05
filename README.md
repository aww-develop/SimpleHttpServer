## Simple Http Server for Unreal Engine

Simple Http Server is a plugin for Unreal Engine that lets you host an HTTP server with support for both Blueprint and C++.

Please refer to the following samples:
- `Plugins/SimpleHttpServer/Content/Samples`
- `Plugins/SimpleHttpServer/Source/SimpleHttpServerSample` (C++ Implementation)
- `Samples` (web front-end example)

## Notice
These samples cannot be packaged or run in Standalone because the sample module is set to `UncookedOnly`.
If you want to use the Simple Http Server sample in a packaged build, change the sample module type to `Runtime`.
