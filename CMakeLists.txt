set(name helics)
set(source_files
        model/helics.cc
        model/helics-application.cc
        model/helics-filter-application.cc
        model/helics-id-tag.cc
        model/helics-simulator-impl.cc
        model/helics-static-sink-application.cc
        model/helics-static-source-application.cc
        helper/helics-helper.cc
        )

set(header_files
        model/helics.h
        model/helics-application.h
        model/helics-filter-application.h
        model/helics-id-tag.h
        model/helics-simulator-impl.h
        model/helics-static-sink-application.h
        model/helics-static-source-application.h
        helper/helics-helper.h
        )

set(helics_libs
        /usr/local/lib/libhelics-apps-shared.so
        /usr/local/lib/libhelics-shared.so
        /usr/local/lib/libhelicsSharedLib.so)

set(test_sources
        test/helics-test-suite.cc
        )
# link to dependencies
set(libraries_to_link "${helics_libs}")

set(test_sources)

build_lib("${name}" "${source_files}" "${header_files}" "${libraries_to_link}" "${test_sources}")