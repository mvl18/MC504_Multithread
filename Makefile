PROJECT_NAME = santa

CC = gcc

BUILD_FLAGS = -O2 -Wall

INCLUDES = -Ilib -pthread -lSDL2 -lSDL2_image -lSDL2_ttf -Iassets
BUILD_DIR = build
SRC_DIR = src

# Códigos ANSI para cores
BLUE := \033[1;34m
GREEN := \033[1;32m
YELLOW := \033[1;33m
RED := \033[1;31m
NC := \033[0m # Reset de cor

# Encontra todos os arquivos .c em src/
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Converte nomes dos .c em nomes de .o no diretório build/
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

.PHONY: all clean buildFolder

all: buildFolder $(PROJECT_NAME)

# Linka os objetos gerando o binário final
$(PROJECT_NAME): $(OBJECTS)
	@printf "$(GREEN)[LINK]$(NC) Criando o executável %s\n" $@
	$(CC) $^ -o $(BUILD_DIR)/$@ $(BUILD_FLAGS) $(INCLUDES)

# Compila cada .c em um .o correspondente
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c buildFolder
	@printf "$(BLUE)[CC]$(NC) Compilando %s\n" $<
	$(CC) -c $< -o $@ $(BUILD_FLAGS) $(INCLUDES)

# Criar diretório de build se necessário
buildFolder:
	@printf "$(YELLOW)[BUILD]$(NC) Criando diretório de build\n"
	@mkdir -p $(BUILD_DIR)

# Limpeza dos arquivos gerados
clean:
	@printf "$(RED)[CLEAN]$(NC) Removendo arquivos gerados\n"
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(PROJECT_NAME)

# Habilita paralelização automática com -j$(nproc)
MAKEFLAGS += -j$(shell nproc)
