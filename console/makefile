include settings.mk

################################################################################

filename = $(1).out

define compile_bin
gcc $(CFLAGS) -o "$@" $^ $(IDIRS:%=-I%) $(LIBDIRS:%=-L%) $(LIBS:%=-l%)
endef

define compile_objs
gcc $(CFLAGS) -c -o "$@" $< $(IDIRS:%=-I%)
endef

################################################################################

# Set prerrequisites
SRCS_C += $(shell find src/ -iname "*.c")
SRCS_H += $(shell find include/ -iname "*.h")
DEPS = $(foreach SHL,$(SHARED_LIBPATHS),$(SHL:%=%/bin/lib$(notdir $(SHL)).so)) \
	$(foreach STL,$(STATIC_LIBPATHS),$(STL:%=%/bin/lib$(notdir $(STL)).a))

# Set header files' directories to (-I)nclude
IDIRS += $(addsuffix /include,$(SHARED_LIBPATHS) $(STATIC_LIBPATHS) .)

# Set library files' directories to (-L)ook
LIBDIRS = $(addsuffix /bin,$(SHARED_LIBPATHS) $(STATIC_LIBPATHS))

# Set intermediate objects
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS_C))

# Set binary target
BIN = bin/$(call filename,$(shell cd . && pwd | xargs basename))

.PHONY: all
all: CFLAGS = $(CDEBUG)
all: $(BIN)

.PHONY: release
release: CFLAGS = $(CRELEASE)
release: $(BIN)

.PHONY: clean
clean:
	-rm -rfv obj bin

.PHONY: watch
watch:
	@test $(shell which entr) || entr
	while sleep 0.1; do \
		find src/ include/ | entr -d make all --no-print-directory; \
	done

$(BIN): $(OBJS) | $(dir $(BIN))
	$(call compile_bin)

obj/%.o: src/%.c $(SRCS_H) $(DEPS) | $(dir $(OBJS))
	$(call compile_objs)

.SECONDEXPANSION:
$(DEPS): $$(shell find $$(patsubst %bin/,%src/,$$(dir $$@)) -iname "*.c") \
	$$(shell find $$(patsubst %bin/,%include/,$$(dir $$@)) -iname "*.h")
	make --no-print-directory -C $(patsubst %bin/,%,$(dir $@))

$(sort $(dir $(BIN) $(OBJS))):
	mkdir -pv $@


################################################################################

LD_LIBRARY_PATH != echo $(addsuffix /bin,$(SHARED_LIBPATHS)) | tr ' ' ':'

.PHONY: start
start: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	valgrind --tool=none ./$(BIN) $(ARGS)

.PHONY: daemon
daemon:
	@test $(shell which entr) || entr
	while sleep 0.1; do \
		find src/ include/ | entr -d make start --no-print-directory; \
	done

.PHONY: memcheck
memcheck: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	valgrind --leak-check=full $(MEMCHECK_FLAGS) ./$(BIN) $(ARGS)

.PHONY: helgrind
helgrind: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	valgrind --tool=helgrind $(HELGRIND_FLAGS) ./$(BIN) $(ARGS)
	
.PHONY: base1
base1: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(BASE1)

.PHONY: base2
base2: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(BASE2)

.PHONY: planificacion1
planificacion1: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(PLANIFICACION1)

.PHONY: planificacion2
planificacion2: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(PLANIFICACION2)

.PHONY: suspension1
suspension1: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(SUSPENSION1)

.PHONY: suspension2
suspension2: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(SUSPENSION2)

.PHONY: suspension3
suspension3: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(SUSPENSION3)

.PHONY: memoria
memoria: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(MEMORIA)

.PHONY: tlb1
tlb1: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(TLB1)

.PHONY: tlb2
tlb2: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(TLB2)

.PHONY: integral1
integral1: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(INTEGRAL1)

.PHONY: integral2
integral2: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(INTEGRAL2)

.PHONY: integral3
integral3: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(INTEGRAL3)

.PHONY: integral4
integral4: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(INTEGRAL4)
	
.PHONY: integral5
integral5: release
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); \
	./$(BIN) $(INTEGRAL5)

