# make [all | clean | coverage | vscode] [DEBUG=1] [INTERMEDIATE=1] [COVERAGE=1] [APPLICATION=name] [STD=11,14,17,20]

# Rules:
#   all       - builds the application (default rule)
#   clean     - cleans the application build
#   coverage  - runs coverage on the test application (the 'COVERAGE' option should be set)
#   vscode    - add a build configuration to VS Code

# Options:
#   DEBUG=1          - enables a debug build of the application (default is a release build with no debug information)
#   INTERMEDIATE=1   - saves generated intermediate pre-processor and assembly files (default is not to save files)
#   COVERAGE=1       - enables coverage to get added to the application
#   APPLICATION=name - name of the application to build (unit test application is built if no application name is given)
#   STD=nn           - sets which C++ standard should be used to build the application (default is 20)

# Some useful character constants
NULL  :=
SPACE := $(NULL) $(NULL)
COMMA := ,

# @brief A function to escape characters in a text string
# @param $(1) The text string to escape
# @return The escaped text
# @note This will escape the '"', '$' and '\' characters (with a '\' character prefix)
escape = $(subst $$,\$$,$(subst ",\",$(subst \,\\,$(1))))

# @brief Escapes a text item and marks it as an item in a list
# @param $(1) The text item to escape and mark as a list item
# @return The list item
# @note The text item must not contain a '¬' character (as this is used for marking the list item).
list_add = ¬$(call escape,$(1))¬¬

# @brief Gets the text of items that are in an item list
# @param $(1) The text to prefix each item text
# @param $(2) The item list
# @param $(3) The text to suffix each item text
# @return The list of text items
# @note By using list_add() and list_get() you can manipulate text items that have any spaces in them.
# @note This would normally be quite difficult using the normal text functions as these use the space to separate out each text item.
list_get = $(subst ¬,$(1),$(subst ¬¬,$(3),$(subst ¬¬¬,$(3)$(1),$(2))))

# @brief Gets the comma separated list of text items from an item list
# @param $(1) The item list
# @return The comma separated list of text items
# @note Each text item will be escaped and surrounded by quotes (").
# @note By using list_add() and list_get() you can manipulate text items that have any spaces in them.
# @note This would normally be quite difficult using the normal text functions as these use the space to separate out each text item.
list_get_csv = $(filter-out $(COMMA)¬,$(call escape,$(call list_get,",$(1)," $(COMMA)))¬)

ifndef TARGET_PLATFORM
ifeq ($(OS), Windows_NT)
TARGET_PLATFORM := windows
else
TARGET_PLATFORM := linux
endif
endif

ifndef APPLICATION
APPLICATION := test
endif

ifndef STD
STD := 20
endif

APP_NAME := $(notdir $(APPLICATION))

BIN_DIR := ./bin/$(TARGET_PLATFORM)

ifeq ($(COVERAGE),1)
APP_NAME := $(APP_NAME)-coverage
BIN_DIR := $(BIN_DIR)/coverage
CXX_FLAGS += --coverage
LNK_FLAGS += --coverage
endif

ifeq ($(DEBUG),1)
BUILD_NAME := Debug
BIN_DIR := $(BIN_DIR)/debug
CXX_DEFINE += $(call list_add,DEBUG)
CXX_FLAGS += -g3
else
BUILD_NAME := Release
BIN_DIR := $(BIN_DIR)/release
CXX_DEFINE += $(call list_add,RELEASE)
CXX_FLAGS += -O3
endif

ifeq ($(TARGET_PLATFORM),windows)
CXX_DEFINE += $(call list_add,PLATFORM_WINDOWS)
endif

ifeq ($(TARGET_PLATFORM),linux)
CXX_DEFINE += $(call list_add,PLATFORM_LINUX)
endif

ifeq ($(INTERMEDIATE),1)
CXX_FLAGS += -save-temps=obj -fverbose-asm
endif

CXX_VERSION := $(shell $(CXX) -dumpversion)

SRC_DIR := ./source
APP_DIR := $(SRC_DIR)/$(APPLICATION)
INC_DIR := $(call list_add,$(SRC_DIR)/library)
INC_DIR += $(call list_add,$(APP_DIR))
OBJ_DIR := $(BIN_DIR)/obj

MK_FILES := $(APP_DIR)/$(TARGET_PLATFORM).mk
CXX_FILES := $(wildcard $(addsuffix /*.cpp,$(call list_get,,$(INC_DIR),)))
C_FILES := $(wildcard $(addsuffix /*.c,$(call list_get,,$(INC_DIR),)))
O_FILES := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(CXX_FILES:.cpp=.o) $(C_FILES:.c=.o))
S_FILES := $(O_FILES:.o=.s)
II_FILES := $(O_FILES:.o=.ii)
D_FILES := $(O_FILES:.o=.d)
GCX_FILES := $(O_FILES:.o=.gcno) $(O_FILES:.o=.gcda)

BUILD_STD=C++$(STD)
CXX_STD=c++$(STD)

CXX_DEFINE += $(call list_add,APP_NAME="$(APP_NAME) ($(BUILD_NAME) $(BUILD_STD))")
CXX_DEFINE += $(call list_add,STD=$(STD))
CXX_FLAGS += -c -Wall -Werror $(call list_get,-I",$(INC_DIR),") $(call list_get,-D",$(CXX_DEFINE),")

ifeq ($(CXX),g++)
ifeq (,$(filter-out 11 11.% 12 12.% 13 13.%,$(CXX_VERSION)))
CXX_FLAGS += -fno-tree-vrp
endif
LNK_FLAGS += -pthread
endif

BUILD_CFG := ./.vscode/c_cpp_properties.json
BUILD_DEFINE := $(call list_get_csv,$(CXX_DEFINE))
BUILD_INCLUDE := $(call list_get_csv,$(INC_DIR) $(call list_add,$${workspaceFolder}/**))

CFG_NAME := $(APP_NAME) - $(BUILD_NAME) $(BUILD_STD)

APP := $(BIN_DIR)/$(APPLICATION)


all: $(APP)

clean:
	rm -f $(APP)
	rm -f $(O_FILES) $(D_FILES) $(S_FILES) $(II_FILES) $(GCX_FILES)

vscode:
	mkdir -p ./.vscode
	touch $(BUILD_CFG)
	cat $(BUILD_CFG) | ./jsoncfg "/configurations[/name:\"$(CFG_NAME)\"]" \
       "{ \
            \"name\":\"$(CFG_NAME)\", \
            \"includePath\":[ $(BUILD_INCLUDE) ], \
            \"defines\":[ $(BUILD_DEFINE) ], \
            \"cppStandard\":\"$(CXX_STD)\" \
        }" >$(BUILD_CFG).tmp
	mv $(BUILD_CFG).tmp $(BUILD_CFG)

coverage: $(APP)
	$(APP)
	gcov -n -o $(OBJ_DIR)/test $(wildcard $(addsuffix /*.cpp,$(SRC_DIR)/test))


-include $(D_FILES)
-include $(MK_FILES)


$(APP) : $(O_FILES)
	@echo -- BUILDING $@
	mkdir -p $(@D)
	$(CXX) $(O_FILES) $(LNK_FLAGS) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@echo -- COMPILING $<
	mkdir -p $(@D)
	$(CXX) $< $(CXX_FLAGS) -std=$(CXX_STD) -MMD -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo -- COMPILING $<
	mkdir -p $(@D)
	$(CC) $< $(CXX_FLAGS) -MMD -o $@
