
local common = require "CommonStyle"
local util = require "util"

local my_style = {}
my_style.header = {}
my_style.source = {}

local function underscore_lower(x)
    return "_" .. x:lower()
end

local function uncamel(x)
    return x
        :gsub("([1-3])D", "%1d")            -- special case "3D" suffixes
        :gsub("[A-Z]", underscore_lower)    -- uncamel
        :gsub("^_+", "")                    -- remove prefixed "_"
        :gsub("^getn_", "get_n_")           -- special case "GetnBlah" as "get_n_blah"
        :gsub("^readn_", "read_n_")         -- special case "ReadnBlah" as "read_n_blah"
end

local function fix_gl_type(x)
    return x
        :gsub("GLDEBUGPROC", "debug_proc_type")
        :gsub("(debug_proc_type)%s+([a-z_])", "%1* %2") -- `...debug_proc_type hello` -> `...debug_proc_type* hello`
        :gsub("GL([a-z0-9_]+)", "%1_type")              -- `GLblah` -> `blah_type`
        :gsub("const%s+([a-z_]+)%s*%*", "%1 const*")    -- `const blah_type *` -> `blah_type const*`
        :gsub("(const%*)([a-z_])", "%1 %2")             -- `... const*blah` -> `... const* blah`
        :gsub(",%s*", ", ")                             -- append space to comma
end

local function fix_typedef(x)
    return x
        :gsub("typedef%s+([^(]-)%s*%([^*]*%*%s*([^)]+)%)%s*(%(.-%));", "using %2 = %1 %3;") -- typedef void (APIENTRY *blah)(....)
        :gsub("typedef%s+(.*)%s+%*(%S+);", "using %2 = %1*;")                               -- typedef A *B
        :gsub("typedef%s+(.*)%s+(%S+);", "using %2 = %1;")                                  -- typedef A B
end

local function StartNamespace(hFile, namespaceName)
    if namespaceName then
        hFile:fmt("namespace %s\n", namespaceName)
    else
        hFile:fmt("namespace\n")
    end
    hFile:write("{\n")
    hFile:inc()
end

local function EndNamespace(hFile, namespaceName)
    hFile:dec()
    hFile:write("}\n")
    if namespaceName then
        hFile:fmt("using namespace %s;\n", namespaceName)
    end
end


local function write_fix_ifdef_indent_filtered(hFile, xs, filter)
    filter = filter or function (x) return x end
    for _, str in ipairs(xs) do
        for x in str:gmatch("[^\n]*") do
            local cx = x:gsub("^%s+", ""):gsub("%s+$", "")

            if (cx:match("^#endif") or cx:match("^#el")) then
                hFile:dec()
            end

            local line = filter(cx)
            if line then
                hFile:write(line, "\n")
            end

            if (cx:match("^#if") or cx:match("^#el")) then
                hFile:inc()
            end
        end
    end
end

local function make_set(items)
    local set = {}
    for item in items:gmatch("%S+") do
        set[item] = true
    end
    return set
end

local problematic_names = make_set[[
    bool true false
    short int
    float double
    and or xor
]]

local constant_names = make_set[[
    timeout_ignored
]]

local gl_typedefs = {}
local gl_legacy_typedefs = {}
local gl_constants = {}

----------------------------------------------------
-- Global styling functions.
function my_style.WriteLargeHeading(hFile, headingName)
	hFile:write(string.rep("/", 6 + #headingName), "\n")
	hFile:write("// ", headingName, "\n")
	hFile:write(string.rep("/", 6 + #headingName), "\n")
end

function my_style.WriteSmallHeading(hFile, headingName)
	hFile:write("// ", headingName, "\n")
end

function my_style.WriteFilePreamble(hFile, specData, spec, options)
	common.WriteCPPGeneratorInfo(hFile, specData, spec, options)
end



------------------------------------------------------
-- Header styling functions

function my_style.header.GetFilename(basename, options)
	return basename .. ".hh"
end

function my_style.header.WriteBlockBeginIncludeGuard(hFile, spec, options)
	hFile:write("#pragma once\n")
end

function my_style.header.WriteBlockEndIncludeGuard(hFile, spec, options)
end

function my_style.header.WriteInit(hFile, spec, options)
	write_fix_ifdef_indent_filtered(hFile, { spec.GetHeaderInit() })
end

function my_style.header.WriteStdTypedefs(hFile, specData, spec, options)
end

local function format_passthru(x)
    -- core only
    if x:match("typedef.-AMD") or x:match("typedef.-ARB") or x:match("typedef.-NV") or x:match("typedef.-EXT") then
        return nil
    end
    -- no enum
    if x:match("typedef.-GLenum;") then
        return nil
    end
    local cx = fix_gl_type(fix_typedef(x))
    -- postponed GLtype
    if x:match("typedef.-GL") or x:match("struct") then
        table.insert(gl_legacy_typedefs, x)
        table.insert(gl_typedefs, cx)
        return nil
    end
    return cx
end

local function write_gl_typedefs(hFile)
    StartNamespace(hFile, "types")
    hFile:write('extern "C"\n')
    hFile:write('{\n')
    hFile:inc()
    for _, def in ipairs(gl_typedefs) do
        hFile:fmt("%s\n", def)
    end
    hFile:write("#ifdef GL_LOADER_WANT_LEGACY_TYPES\n")
    StartNamespace(hFile, "legacy")
    hFile:write("typedef enum_type GLenum;\n")
    for _, def in ipairs(gl_legacy_typedefs) do
        hFile:fmt("%s\n", def)
    end
    EndNamespace(hFile, "legacy")
    hFile:write("#endif // GL_LOADER_WANT_LEGACY_TYPES\n")
    hFile:dec()
    hFile:write('}\n')
    EndNamespace(hFile, "types")
end

local function write_gl_constants(hFile)
    StartNamespace(hFile, "constants")
    for _, c in ipairs(gl_constants) do
        hFile:fmt("static constexpr auto %s = %s;\n", c.name, c.value)
    end
    EndNamespace(hFile, "constants")
end

function my_style.header.WriteSpecTypedefs(hFile, specData, spec, options)
	hFile:push()
    write_fix_ifdef_indent_filtered(hFile, specData.funcData.passthru, format_passthru)
	hFile:pop()
end

function my_style.header.WriteBlockBeginDecl(hFile, spec, options)
	if(#options.prefix > 0) then
		StartNamespace(hFile, options.prefix)
	end
	StartNamespace(hFile, spec.FuncNamePrefix())
end

function my_style.header.WriteBlockEndDecl(hFile, spec, options)
    EndNamespace(hFile)
    hFile:write("\n")
end

function my_style.header.WriteBlockBeginExtVarDecl(hFile, spec, options)
end

function my_style.header.WriteBlockEndExtVarDecl(hFile, spec, options)
end

function my_style.header.WriteExtVariableDecl(hFile, extName,
	specData, spec, options)
end

function my_style.header.WriteBlockBeginEnumDecl(hFile, spec, options)
    StartNamespace(hFile, "enums")
	hFile:write("enum enum_type : unsigned int\n")
	hFile:write("{\n")
	hFile:inc()
end

function my_style.header.WriteBlockEndEnumDecl(hFile, spec, options)
	hFile:dec()
	hFile:write("};\n")
    EndNamespace(hFile, "enums")
    write_gl_constants(hFile)
    write_gl_typedefs(hFile)
end

local function GenEnumName(enum)
    local name = enum.name:lower()
    if name:match("^[^a-z_]") then
        error("Invalid enum name: " .. name)
    end
    if problematic_names[name] then
        name = name .. "_"
    end
    return name
end

function my_style.header.WriteEnumDecl(hFile, enum, enumTable, spec, options, enumSeen)
	if(enumSeen[enum.name]) then
		hFile:fmt("// %s taken from ext: %s\n", enum.name, enumSeen[enum.name])
	else
		local enumName = GenEnumName(enum)
		local enumValue = common.ResolveEnumValue(enum, enumTable)

        if constant_names[enumName] then
            table.insert(gl_constants, { name = enumName, value = enumValue })
            return
        end

		local lenEnum = #enumName
		local numIndent = 33

		local numSpaces = numIndent - lenEnum
		if(numSpaces < 1) then
			numSpaces = 1
		end

		hFile:fmt("%s%s= %s,\n",
			enumName,
			string.rep(" ", numSpaces),
			enumValue)
	end
end

function my_style.header.WriteBlockBeginFuncDecl(hFile, spec, options)
    StartNamespace(hFile, "funcs")
end

function my_style.header.WriteBlockEndFuncDecl(hFile, spec, options)
    EndNamespace(hFile, "funcs")
end

function my_style.header.WriteBlockBeginExtFuncDecl(hFile, extName, spec, options)
	--Block containing all spec function declarations for a particular extension.
	--Useful for include-guards around extension function pointers.
end

function my_style.header.WriteBlockEndExtFuncDecl(hFile, extName, spec, options)
	--Block containing all spec function declarations for a particular extension.
end

local function GenFuncPtrName(func, spec, options)
    return uncamel(func.name)
end

local function GenFuncPtrTypedefName(func, spec, options)
    return uncamel(GenFuncPtrName(func, spec, options)) .. "_type"
end

local function WriteFuncDefAPI(hFile, func, spec, options)
    hFile:fmt("DEFINE_API(%s, %s, %s);\n",
        fix_gl_type(common.GetFuncReturnType(func)),
        GenFuncPtrName(func, spec, options),
        fix_gl_type(common.GetFuncParamList(func)))
end

local function GenFuncPtrDefDirect(func, spec, options)
    return string.format("%s (*%s)(%s)",
        fix_gl_type(common.GetFuncReturnType(func)),
        GenFuncPtrName(func, spec, options),
        fix_gl_type(common.GetFuncParamList(func, true)))
end

function my_style.header.WriteFuncDecl(hFile, func, spec, options)
	hFile:write("extern ",
		GenFuncPtrDefDirect(func, spec, options),
		";\n")
end

function my_style.header.WriteBlockBeginSysDecl(hFile, spec, options)
	StartNamespace(hFile, "loader")
end

function my_style.header.WriteBlockEndSysDecl(hFile, spec, options)
	EndNamespace(hFile, "loader")
end

function my_style.header.WriteUtilityDecls(hFile, spec, options)
	--Write declarations for public utility stuff. Enums for return values, etc.
end

function my_style.header.WriteMainLoaderFuncDecl(hFile, spec, options)
	hFile:fmt("bool load(%s);\n", spec.GetLoaderParams())
end

function my_style.header.WriteVersioningFuncDecls(hFile, spec, options)
end

--------------------------------------------------
-- Source code styling functions.
function my_style.source.GetFilename(basename, options)
	return basename .. ".cc"
end

function my_style.source.WriteIncludes(hFile, basename, spec, options)
    local base = util.ParsePath(my_style.header.GetFilename(basename, options))
	hFile:write("#define GL_LOADER_WANT_LEGACY_TYPES\n")
    hFile:fmt('#include "%s"\n', base)
	hFile:write("#undef GL_LOADER_WANT_LEGACY_TYPES\n")
end

function my_style.source.WriteLoaderData(hFile, spec, options)
    hFile:write("using namespace gl::types;\n")
	hFile:writeblock((fix_gl_type(spec.GetLoaderFunc())))
end

function my_style.source.WriteBlockBeginDef(hFile, spec, options)
	if(#options.prefix > 0) then
		StartNamespace(hFile, options.prefix)
	end
	StartNamespace(hFile, spec.FuncNamePrefix())
end

function my_style.source.WriteBlockEndDef(hFile, spec, options)
    EndNamespace(hFile)
    hFile:write("\n")
end

function my_style.source.WriteBlockBeginExtVarDef(hFile, spec, options)
end

function my_style.source.WriteBlockEndExtVarDef(hFile, spec, options)
end

function my_style.source.WriteExtVariableDef(hFile, extName, specData, spec, options)
end

function my_style.source.WriteBlockBeginExtFuncDef(hFile, extName, spec, options)
	--Block containing the extension function definitions and load function
	--for the functions in the extension `extName`.
end

function my_style.source.WriteBlockEndExtFuncDef(hFile, extName, spec, options)
	--Block containing the extension function definitions and load function
	--for the functions in the extension `extName`.
end

function my_style.source.WriteFuncDef(hFile, func, spec, options, funcSeen)
	if (not funcSeen[func.name]) then
		WriteFuncDefAPI(hFile, func, spec, options)
	end
end

function my_style.source.WriteBlockBeginExtLoader(hFile, extName, spec, options)
end

function my_style.source.WriteBlockEndExtLoader(hFile, extName, spec, options)
end

function my_style.source.WriteExtFuncLoader(hFile, func, spec, options)
end

function my_style.source.WriteBlockBeginCoreFuncDef(hFile, version, spec, options)
    StartNamespace(hFile, "funcs")
	hFile:write("#define DEFINE_API(RESULT, VAR, ARGS...) \\\n")
	hFile:inc()
	hFile:write("using VAR##_type = RESULT (ARGS); \\\n")
	hFile:write("VAR##_type* VAR\n")
	hFile:dec()
end

function my_style.source.WriteBlockEndCoreFuncDef(hFile, version, spec, options)
	hFile:write("#undef DEFINE_API\n")
    EndNamespace(hFile, "funcs")
end

local function GenCoreLoaderFuncName(version, spec, options)
    return "load_core"
end

function my_style.source.WriteBlockBeginCoreLoader(hFile, version, spec, options)
    StartNamespace(hFile)
	hFile:write("#define LOAD_PROC(VAR, NAME) do { \\\n")
	hFile:inc()
	hFile:fmt("VAR = reinterpret_cast<VAR##_type*>(IntGetProcAddress(NAME)); \\\n")
	hFile:write("if (VAR == nullptr) return false; \\\n")
	hFile:dec()
	hFile:write("} while (false)\n")
	hFile:fmt("bool %s()\n", GenCoreLoaderFuncName(version, spec, options))
	hFile:write("{\n")
	hFile:inc()
end

function my_style.source.WriteBlockEndCoreLoader(hFile, version, spec, options)
	hFile:write "return true;\n"
	hFile:dec()
	hFile:write("}\n")
	hFile:write("#undef LOAD_PROC\n")
    EndNamespace(hFile)
end

function my_style.source.WriteCoreFuncLoader(hFile, func, spec, options)
	hFile:fmt('LOAD_PROC(%s, "%s%s");\n',
		GenFuncPtrName(func, spec, options),
		spec.FuncNamePrefix(), func.name)

	if(func.name:match("EXT$")) then
        error("There should be no EXT functions")
	end
end

function my_style.source.WriteExtStringFuncDef(hFile, specData, spec, options, funcSeen)
end

function my_style.source.WriteBlockBeginSysDef(hFile, spec, options)
	StartNamespace(hFile, "loader")
end

function my_style.source.WriteBlockEndSysDef(hFile, spec, options)
	EndNamespace(hFile, "loader")
end

function my_style.source.WriteUtilityDefs(hFile, specData, spec, options)
end

function my_style.source.WriteMainLoaderFunc(hFile, specData, spec, options)
	hFile:fmt("bool load(%s)\n", spec.GetLoaderParams())
	hFile:write("{\n")
	hFile:inc()
    hFile:fmt("return load_core();\n")
	hFile:dec()
	hFile:write("}\n")
end

function my_style.source.WriteVersioningFuncs(hFile, specData, spec, options)
end


--------------------------------------------------
-- Style retrieval machinery

local function Create()
	return common.DeepCopyTable(my_style), common.GetStandardStructure()
end

return { Create = Create }

