BUILD_DIR = build
OUTPUT = sobel_operator

ifneq (,$(filter run run_debug run_profile gprof cachegrind callgrind,$(firstword $(MAKECMDGOALS))))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

all: clean build

build:
	cmake -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build $(BUILD_DIR)

build_debug:
	cmake -B $(BUILD_DIR) -DENABLE_DEBUG=ON
	cmake --build $(BUILD_DIR)

build_gprof:
	cmake -B $(BUILD_DIR) -DENABLE_PROFILING=ON -DENABLE_DEBUG=ON
	cmake --build $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)

run_debug: build_debug
	valgrind -s ./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)

gprof: build_gprof
	@echo "=== Running with gprof profiling ==="
	./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)
	@echo "=== Generating gprof analysis ==="
	gprof $(BUILD_DIR)/$(OUTPUT) gmon.out > gprof_report.txt
	@echo "gprof analysis saved to $(BUILD_DIR)/gprof_report.txt"

cachegrind: build_debug
	@echo "=== Running Cachegrind analysis ==="
	valgrind --tool=cachegrind ./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)
	cg_annotate --auto=yes cachegrind.out.* > cachegrind_report.txt
	@echo "Cachegrind analysis saved to cachegrind_report.txt"

callgrind: build_debug
	@echo "=== Running Callgrind analysis ==="
	valgrind --tool=callgrind ./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)
	callgrind_annotate --auto=yes callgrind.out.* > callgrind_report.txt
	@echo "Callgrind analysis saved to callgrind_report.txt"

run_profile: gprof cachegrind callgrind
	@echo "==================================="
	@echo "     All profiling completed!"
	@echo "==================================="
	@echo ""
	@echo "Summary:"
	@echo "  - gprof:     function call counts and CPU time"
	@echo "  - cachegrind: cache simulation (memory access patterns)"
	@echo "  - callgrind:  detailed function-level profiling"

clean: clean_profile
	rm -rf $(BUILD_DIR)
	@echo "Build files cleaned"

clean_profile:
	rm -f gmon.out $(BUILD_DIR)/gmon.out gprof_report.txt cachegrind_report.txt callgrind_report.txt
	rm -f cachegrind.out.* callgrind.out.*
	@echo "Profiling files cleaned"

help:
	@echo "Available targets:"
	@echo "  make build              	- Build optimized version"
	@echo "  make build_debug        	- Build with debug (-g)"
	@echo "  make build_gprof        	- Build with profiling (-pg)"
	@echo "  make run -- ARGS           - Run program normally"
	@echo "  make run_debug -- ARGS     - Run with valgrind"
	@echo "  make run_profile -- ARGS   - Run with profiling and generate analysis"
	@echo "  make gprof -- ARGS    		- Run gprof analysis"
	@echo "  make cachegrind -- ARGS    - Run cachegrind analysis"
	@echo "  make callgrind -- ARGS     - Run callgrind analysis"
	@echo "  make clean              	- Clean everything"
	@echo "  make clean_profile      	- Clean only profiling outputs"
	@echo ""
	@echo "Example: make run_profile -- -i input.pgm -o output.pgm"

.PHONY: all build build_debug build_gprof run run_debug gprof cachegrind callgrind clean clean_profile help
