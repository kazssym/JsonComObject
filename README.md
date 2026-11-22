# JsonComObject

A small Windows COM/ATL in-process server that parses JSON and exposes the result as automation-friendly COM values. It targets legacy Windows applications and automation hosts that do not provide native JSON support.

**Status:** Early development. Scalar values and arrays are implemented; JSON objects are not implemented yet.

## Current progress

`JsonParser.ParseJson` currently converts:

| JSON value | COM `VARIANT` value |
| --- | --- |
| `null` | `VT_NULL` |
| Boolean | `VT_BOOL` |
| String | `VT_BSTR` |
| Negative integer | `VT_I8` |
| Non-negative integer | `VT_UI8` |
| Floating-point number | `VT_R8` |
| Array | `VT_DISPATCH` exposing `IJsonArray` |

Arrays support indexed item access, a count, nested arrays, and `IEnumVARIANT` enumeration. Parser and array COM objects expose `IAgileObject`, `IDispatch`, and `ISupportErrorInfo`.

JSON syntax errors reported by `jsoncons` are returned as `DISP_E_EXCEPTION` with error information; other unsupported or internal failures return their corresponding COM HRESULT.

The public COM contracts are defined in `JsonComObject/JsonComObject.idl`. The DLL is implemented with C++23 modules and ATL, and parser functionality uses `jsoncons`.

## Build and test

The project requires Windows, Visual Studio/MSBuild with the ATL toolchain, the `v145` platform toolset, and Windows SDK `10.0.26100.0`. Dependencies are managed through the repository's `vcpkg.json` manifest.

Build the x64 Release configuration from the repository root:

```powershell
msbuild /m /p:Platform=x64 /p:Configuration=Release .
```

Run the Boost.Test executable from its output directory so registration-free COM activation can load the DLL and manifest:

```powershell
Set-Location x64\Release
.\JsonComObject.Test.exe
```

Both `x64` and `Win32` configurations are defined. `JsonComObject.Setup` is an Advanced Installer packaging project and is not part of the normal solution build.

## Project status

The API is still evolving and is not yet a stable public contract. The current tests cover type-library loading, COM interface exposure, scalar parsing, arrays, enumeration, and malformed array input.
