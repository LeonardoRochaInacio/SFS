#Nome do target executável
APP := sfs #Simple File System

#Diretivas do compilador
COMPILERFLAGS = -ggdb -std=c++14 -pthread

#Diretórios mínimos
GENERAL_DIR_SRC := ./source
GENERAL_DIR_INC := ./include

#Diretórios de saída
DIR_BIN := ./bin
DIR_OBJ := ./obj
DIR_DEP := .dependencies

#Lista de source files (.cpp ou .c)
SOURCE_FILES := $(shell find $(GENERAL_DIR_SRC) -name *.cpp)

#Lista de diretórios dentro de SRC, serve de template para replicar pasta em OBJ e DEPS
DIR_LIST_IN_SRC := $(shell find $(GENERAL_DIR_SRC) -type d)

#Lista de diretórios de include
DIR_LIST_INCLUDE := $(shell find $(GENERAL_DIR_INC) -type d)

#Lista de diretorios para expandir para in deps
DIR_LIST_IN_DEPS := $(subst $(GENERAL_DIR_SRC), $(DIR_DEP), $(DIR_LIST_IN_SRC))

#Lista de diretorios para expandir para in deps
DIR_LIST_IN_OBJS := $(subst $(GENERAL_DIR_SRC), $(DIR_OBJ), $(DIR_LIST_IN_SRC))

#Lista de futuros objects
OBJS_FILES := $(subst $(GENERAL_DIR_SRC), $(DIR_OBJ), $(SOURCE_FILES:.cpp=.o))

#Lista de dependencias
DEPS_FILES := $(subst $(GENERAL_DIR_SRC), $(DIR_DEP), $(SOURCE_FILES:.cpp=.d))

#Lista de includes já com flag -I
INCLUDE_FLAGS := $(addprefix -I , $(DIR_LIST_INCLUDE))

#---------------------------------------------------------------------------------------------------------------------#
build: $(DIR_LIST_IN_DEPS) $(DIR_LIST_IN_OBJS) $(APP)

$(APP): $(OBJS_FILES)
	@echo Linkando objetos $(OBJS_FILES) em $(DIR_BIN)/$(APP)
	@g++ $(COMPILERFLAGS) $(OBJS_FILES) -o $(DIR_BIN)/$(APP)
	
$(DIR_OBJ)/%.o: $(GENERAL_DIR_SRC)/%.cpp
	@echo Compilando $<
	@g++ $(COMPILERFLAGS) -c $< $(INCLUDE_FLAGS) -o $(DIR_OBJ)/$*.o
	@echo Gerando dependência para $@
	@g++ -c -MT $@ -MM $< -o $(DIR_DEP)/$*.d

#Cria todos diretorios que nao existem em dependencias
$(DIR_LIST_IN_DEPS):
	@mkdir -p $@

#Cria todos diretorios que nao existem em objects
$(DIR_LIST_IN_OBJS):
	@mkdir -p $@

#Limpa objs e dependencias
.PHONY: clean
clean:
	@rm -r -f $(DIR_DEP)
	@rm -r -f $(DIR_OBJ)
	@echo "Objetos e Dependências deletadas!"

-include $(wildcard $(DEPS_FILES))