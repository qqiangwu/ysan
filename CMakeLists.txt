cmake_minimum_required(VERSION 3.17)
project(ytk VERSION 0.1.0)

include(CTest)
enable_testing()

# cpp options
add_compile_definitions(BOOST_PROCESS_USE_STD_FS)
add_compile_definitions(FUSION_MAX_VECTOR_SIZE=30)
add_compile_definitions(BOOST_SPIRIT_USE_PHOENIX_V3)
add_compile_definitions(BOOST_SPIRIT_UNICODE)
add_compile_definitions(BOOST_SPIRIT_NO_PREDEFINED_TERMINALS)
add_compile_definitions(BOOST_LOG_WITHOUT_WCHAR_T)
add_compile_definitions(BOOST_SPIRIT_UNICODE)
if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-Werror)
	#add_compile_options(-Wfatal-errors)
	add_compile_options(-Wno-unused-parameter)
	add_compile_options(-Wno-missing-field-initializers)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	add_compile_definitions(BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED)
endif()

if((CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(/Zc:__cplusplus)
	add_compile_options(/Zc:preprocessor)
	add_compile_options(/MP)
endif()


# profile cpp options
if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
	add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
	add_link_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
	message(STATUS "Enable ubsan")
endif()

if((CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
	add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
	add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)
	message(STATUS "Enable asan")
endif()


# cpp std
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Dependency management
include(cmake/conan.cmake)

# Package finders
find_package(Boost REQUIRED)
find_package(Qt5 REQUIRED)

add_library(ytk_deps INTERFACE)
target_link_libraries(ytk_deps INTERFACE Boost::headers Boost::log Boost::system Boost::filesystem Boost::thread)
target_link_libraries(ytk_deps INTERFACE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::QCocoaIntegrationPlugin)

# LIB
add_library(ytk_lib ${CMAKE_SOURCE_DIR}/lib/foundation/debug_support.cpp
${CMAKE_SOURCE_DIR}/lib/foundation/foundation.cpp
${CMAKE_SOURCE_DIR}/lib/foundation/log_support.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/aggr_ai/aggressive_impl.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/aggr_ai/aggressiveai.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_action.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_job.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_military.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_modifier.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_obj.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_property.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_signal.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/arch/architecture_slot.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/common/common_util.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/common/print_troop.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/common/randutil.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/core.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/core/game_obj.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/core/subsys.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/core/timeline.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/diplomacy/diplomacy_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/diplomacy/hash_fraction_pair.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/effect/condition.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/effect/condition/leader_condition.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/effect/condition/mayor_condition.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/effect/effect.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/effect/effect_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/effect/person_effect.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/ai_controller.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_action.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_controller.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_job.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_modifier.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_obj.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_property.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_signal.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/fraction/fraction_slot.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/arch_impl.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/archai/ai_architecture_aggressive.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/archai/ai_architecture_common.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/archai/ai_architecture_defensive.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/archai/ai_architecture_development.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/core_impl.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/fraction_impl.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/person_impl.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/serialization/parser.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/serialization/parser/architecture_parser.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/serialization/parser/fraction_parser.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/serialization/parser/map_parser.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/serialization/parser/person_parser.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/impl/serialization/parser/skipper.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/job/arch_job.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/job/diplomacy_job.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/job/job.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/intruded_arch.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/legion.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/military_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/military_support.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/military_war.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/strategy.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/military/troop.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_action.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_combat.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_modifier.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_obj.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_property.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_signal.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/person_slot.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/person/position.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/report/report_mgr.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/resource/map.cpp
${CMAKE_SOURCE_DIR}/lib/kernel/resource/string_table.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Adaptor/ArchItemModel.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Adaptor/FractionItemModel.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Adaptor/JobItemModel.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Adaptor/PersonItemModel.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Adaptor/QtAIPlayer.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Adaptor/QtPlayer.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Detail/Filter.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Detail/Shim.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Detail/debug_support.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Form/About_form.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Form/ByScreen.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Form/Intro_form.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Form/Select_form.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Frontend.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Main/Byapp.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Main/GameLogic.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Main/ImageProvider.cpp
${CMAKE_SOURCE_DIR}/lib/ui/ScreenView/BattleView.cpp
${CMAKE_SOURCE_DIR}/lib/ui/ScreenView/ByView.cpp
${CMAKE_SOURCE_DIR}/lib/ui/ScreenView/Mapview.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/CityInfo.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/GraphicsCity.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/GraphicsTroop.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/GraphicsWarCity.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/InfoBar.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/PromoteSelect.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/Reporter.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/SelectDialog.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/SysMenu.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/TransportDialog.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/WorkMenu.cpp
${CMAKE_SOURCE_DIR}/lib/ui/Widget/YesOrNo.cpp)
set_target_properties(ytk_lib PROPERTIES OUTPUT_NAME "ytk")
set_target_properties(ytk_lib PROPERTIES AUTOMOC ON)
set_target_properties(ytk_lib PROPERTIES AUTOUIC ON)

target_include_directories(ytk_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(ytk_lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/ui)

target_link_libraries(ytk_lib PUBLIC ytk_deps)

# BIN
add_executable(ytk ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(ytk PRIVATE ytk_lib)
target_link_libraries(ytk PRIVATE ytk_deps)

target_compile_definitions(ytk PRIVATE YTK_VERSION="${PROJECT_VERSION}")

install(TARGETS ytk)

# Dev Package finders
add_library(ytk_dev_deps INTERFACE)
target_link_libraries(ytk_dev_deps INTERFACE ytk_deps)

# Groups
add_custom_target(cppship_group_binaries DEPENDS ytk ytk_lib)
add_custom_target(cppship_group_examples DEPENDS )
add_custom_target(cppship_group_benches DEPENDS )
add_custom_target(cppship_group_tests DEPENDS )

# Footer
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
