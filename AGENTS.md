# AGENTS.md

## Project overview

JsonComObject is a Windows-only COM/ATL in-process server that parses JSON for
automation-friendly clients. The public COM contracts are defined in
`JsonComObject/JsonComObject.idl`; the DLL implementation uses C++23 modules.

- `JsonComObject/`: ATL DLL, module partitions, MIDL contract, resources, and
  registration metadata.
- `JsonComObject.Test/`: Boost.Test executable for parser and COM integration
  tests.
- `JsonComObject.Setup/`: Advanced Installer packaging project. It is not
  built as part of the normal solution build.
- `JsonComObject.props`: shared C/C++ language, UTF-8, and MIDL settings.
- `vcpkg.json`: manifest dependencies (`boost-test`, `jsoncons`, `ms-gsl`,
  and `wil`).

## Environment and dependencies

Use Windows with a Visual Studio Developer PowerShell/Command Prompt that
provides MSBuild and the Windows/ATL toolchain required by the projects. The
projects currently require the `v145` platform toolset and Windows SDK
`10.0.26100.0`.

Run commands from the repository root. With vcpkg integrated into Visual
Studio/MSBuild, manifest dependencies are restored during the build. The CI
workflow also runs:

```powershell
vcpkg integrate install
nuget restore .
```

Do not add a second dependency system or hard-code machine-specific package
paths. Add native dependencies to `vcpkg.json` when needed.

## Build and test

The CI-equivalent build is:

```powershell
msbuild /m /p:Platform=x64 /p:Configuration=Release .
```

Run the resulting test executable from its output directory:

```powershell
Set-Location x64\Release
.\JsonComObject.Test.exe
```

For focused iteration, build a specific project:

```powershell
msbuild /m /p:Platform=x64 /p:Configuration=Debug JsonComObject.Test\JsonComObject.Test.vcxproj
```

Both `x64` and `Win32` configurations are defined. Keep changes compatible
with Debug and Release. Do not report a change as complete unless the
affected configuration builds and the Boost.Test executable passes.

## Architecture and implementation rules

- Treat `JsonComObject.idl` as the public binary contract. Coordinate every
  interface, coclass, UUID, dispatch ID, and parameter change with the ATL
  implementation and COM tests.
- `JsonComObject.cpp` is the primary module interface. Array and parser
  implementations are internal partitions in `JsonComObject-Array.cpp` and
  `JsonComObject-Parser.cpp`.
- C++ module interface units declare `module;` before global-module-fragment
  includes. Preserve this structure and the `CompileAsCppModule` /
  `CompileAsCppModuleInternalPartition` settings in the project file.
- The C++ implementation must include `pch.h` where the Visual C++ project
  expects precompiled headers. Non-PCH files are configured explicitly;
  do not change project settings merely to accommodate an include.
- Keep registration resources and registration behavior synchronized:
  `res/JsonComObject.manifest`, `res/JsonParser.rgs`, `resource.h`, and
  `DllExport.cpp` all participate in activation.
- Never hand-edit MIDL outputs such as `JsonComObject_i.h`,
  `JsonComObject_i.c`, proxy files, or `.tlb` files. They are generated under
  the configured `Generated Files` output.
- Preserve apartment safety and automation semantics. Public COM methods are
  HRESULT-based, validate caller pointers before use, and return failures
  instead of allowing C++ exceptions to cross COM.
- Use ATL/WIL wrappers (`CComPtr`, `CComVariant`, `CComBSTR`,
  `RETURN_*`, `THROW_*`) and the existing explicit error propagation patterns.
  Initialize output `VARIANT` values consistently and preserve ownership
  transfer when detaching them.
- Parser/array objects use the ATL multithreaded model and expose
  `IAgileObject`. Protect shared mutable state and keep lock scope consistent
  with the existing array implementation.
- Use `gsl::narrow` for checked C++/COM size conversions rather than unchecked
  casts. Return an appropriate HRESULT if narrowing fails.

## Testing expectations

- Add tests to the existing Boost.Test project. Update the `.vcxproj` only when
  adding a new source file.
- Test both successful values and failure HRESULTs at the COM boundary.
  Parser tests should verify the resulting `VARTYPE` as well as its value.
- When a change affects an interface, also verify:
  - `IDispatch` and `ISupportErrorInfo` exposure where applicable;
  - `IAgileObject` exposure;
  - type-library loading or registration metadata;
  - safe behavior for invalid pointers, bounds, and malformed JSON.
- The test global fixture initializes the MTA. Use apartment-independent test
  code and do not initialize/tear down COM in individual test cases unless a
  test specifically needs a different model.
- Integration tests use registration-free COM through the application
  manifest. Run the test from its output directory so the DLL and embedded
  metadata remain available.

## Style and file hygiene

- Follow `.editorconfig`: LF for normal text/source files, CRLF with no final
  newline for Visual Studio XML files, two-space indentation in XML/JSON, and
  UTF-8 source. `.rc` files are UTF-16LE.
- Match the existing C++ layout: namespaces around project types, four-space
  indentation, `auto`-style declarations, braced blocks, and two-space
  comment conventions around namespaces where currently used.
- Preserve the MIT/SPDX copyright header on C++ source files.
- Keep generated output, `vcpkg_installed`, `.vs`, and build directories out
  of commits; they are ignored.
- Avoid unrelated project-file rewrites by Visual Studio. Keep edits to
  `.vcxproj` and `.filters` minimal and targeted.
