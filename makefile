# The binaries to build are listed in config/modules.txt (name, file, ...); this file only
# needs touching for something that applies to every module.
#
# Quick reference:
#   make / make all     Build every module
#   make loader         Build only the loader (SCPS_150.97)
#   make kernel         Build only the kernel (KERNEL.XFF)
#   make status         Show matching / build status
#   make progress       Run objdiff report (if installed) and print progress
#   make objdiff        Generate objdiff.json for the GUI/CLI
#   make help           List all targets
#   make clean          Remove build artifacts
#
# See docs/BUILD.md for the full build-system documentation.

MODULES := $(shell awk '$$1 !~ /^\#/ && NF { print $$1 ":" $$2 }' config/modules.txt)
NAMES   := $(foreach m,$(MODULES),$(word 1,$(subst :, ,$(m))))
FILES   := $(foreach m,$(MODULES),$(word 2,$(subst :, ,$(m))))

.DEFAULT_GOAL := all

all: $(NAMES)

# $(1) = name (make loader), $(2) = file (SCPS_150.97)
define module_rules
$(1): $(2).ninja FORCE
	@echo Building $(2)...
	@ninja -f $(2).ninja

build/$(2)/%.o: $(2).ninja FORCE
	@ninja -f $(2).ninja $$@

$(2).ninja:
	@./configure.py -c -t $(1)
endef

$(foreach m,$(MODULES),$(eval $(call module_rules,$(word 1,$(subst :, ,$(m))),$(word 2,$(subst :, ,$(m))))))

## Show a short status summary for every module
status:
	@python3 tools/status.py

## Generate objdiff.json (for GUI / CLI)
objdiff:
	@./configure.py --emit-objdiff objdiff.json
	@echo "Wrote objdiff.json — open this directory in the objdiff GUI, or run:"
	@echo "  make progress"

## Progress report (needs objdiff-cli on PATH)
progress: objdiff
	@mkdir -p build
	@if command -v objdiff-cli >/dev/null 2>&1; then \
		objdiff-cli report generate -p . -o build/report.json -f json && \
		python3 tools/status.py --report build/report.json; \
	else \
		echo "objdiff-cli not found. Install from https://github.com/encounter/objdiff/releases"; \
		echo "Falling back to SHA-1 status only:"; \
		python3 tools/status.py; \
	fi

## Rebuild compile_commands.json for clangd / IDE support
compdb:
	@ninja -t compdb > compile_commands.json 2>/dev/null || true
	@for f in $(FILES); do \
		if [ -f $$f.ninja ]; then ninja -f $$f.ninja -t compdb >> compile_commands.json 2>/dev/null || true; fi; \
	done
	@echo "Updated compile_commands.json"

## Check a single object (usage: make check OBJ=build/SCPS_150.97/src/foo.o)
check:
	@if [ -z "$(OBJ)" ]; then echo "Usage: make check OBJ=build/<module>/path/to/file.o"; exit 1; fi
	@python3 tools/match.py check $(OBJ)

FORCE:

clean:
	@rm -rf asm assets build
	@rm -rf *.ninja objdiff.json build/report.json
	@echo "Cleaned."

help:
	@echo "SOTC build targets"
	@echo ""
	@echo "  make / make all     Build every module listed in config/modules.txt"
	@echo "  make loader         Build SCPS_150.97 only"
	@echo "  make kernel         Build KERNEL.XFF only"
	@echo "  make status         SHA-1 + nonmatching summary"
	@echo "  make progress       objdiff report + percentage (needs objdiff-cli)"
	@echo "  make objdiff        Write objdiff.json for the objdiff GUI"
	@echo "  make compdb         Refresh compile_commands.json for clangd"
	@echo "  make check OBJ=...  Diff a single object (see tools/match.py)"
	@echo "  make clean          Remove asm/, assets/, build/, *.ninja"
	@echo "  make help           This message"
	@echo ""
	@echo "Modules (from config/modules.txt):"
	@$(foreach m,$(MODULES),echo "  $(word 1,$(subst :, ,$(m))) -> $(word 2,$(subst :, ,$(m)))";)
	@echo ""
	@echo "Full documentation: docs/BUILD.md"

.PHONY: all status progress objdiff compdb check clean help FORCE
