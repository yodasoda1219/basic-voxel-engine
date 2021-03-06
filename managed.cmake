if(WIN32)
    include_external_msproject(BasicVoxelEngine ${CMAKE_CURRENT_SOURCE_DIR}/BasicVoxelEngine/BasicVoxelEngine.csproj GUID 2fce1b93-8db7-4844-9a28-96c7b1e70a17)
    include_external_msproject(BasicVoxelEngine.Content ${CMAKE_CURRENT_SOURCE_DIR}/BasicVoxelEngine.Content/BasicVoxelEngine.Content.csproj GUID 09aa96d4-84a6-425d-adee-e299962ddde3)
    set(MANAGED_TARGET_NAMES BasicVoxelEngine BasicVoxelEngine.Content)
else()
    find_file(MSBUILD_ASSEMBLY MSBuild.dll PATHS "/usr/lib" "/usr/local/lib" "/Library/Frameworks/Mono.framework/Versions/Current/lib" PATH_SUFFIXES "mono/msbuild/Current/bin")
    if(MSBUILD_ASSEMBLY)
        message(STATUS "Found MSBuild: ${MSBUILD_ASSEMBLY}")
    else()
        message(FATAL_ERROR "Could not find MSBuild. Is mono installed?")
    endif()
    add_custom_target(managed-code ALL
        COMMAND mono ${MSBUILD_ASSEMBLY} -v:q -p:Configuration=$<CONFIG>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(MANAGED_TARGET_NAMES managed-code)
endif()
set_target_properties(${MANAGED_TARGET_NAMES} PROPERTIES FOLDER BVE)
add_dependencies(${BVE_TARGET_NAME} ${MANAGED_TARGET_NAMES})