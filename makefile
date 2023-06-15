# make [all | clean | coverage | vscode] [DEBUG=1] [EXAMPLE=name] [STD=11,14,17,20] [COVERAGE=1]

# Rules:
#   all       - builds the application (default rule)
#   clean     - cleans the application build
#   coverage  - runs coverage on the test application (the 'COVERAGE' option should be set)
#   vscode    - add a build configuration to VS Code

# Options:
#   DEBUG=1       - enables a debug build of the application (default is a release build with no debug information)
#   EXAMPLE=name  - name of the example application to build (unit test application is built if no example is given)
#   STD=nn        - sets which C++ standard should be used to build the application (default is 17)
#   COVERAGE=1    - enables coverage to get added to the application


ifndef EXAMPLE
APP_NAME := test
else
APP_NAME := $(EXAMPLE)
endif

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
list_get = $(patsubst ¬%,$(1)%,$(patsubst %¬¬,%$(3),$(patsubst ¬%¬¬,$(1)%$(3),$(2))))

# @brief Gets the comma separated list of text items from an item list
# @param $(1) The item list
# @return The comma separated list of text items
# @note Each text item will be escaped and surrounded by quotes (").
# @note By using list_add() and list_get() you can manipulate text items that have any spaces in them.
# @note This would normally be quite difficult using the normal text functions as these use the space to separate out each text item.
list_get_csv = $(filter-out $(COMMA)¬,$(call escape,$(call list_get,",$(1)," $(COMMA)))¬)

SRC_DIR = ./source
INC_DIR := $(call list_add,$(SRC_DIR)/library)
ifndef EXAMPLE
INC_DIR += $(call list_add,$(SRC_DIR)/test)
else
INC_DIR += $(call list_add,$(SRC_DIR)/example/$(EXAMPLE))
endif

BIN_DIR := ./bin

ifeq ($(COVERAGE),1)
APP_NAME := $(APP_NAME)-coverage
BIN_DIR := $(BIN_DIR)/coverage
endif

ifeq ($(DEBUG),1)
BIN_DIR := $(BIN_DIR)/debug
else
BIN_DIR := $(BIN_DIR)/release
endif
OBJ_DIR := $(BIN_DIR)/obj

C_FILES := $(wildcard $(addsuffix /*.cpp,$(call list_get,,$(INC_DIR),)))
O_FILES := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(C_FILES:.cpp=.o))
D_FILES := $(O_FILES:.o=.d)
ifeq ($(COVERAGE),1)
GCX_FILES := $(O_FILES:.o=.gcno) $(O_FILES:.o=.gcda)
endif

ifeq ($(DEBUG),1)
BUILD_NAME := Debug
C_DEFINE += $(call list_add,DEBUG)
C_FLAGS += -g3
else
BUILD_NAME := Release
C_DEFINE += $(call list_add,RELEASE)
C_FLAGS += -O3
endif
ifeq ($(COVERAGE),1)
C_FLAGS += --coverage
LNK_FLAGS += --coverage
endif

ifndef STD
STD := 17
endif

BUILD_STD=C++$(STD)
C_STD=c++$(STD)

C_DEFINE += $(call list_add,APP_NAME="$(APP_NAME) ($(BUILD_NAME) $(BUILD_STD))")
C_DEFINE += $(call list_add,STD=$(STD))
C_FLAGS += -c -std=$(C_STD) $(call list_get,-I",$(INC_DIR),") $(call list_get,-D",$(C_DEFINE),")

BUILD_CFG := ./.vscode/c_cpp_properties.json
BUILD_DEFINE := $(call list_get_csv,$(C_DEFINE))
BUILD_INCLUDE := $(call list_get_csv,$(INC_DIR) $(call list_add,$${workspaceFolder}/**))

ifeq ($(CC),cc)
CC := g++
endif

APP := $(BIN_DIR)/$(APP_NAME)

all: $(APP)

clean:
	rm -f $(APP)
	rm -f $(O_FILES) $(D_FILES) $(GCX_FILES)

vscode:
	mkdir -p ./.vscode
	touch $(BUILD_CFG)
	cat $(BUILD_CFG) | ./jsoncfg "/configurations[/name:\"$(BUILD_NAME) $(BUILD_STD)\"]" \
       "{ \
            \"name\":\"$(BUILD_NAME) $(BUILD_STD)\", \
            \"includePath\":[ $(BUILD_INCLUDE) ], \
            \"defines\":[ $(BUILD_DEFINE) ], \
            \"cppStandard\":\"$(C_STD)\" \
        }" >$(BUILD_CFG).tmp
	mv $(BUILD_CFG).tmp $(BUILD_CFG)

coverage: $(APP)
	$(APP)
	gcov -n -o $(OBJ_DIR)/test $(wildcard $(addsuffix /*.cpp,$(SRC_DIR)/test))


$(APP) : $(O_FILES)
	@echo -- BUILDING $(NAME)
	mkdir -p $(@D)
	$(CC) $(O_FILES) $(LNK_FLAGS) -o $@

-include $(D_FILES)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@echo -- COMPILING $<
	mkdir -p $(@D)
	$(CC) $< $(C_FLAGS) -MMD -o $@
#	$(CC) $< $(C_FLAGS) -MMD -S -fverbose-asm -o $@
