BUILD_DIR = build
OUTPUT = sobel_operator

all: clean build

build:
	cmake -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build $(BUILD_DIR)

debug:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)

ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run:
	./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)

run_debug:
	valgrind -s ./$(BUILD_DIR)/$(OUTPUT) $(RUN_ARGS)

clean:
	rm -rf build
