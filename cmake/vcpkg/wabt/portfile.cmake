vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH source_path
    REPO WebAssembly/wabt
    REF "${VERSION}"
    SHA512 d4bab93fbddc5f4abac67b4161bdaedc8cc25cd25348553eefb0bb677f5403f8f44ec58c7c094776cc703327f5c1282f696a909efce9c6d281b2cd5e66aca8c7
    HEAD_REF main
)

if ("openssl" IN_LIST FEATURES)
    set(USE_INTERNAL_SHA256 OFF)
else ()
    set(USE_INTERNAL_SHA256 ON)
    vcpkg_from_github(
        OUT_SOURCE_PATH picosha2_path
        REPO okdshin/PicoSHA2
        REF "27fcf6979298949e8a462e16d09a0351c18fcaf2"
        SHA512 9bf66c70c5828a0b89210d1690078a3d418276e9615170cf5b0a72a28bc37087d075f27c07d525ccbb490102b263527e06516fa41ce6231bc918745abbc85fd9
        HEAD_REF master
    )
    file(COPY "${picosha2_path}/" DESTINATION "${source_path}/third_party/picosha2")
endif ()

vcpkg_cmake_configure(
    SOURCE_PATH "${source_path}"
    OPTIONS
    -DWITH_EXCEPTIONS=ON
    -DBUILD_TESTS=OFF
    -DBUILD_TOOLS=OFF
    -DBUILD_LIBWASM=OFF
    -DUSE_INTERNAL_SHA256=${USE_INTERNAL_SHA256}
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/wabt)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

vcpkg_install_copyright(FILE_LIST "${source_path}/LICENSE")
