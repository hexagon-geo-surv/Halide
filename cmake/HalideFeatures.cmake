if (PROJECT_IS_TOP_LEVEL)
    include(FeatureSummary)
    cmake_language(
        DEFER DIRECTORY "${Halide_SOURCE_DIR}"
        CALL feature_summary WHAT ENABLED_FEATURES DISABLED_FEATURES
    )
endif ()

function(_Halide_feature_info opt cond doc)
    if (NOT PROJECT_IS_TOP_LEVEL)
        return()
    endif ()

    set(notice "")
    if (ARG_ADVANCED)
        cmake_language(GET_MESSAGE_LOG_LEVEL log_level)
        if (log_level MATCHES "^(VERBOSE|DEBUG|TRACE)$")
            set(notice " (advanced)")
        else ()
            return()
        endif ()
    endif ()

    add_feature_info("${opt}${notice}" "${cond}" "${doc}")
endfunction()

function(Halide_feature ARG_OPTION ARG_DOC)
    cmake_parse_arguments(PARSE_ARGV 2 ARG "ADVANCED;OFF;ON" "" "DEPENDS")

    if (ARG_ON)
        set(default_value ON)
    elseif (ARG_OFF)
        set(default_value OFF)
    elseif (PROJECT_IS_TOP_LEVEL)
        set(default_value ON)
    else ()
        set(default_value OFF)
    endif ()

    option("${ARG_OPTION}" "${ARG_DOC}" "${default_value}")
    if (ARG_ADVANCED)
        mark_as_advanced("${ARG_OPTION}")
    endif ()

    set(condition "${ARG_OPTION}")
    if (${ARG_OPTION} AND DEFINED ARG_DEPENDS AND NOT (${ARG_DEPENDS}))
        message(WARNING "${ARG_OPTION} forcibly disabled -- requires ${ARG_DEPENDS}")
        set("${ARG_OPTION}" 0)
        set("${ARG_OPTION}" "${${ARG_OPTION}}" CACHE BOOL "${ARG_DOC}" FORCE)
        set(condition 0)
    endif ()

    _Halide_feature_info("${ARG_OPTION}" "${condition}" "${ARG_DOC}")

    set("${ARG_OPTION}" "${${ARG_OPTION}}" PARENT_SCOPE)
endfunction()
