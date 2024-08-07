##
# Lists of source files. Keep ALL lists sorted in alphabetical order.
##

# The externally-visible header files that go into making Halide.h.
# Don't include anything here that includes llvm headers.
# Also *don't* include anything that's only used internally (eg SpirvIR.h).
set(HEADER_FILES
    AbstractGenerator.h
    AddAtomicMutex.h
    AddImageChecks.h
    AddParameterChecks.h
    AddSplitFactorChecks.h
    AlignLoads.h
    AllocationBoundsInference.h
    ApplySplit.h
    Argument.h
    AssociativeOpsTable.h
    Associativity.h
    AsyncProducers.h
    AutoScheduleUtils.h
    BoundaryConditions.h
    Bounds.h
    BoundsInference.h
    BoundConstantExtentLoops.h
    BoundSmallAllocations.h
    Buffer.h
    Callable.h
    CanonicalizeGPUVars.h
    ClampUnsafeAccesses.h
    Closure.h
    CodeGen_C.h
    CodeGen_D3D12Compute_Dev.h
    CodeGen_GPU_Dev.h
    CodeGen_Internal.h
    CodeGen_LLVM.h
    CodeGen_Metal_Dev.h
    CodeGen_OpenCL_Dev.h
    CodeGen_Posix.h
    CodeGen_PTX_Dev.h
    CodeGen_PyTorch.h
    CodeGen_Targets.h
    CodeGen_Vulkan_Dev.h
    CodeGen_WebGPU_Dev.h
    CompilerLogger.h
    ConciseCasts.h
    CPlusPlusMangle.h
    ConstantBounds.h
    ConstantInterval.h
    CSE.h
    Debug.h
    DebugArguments.h
    DebugToFile.h
    Definition.h
    Deinterleave.h
    Derivative.h
    DerivativeUtils.h
    Deserialization.h
    DeviceAPI.h
    DeviceArgument.h
    DeviceInterface.h
    Dimension.h
    DistributeShifts.h
    EarlyFree.h
    Elf.h
    EliminateBoolVectors.h
    EmulateFloat16Math.h
    Error.h
    Expr.h
    ExprUsesVar.h
    Extern.h
    ExternFuncArgument.h
    ExtractTileOperations.h
    FastIntegerDivide.h
    FindCalls.h
    FindIntrinsics.h
    FlattenNestedRamps.h
    Float16.h
    Func.h
    Function.h
    FunctionPtr.h
    FuseGPUThreadLoops.h
    FuzzFloatStores.h
    Generator.h
    HexagonOffload.h
    HexagonOptimize.h
    ImageParam.h
    InferArguments.h
    InjectHostDevBufferCopies.h
    Inline.h
    InlineReductions.h
    IntegerDivisionTable.h
    Interval.h
    IntrusivePtr.h
    IR.h
    IREquality.h
    IRMatch.h
    IRMutator.h
    IROperator.h
    IRPrinter.h
    IRVisitor.h
    JITModule.h
    Lambda.h
    Lerp.h
    LICM.h
    LLVM_Output.h
    LLVM_Runtime_Linker.h
    LoopCarry.h
    Lower.h
    LowerParallelTasks.h
    LowerWarpShuffles.h
    MainPage.h
    Memoization.h
    Module.h
    ModulusRemainder.h
    Monotonic.h
    ObjectInstanceRegistry.h
    OffloadGPULoops.h
    OptimizeShuffles.h
    OutputImageParam.h
    ParallelRVar.h
    Param.h
    Parameter.h
    PartitionLoops.h
    Pipeline.h
    Prefetch.h
    Profiling.h
    PurifyIndexMath.h
    PythonExtensionGen.h
    Qualify.h
    Random.h
    RDom.h
    Realization.h
    RealizationOrder.h
    RebaseLoopsToZero.h
    Reduction.h
    RegionCosts.h
    RemoveDeadAllocations.h
    RemoveExternLoops.h
    RemoveUndef.h
    runtime/HalideBuffer.h
    runtime/HalideRuntime.h
    Schedule.h
    ScheduleFunctions.h
    Scope.h
    SelectGPUAPI.h
    Serialization.h
    Simplify.h
    SimplifyCorrelatedDifferences.h
    SimplifySpecializations.h
    SkipStages.h
    SlidingWindow.h
    Solve.h
    SplitTuples.h
    StageStridedLoads.h
    StmtToHTML.h
    StorageFlattening.h
    StorageFolding.h
    StrictifyFloat.h
    StripAsserts.h
    Substitute.h
    Target.h
    TargetQueryOps.h
    Tracing.h
    TrimNoOps.h
    Tuple.h
    Type.h
    UnifyDuplicateLets.h
    UniquifyVariableNames.h
    UnpackBuffers.h
    UnrollLoops.h
    UnsafePromises.h
    Util.h
    Var.h
    VectorizeLoops.h
    WasmExecutor.h
    WrapCalls.h
    )

set(SOURCE_FILES
    AbstractGenerator.cpp
    AddAtomicMutex.cpp
    AddImageChecks.cpp
    AddParameterChecks.cpp
    AddSplitFactorChecks.cpp
    AlignLoads.cpp
    AllocationBoundsInference.cpp
    ApplySplit.cpp
    Argument.cpp
    AssociativeOpsTable.cpp
    Associativity.cpp
    AsyncProducers.cpp
    AutoScheduleUtils.cpp
    BoundaryConditions.cpp
    Bounds.cpp
    BoundsInference.cpp
    BoundConstantExtentLoops.cpp
    BoundSmallAllocations.cpp
    Buffer.cpp
    Callable.cpp
    CanonicalizeGPUVars.cpp
    ClampUnsafeAccesses.cpp
    Closure.cpp
    CodeGen_ARM.cpp
    CodeGen_C.cpp
    CodeGen_D3D12Compute_Dev.cpp
    CodeGen_GPU_Dev.cpp
    CodeGen_Hexagon.cpp
    CodeGen_Internal.cpp
    CodeGen_LLVM.cpp
    CodeGen_Metal_Dev.cpp
    CodeGen_OpenCL_Dev.cpp
    CodeGen_Posix.cpp
    CodeGen_PowerPC.cpp
    CodeGen_PTX_Dev.cpp
    CodeGen_PyTorch.cpp
    CodeGen_RISCV.cpp
    CodeGen_Vulkan_Dev.cpp
    CodeGen_WebAssembly.cpp
    CodeGen_WebGPU_Dev.cpp
    CodeGen_X86.cpp
    CompilerLogger.cpp
    CPlusPlusMangle.cpp
    ConstantBounds.cpp
    ConstantInterval.cpp
    CSE.cpp
    Debug.cpp
    DebugArguments.cpp
    DebugToFile.cpp
    Definition.cpp
    Deinterleave.cpp
    Derivative.cpp
    DerivativeUtils.cpp
    Deserialization.cpp
    DeviceArgument.cpp
    DeviceInterface.cpp
    Dimension.cpp
    DistributeShifts.cpp
    EarlyFree.cpp
    Elf.cpp
    EliminateBoolVectors.cpp
    EmulateFloat16Math.cpp
    Error.cpp
    Expr.cpp
    ExtractTileOperations.cpp
    FastIntegerDivide.cpp
    FindCalls.cpp
    FindIntrinsics.cpp
    FlattenNestedRamps.cpp
    Float16.cpp
    Func.cpp
    Function.cpp
    FuseGPUThreadLoops.cpp
    FuzzFloatStores.cpp
    Generator.cpp
    HexagonOffload.cpp
    HexagonOptimize.cpp
    ImageParam.cpp
    InferArguments.cpp
    InjectHostDevBufferCopies.cpp
    Inline.cpp
    InlineReductions.cpp
    IntegerDivisionTable.cpp
    Interval.cpp
    IR.cpp
    IREquality.cpp
    IRMatch.cpp
    IRMutator.cpp
    IROperator.cpp
    IRPrinter.cpp
    IRVisitor.cpp
    JITModule.cpp
    Lambda.cpp
    Lerp.cpp
    LICM.cpp
    LLVM_Output.cpp
    LLVM_Runtime_Linker.cpp
    LoopCarry.cpp
    Lower.cpp
    LowerParallelTasks.cpp
    LowerWarpShuffles.cpp
    Memoization.cpp
    Module.cpp
    ModulusRemainder.cpp
    Monotonic.cpp
    ObjectInstanceRegistry.cpp
    OffloadGPULoops.cpp
    OptimizeShuffles.cpp
    OutputImageParam.cpp
    ParallelRVar.cpp
    Parameter.cpp
    PartitionLoops.cpp
    Pipeline.cpp
    Prefetch.cpp
    PrintLoopNest.cpp
    Profiling.cpp
    PurifyIndexMath.cpp
    PythonExtensionGen.cpp
    Qualify.cpp
    Random.cpp
    RDom.cpp
    Realization.cpp
    RealizationOrder.cpp
    RebaseLoopsToZero.cpp
    Reduction.cpp
    RegionCosts.cpp
    RemoveDeadAllocations.cpp
    RemoveExternLoops.cpp
    RemoveUndef.cpp
    Schedule.cpp
    ScheduleFunctions.cpp
    SelectGPUAPI.cpp
    Serialization.cpp
    Simplify.cpp
    Simplify_Add.cpp
    Simplify_And.cpp
    Simplify_Call.cpp
    Simplify_Reinterpret.cpp
    Simplify_Cast.cpp
    Simplify_Div.cpp
    Simplify_EQ.cpp
    Simplify_Exprs.cpp
    Simplify_Let.cpp
    Simplify_LT.cpp
    Simplify_Max.cpp
    Simplify_Min.cpp
    Simplify_Mod.cpp
    Simplify_Mul.cpp
    Simplify_Not.cpp
    Simplify_Or.cpp
    Simplify_Select.cpp
    Simplify_Shuffle.cpp
    Simplify_Stmts.cpp
    Simplify_Sub.cpp
    SimplifyCorrelatedDifferences.cpp
    SimplifySpecializations.cpp
    SkipStages.cpp
    SlidingWindow.cpp
    Solve.cpp
    SpirvIR.cpp
    SpirvIR.h
    SplitTuples.cpp
    StageStridedLoads.cpp
    StmtToHTML.cpp
    StorageFlattening.cpp
    StorageFolding.cpp
    StrictifyFloat.cpp
    StripAsserts.cpp
    Substitute.cpp
    Target.cpp
    TargetQueryOps.cpp
    Tracing.cpp
    TrimNoOps.cpp
    Tuple.cpp
    Type.cpp
    UnifyDuplicateLets.cpp
    UniquifyVariableNames.cpp
    UnpackBuffers.cpp
    UnrollLoops.cpp
    UnsafePromises.cpp
    Util.cpp
    Var.cpp
    VectorizeLoops.cpp
    WasmExecutor.cpp
    WrapCalls.cpp
    )

set(C_TEMPLATE_FILES
    CodeGen_C_prologue
    CodeGen_C_vectors
)

set(HTML_TEMPLATE_FILES
    StmtToHTML_dependencies.html
    StmtToHTML.js
    StmtToHTML.css
)

##
# Build and import the runtime.
##

add_subdirectory(runtime)

##
# Build the template files via binary2cpp.
##

add_library(Halide_c_templates OBJECT)
foreach (f IN LISTS C_TEMPLATE_FILES)
    set(SRC "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/${f}.template.cpp>")
    set(DST "c_template.${f}.template.cpp")

    add_custom_command(OUTPUT "${DST}"
                       COMMAND binary2cpp "halide_c_template_${f}" < "${SRC}" > "${DST}"
                       DEPENDS "${SRC}" binary2cpp
                       VERBATIM)
    target_sources(Halide_c_templates PRIVATE ${DST})
endforeach ()

foreach (f IN LISTS HTML_TEMPLATE_FILES)
    set(SRC "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/irvisualizer/html_template_${f}>")
    string(REPLACE "." "_" VARNAME "halide_html_template_${f}")
    set(DST "html_template.${f}.cpp")

    add_custom_command(OUTPUT "${DST}"
                       COMMAND binary2cpp "${VARNAME}" < "${SRC}" > "${DST}"
                       DEPENDS "${SRC}" binary2cpp
                       VERBATIM)
    target_sources(Halide_c_templates PRIVATE ${DST})
endforeach ()


##
# Build the Halide mono-header.
##

set(HALIDE_H "${Halide_BINARY_DIR}/include/Halide.h")
set(LICENSE_PATH "${Halide_SOURCE_DIR}/LICENSE.txt")
add_custom_command(OUTPUT "${HALIDE_H}"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "$<SHELL_PATH:${Halide_BINARY_DIR}/include>"
                   COMMAND build_halide_h "$<SHELL_PATH:${LICENSE_PATH}>" ${HEADER_FILES} > "$<SHELL_PATH:${HALIDE_H}>"
                   DEPENDS build_halide_h "${LICENSE_PATH}" ${HEADER_FILES}
                   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
                   VERBATIM)
add_custom_target(HalideIncludes DEPENDS "${HALIDE_H}")

##
# Define the Halide library target.
##

add_library(Halide
            ${SOURCE_FILES}
            ${HEADER_FILES}
            # Including these as sources works around the need to "install" Halide_initmod
            $<TARGET_OBJECTS:Halide_initmod>
            $<TARGET_OBJECTS:Halide_c_templates>)

##
# Flatbuffers and Serialization dependencies.
##

# Build serialization, enabled by default
option(WITH_SERIALIZATION "Include experimental Serialization/Deserialization code" ON)

# flatbuffers is small and compiles quickly, but if you want/need to use
# a local version (via find_package), configure with FLATBUFFERS_USE_FETCHCONTENT=OFF
option(FLATBUFFERS_USE_FETCHCONTENT "Enable to download the Flatbuffers library via FetchContent" ON)
set(FLATBUFFERS_VER 23.5.26 CACHE STRING "The Flatbuffers version to use (or download)")

if (WITH_SERIALIZATION)
    if (FLATBUFFERS_USE_FETCHCONTENT)
        include(FetchContent)
        message(STATUS "Fetching flatbuffers ${FLATBUFFERS_VER}...")
        FetchContent_Declare(
            flatbuffers
            GIT_REPOSITORY https://github.com/google/flatbuffers.git
            GIT_TAG v${FLATBUFFERS_VER}
            GIT_SHALLOW TRUE
        )
        # configuration for flatbuffers
        set(FLATBUFFERS_BUILD_TESTS OFF)
        set(FLATBUFFERS_INSTALL OFF)
        FetchContent_MakeAvailable(flatbuffers)
        set_target_properties(flatbuffers PROPERTIES POSITION_INDEPENDENT_CODE ON)

        add_library(Halide_flatbuffers INTERFACE)
        target_sources(Halide_flatbuffers INTERFACE $<BUILD_INTERFACE:$<TARGET_OBJECTS:flatbuffers>>)
        target_include_directories(Halide_flatbuffers
                                   SYSTEM # Use -isystem instead of -I; this is a trick so that clang-tidy won't analyze these includes
                                   INTERFACE
                                   $<BUILD_INTERFACE:${flatbuffers_SOURCE_DIR}>/include
                                   $<BUILD_INTERFACE:${flatbuffers_BINARY_DIR}>/include)
        set_target_properties(Halide_flatbuffers PROPERTIES EXPORT_NAME flatbuffers)

        add_executable(flatbuffers::flatc ALIAS flatc)
        message(STATUS "Using fetched-and-built flatbuffers, version ${FLATBUFFERS_VER}")
    else ()
        # Sadly, there seem to be at least three variations of the Flatbuffer package
        # in terms of the case of the relevant CMake files; if we guess wrong, we
        # fail on case-sensitive file systems. We'll try this as a hack workaround:
        # just try all three. (Note that the internal CMake library name appears to be
        # `flatbuffers` in all cases.)
        set(FB_NAME "")
        foreach (N IN ITEMS flatbuffers Flatbuffers FlatBuffers)
            # TODO: should we check the version here?
            find_package(${N} QUIET)
            if (${N}_FOUND)
                set(FB_NAME ${N})
                message(STATUS "Using installed flatbuffers, version ${${N}_VERSION}")
                break()
            endif ()
        endforeach ()

        if (NOT FB_NAME)
            message(FATAL_ERROR "WITH_SERIALIZATION is ON and FLATBUFFERS_USE_FETCHCONTENT is OFF, "
                                "but could not find flatbuffers installed locally. "
                                "Either install flatbuffers or build with WITH_SERIALIZATION=OFF.")
        endif ()

        add_library(Halide_flatbuffers ALIAS flatbuffers::flatbuffers)
    endif ()

    set(fb_dir "${Halide_BINARY_DIR}/flatc/include")

    set(fb_def "${CMAKE_CURRENT_SOURCE_DIR}/halide_ir.fbs")
    set(fb_header "${fb_dir}/halide_ir.fbs.h")
    add_custom_command(
        OUTPUT "${fb_header}"
        COMMAND flatbuffers::flatc --cpp --cpp-std C++17 --no-union-value-namespacing --keep-prefix --filename-suffix ".fbs" -o "${fb_dir}" "${fb_def}"
        DEPENDS "${fb_def}"
        VERBATIM
    )
    add_custom_target(generate_fb_header DEPENDS "${fb_header}")
    set_source_files_properties("${fb_header}" PROPERTIES GENERATED TRUE)

    add_dependencies(Halide generate_fb_header)
    target_include_directories(Halide PRIVATE "$<BUILD_INTERFACE:${fb_dir}>")
    target_link_libraries(Halide PRIVATE Halide_flatbuffers)
    target_compile_definitions(Halide PRIVATE WITH_SERIALIZATION)
endif ()

# Enable serialization testing by intercepting JIT compilation with a serialization roundtrip;
# This is used only for special builds made specifically for testing, and must be disabled by default.
option(WITH_SERIALIZATION_JIT_ROUNDTRIP_TESTING "Intercepting JIT compilation with a serialization roundtrip, for test only" OFF)
if (WITH_SERIALIZATION_JIT_ROUNDTRIP_TESTING)
    if (WITH_SERIALIZATION)
        target_compile_definitions(Halide PRIVATE WITH_SERIALIZATION_JIT_ROUNDTRIP_TESTING)
    endif ()
endif ()

add_library(Halide::Halide ALIAS Halide)

target_link_libraries(Halide PRIVATE Halide::LLVM)
target_link_libraries(Halide PUBLIC Halide::LanguageOptions)
target_compile_features(Halide PUBLIC cxx_std_17)
if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(Halide PRIVATE Halide_STATIC_DEFINE)
endif ()

include(TargetExportScript)
## TODO: implement something similar for Windows/link.exe
# https://github.com/halide/Halide/issues/4651
target_export_script(Halide
                     APPLE_LD "${CMAKE_CURRENT_LIST_DIR}/exported_symbols.osx"
                     GNU_LD "${CMAKE_CURRENT_LIST_DIR}/exported_symbols.ldscript")

set(Halide_SOVERSION_OVERRIDE "${Halide_VERSION_MAJOR}"
    CACHE STRING "SOVERSION to set for custom Halide packaging")
mark_as_advanced(Halide_SOVERSION_OVERRIDE)

set_target_properties(Halide PROPERTIES
                      VERSION "${Halide_VERSION}"
                      SOVERSION "${Halide_SOVERSION_OVERRIDE}")

# Always build with PIC, even when static
set_target_properties(Halide PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Note that we (deliberately) redeclare these versions here, even though the macros
# with identical versions are expected to be defined in source; this allows us to
# ensure that the versions defined between all build systems are identical.
target_compile_definitions(Halide PUBLIC
                           HALIDE_VERSION_MAJOR=${Halide_VERSION_MAJOR}
                           HALIDE_VERSION_MINOR=${Halide_VERSION_MINOR}
                           HALIDE_VERSION_PATCH=${Halide_VERSION_PATCH})


target_include_directories(Halide INTERFACE "$<BUILD_INTERFACE:${Halide_BINARY_DIR}/include>")
add_dependencies(Halide HalideIncludes)

if (TARGET Halide_wabt)
    target_link_libraries(Halide PRIVATE Halide_wabt)
    target_compile_definitions(Halide PRIVATE WITH_WABT)
endif ()

if (TARGET Halide_V8)
    target_link_libraries(Halide PRIVATE Halide_V8)
    target_compile_definitions(Halide PRIVATE WITH_V8)
endif ()

##
# Set compiler options for libHalide
##

set_halide_compiler_warnings(Halide)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # We could expose the /MP flag to all targets, but that might end up saturating the build
    # since multiple MSBuild projects might get built in parallel, each of which compiling their
    # source files in parallel; the Halide library itself is a "knot" point of the build graph,
    # so compiling its files in parallel should not oversubscribe the system
    target_compile_options(Halide PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif ()

target_compile_definitions(Halide
                           PRIVATE
                           # Disable warnings about standard C functions that have more secure replacements
                           # in the Windows API.
                           $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
                           $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
)

##
# Set up additional backend options for Halide
##

option(TARGET_OPENCL "Include OpenCL-C target" ON)
if (TARGET_OPENCL)
    target_compile_definitions(Halide PRIVATE WITH_OPENCL)
endif ()

option(TARGET_METAL "Include Metal target" ON)
if (TARGET_METAL)
    target_compile_definitions(Halide PRIVATE WITH_METAL)
endif ()

option(TARGET_D3D12COMPUTE "Include Direct3D 12 Compute target" ON)
if (TARGET_D3D12COMPUTE)
    target_compile_definitions(Halide PRIVATE WITH_D3D12)
endif ()

if (TARGET_VULKAN)
    message(STATUS "Enabling Vulkan target")
    target_compile_definitions(Halide PRIVATE WITH_VULKAN)
endif()

if (TARGET_SPIRV)
    find_package(
        SPIRV-Headers 1.5.5 REQUIRED
        HINTS "${Halide_SOURCE_DIR}/dependencies/spirv"
    )
    target_compile_definitions(Halide PRIVATE WITH_SPIRV)
    target_link_libraries(Halide PRIVATE "$<BUILD_LOCAL_INTERFACE:SPIRV-Headers::SPIRV-Headers>")
endif ()

option(TARGET_WEBGPU "Include WebGPU target" ON)
if (TARGET_WEBGPU)
    target_compile_definitions(Halide PRIVATE WITH_WEBGPU)
endif()

##
# Add autoschedulers to the build.
##

if (BUILD_SHARED_LIBS)
    message(STATUS "Building autoschedulers enabled")
    add_subdirectory(autoschedulers)
else ()
    message(STATUS "Building autoschedulers disabled (static Halide)")
endif ()
