##
# Third-party dependencies in their own subdirectories
##

add_subdirectory(llvm)

# Needs cache vars set by llvm, do not reorder.
add_subdirectory(wasm)
