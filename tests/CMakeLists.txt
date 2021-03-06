add_custom_target(check)

# add_executable(test_hdf5 "hdf5.cpp")
# target_link_libraries(test_hdf5 ${MPI_LIBRARIES} ${Boost_LIBRARIES} ${Silo_LIBRARIES} ${HDF5_LIBRARIES} fmt Aperture)

# add_executable(test_f "test_f.cpp")
# target_link_libraries(test_f ${MPI_LIBRARIES} ${Boost_LIBRARIES} ${Silo_LIBRARIES} ${HDF5_LIBRARIES} fmt Aperture)

# add_executable(test_free_path "test_free_path.cpp")
# target_link_libraries(test_free_path ${MPI_LIBRARIES} ${Boost_LIBRARIES} ${Silo_LIBRARIES} ${HDF5_LIBRARIES} fmt Aperture)

set(tests_src
  "test.cpp" "test_AD.cpp" "test_config.cpp" "test_env.cpp" "test_stagger.cpp"
  "test_particles.cpp")
add_executable(tests ${tests_src})
target_link_libraries(tests
  PUBLIC AperCuda
  PUBLIC Aperture)

add_custom_target(run_tests
  COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
  DEPENDS tests
  WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_dependencies(check run_tests)

# if(CUDA_FOUND)
set(test_cuda_src
  "test.cu" "test_cuda.cu" "test_multiarray.cu" "test_particles.cu")
add_cuda_executable(test_cuda)
set_target_properties(test_cuda PROPERTIES LINKER_LANGUAGE CUDA)
target_link_libraries(test_cuda
  PUBLIC AperCuda
  PUBLIC Aperture)
add_custom_target(run_cuda_tests
  COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_cuda"
  DEPENDS test_cuda
  WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_dependencies(check run_cuda_tests)
# endif()

add_executable(test_parse "test_parse.cpp")
target_link_libraries(test_parse Aperture AperCuda)
