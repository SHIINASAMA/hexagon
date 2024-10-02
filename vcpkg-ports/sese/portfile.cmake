function(remove_empty_directories_recursive dir)
    file(GLOB before_subdirs "${dir}/*")
    foreach (subdir ${before_subdirs})
        if (IS_DIRECTORY ${subdir})
            remove_empty_directories_recursive(${subdir})
        endif ()
    endforeach ()
    file(GLOB after_subdirs "${dir}/*")
    if ("${after_subdirs}" STREQUAL "")
        file(REMOVE_RECURSE "${dir}")
    endif ()
endfunction()

set(SOURCE_PATH ${CURRENT_BUILDTRESS_DIR}/sese)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO libsese/sese
        REF db7a1cc04061d904d142309a981fe81203b2bb0e
        SHA512 73f64cb716657b9ddc00fb084114c0514f5143d11a44677f5bdbb1a405047298487b36bc6035e866629d6563da245f7e5d494cdc7732196ba5fbcbe0abe1d4d0
)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
        FEATURES
        tests            SESE_BUILD_TEST
        mysql            SESE_DB_USE_MARIADB
        sqlite3          SESE_DB_USE_SQLITE
        psql             SESE_DB_USE_POSTGRES
        async-logger     SESE_USE_ASYNC_LOGGER
        archive          SESE_USE_ARCHIVE
        replace-execinfo SESE_REPLACE_EXECINFO
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH "lib/cmake/sese")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

remove_empty_directories_recursive("${CURRENT_PACKAGES_DIR}/include/sese")

vcpkg_copy_pdbs()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE" "${SOURCE_PATH}/NOTICE")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")