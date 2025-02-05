include(NeuronTestHelper)
nrn_add_test_group(
  CORENEURON
  NAME tqperf
  SUBMODULE tests/tqperf
  MODFILE_PATTERNS "mod/*.mod" "modx/*.mod"
  NRNIVMODL_ARGS -incflags "-I${OPENSSL_INCLUDE_DIR}" -loadflags "${OPENSSL_CRYPTO_LIBRARY}"
  SCRIPT_PATTERNS "test1.py" "*.hoc")
if("tqperf-heavy" IN_LIST NRN_ENABLE_MODEL_TESTS)
  set(tqperf_mpi_ranks 16)
else()
  set(tqperf_mpi_ranks 2)
endif()
nrn_add_test(
  GROUP tqperf
  NAME coreneuron
  CONFLICTS nmodl # https://github.com/BlueBrain/nmodl/issues/794
  REQUIRES mpi python coreneuron
  PROCESSORS ${tqperf_mpi_ranks}
  COMMAND ${MPIEXEC_NAME} ${MPIEXEC_NUMPROC_FLAG} ${tqperf_mpi_ranks} ${MPIEXEC_OVERSUBSCRIBE}
          ${MPIEXEC_PREFLAGS} special ${MPIEXEC_POSTFLAGS} -mpi -python test1.py)
