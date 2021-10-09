# Chira Editor
![Discord](https://img.shields.io/discord/678074864346857482?label=Discord&logo=Discord&logoColor=%23FFFFFF&style=flat-square)

Yet another Portal 2 puzzlemaker, running on a custom engine.

## Building
- **CLion (recommended)**: The project will build without any prior configuration. If developing on Windows, you will need to install the Windows SDK (see below).

- Visual Studio 2019: You will need to install the following components:

![image](https://user-images.githubusercontent.com/26600014/128105181-39065b10-a4f8-49f3-8818-22dcf4bac9e5.png)

Additionally, compiling with MSVC will produce an error for x64 targets, due to a bug with AngelScript. I strongly recommend compiling with MinGW if you can help it, or targeting x86 if you can't.
