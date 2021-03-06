ExternalData_Expand_Arguments(VTKData _
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/0/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/0.5/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/0.5/uniform/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/1/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/1/uniform/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/1.5/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/1.5/uniform/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/2/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/2/uniform/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/2.5/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/2.5/uniform/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/constant/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/constant/polyMesh/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/system/,REGEX:.*}"
  "DATA{${VTK_TEST_INPUT_DIR}/OpenFOAM/cavity/system/,REGEX:.*}"
  )
vtk_add_test_cxx(vtkIOParallelCxxTests tests
  TestPOpenFOAMReader.cxx
  TestBigEndianPlot3D.cxx,NO_VALID
  )
vtk_test_cxx_executable(vtkIOParallelCxxTests tests)

if(VTK_USE_LARGE_DATA)
  include(vtkMPI)

  # Tell ExternalData to fetch test input at build time.
  ExternalData_Expand_Arguments(VTKData _
    "DATA{${VTK_TEST_INPUT_DIR}/SLAC/ll-9cell-f523/,REGEX:.*}"
    )

  vtk_add_test_mpi(vtkIOParallelCxxTests-MPI tests
    TESTING_DATA
    PSLACReaderLinear.cxx
    PSLACReaderQuadratic.cxx
    )
  vtk_test_mpi_executable(vtkIOParallelCxxTests-MPI tests)
endif()
