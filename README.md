# JsonComObject

A small Windows COM/ATL project that parses JSON and exposes the result as automation-friendly COM objects and arrays. The primary targets of this project are
legacy systems that lack built-in JSON support.

Status: work in progress.

## What it does

- Parses JSON text into COM-visible values (not implemented yet)
- Exposes a `JsonParser` coclass through `IJsonParser`
- Provides `IJsonObject` and `IJsonArray` interfaces for item access and enumeration (not implemented yet)

## Build

- Requires Windows and a Visual Studio/MSBuild environment
- Open `JsonComObject.slnx` and build the solution or the `JsonComObject` project

## Notes

- This is an early prototype and not yet a stable public API
- The project is focused on native Windows COM interoperability and automation hosts
