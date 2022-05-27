
add_executable(fed-filter fed-filter.cc)
target_link_libraries(fed-filter HELICS::helics-shared)

add_executable(fed-pubsub fed-pubsub.cc)
target_link_libraries(fed-pubsub ns3::helics::ns3helics)

add_executable(fed-sndrcv fed-sndrcv.cc)
target_link_libraries(fed-sndrcv HELICS::helics-shared)

#add_executable(new-ns3-filter new-ns3-filter.cc)
#target_link_libraries(new-ns3-filter ns3::helics::ns3helics)

add_executable(ns3-filter ns3-filter.cc)
target_link_libraries(ns3-filter ns3::helics::ns3helics ns3::libcore ns3::libinternet ns3::libpoint-to-point ns3::libcsma ns3::libapplications)

add_executable(ns3-sndrcv ns3-sndrcv.cc)
target_link_libraries(ns3-sndrcv ns3::helics::ns3helics ns3::libcore ns3::libinternet ns3::libpoint-to-point ns3::libcsma ns3::libapplications)

#add_executable(helics-example helics-example.cc)
#target_link_libraries(helics-example ns3::helics::ns3helics)

install(TARGETS fed-filter fed-pubsub fed-sndrcv ns3-filter ns3-sndrcv
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT ns3helics_Examples
)

