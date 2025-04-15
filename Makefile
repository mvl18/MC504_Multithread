# Ferramentas

PROJECT_NAME = santa

CC = gcc

BUILD_FLAGS = -O2 -lncurses -Wall

BUILD_DIR = build
SRC_DIR = src

C_SOURCES = main.c

.PHONY: all clean buildFolder

all: buildFolder $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC_DIR)/main.c
	$(CC) $< -o $(BUILD_DIR)/$@ $(BUILD_FLAGS)


# Criar diretório de build se necessário
buildFolder:
	@mkdir -p $(BUILD_DIR)

# Limpeza dos arquivos gerados
clean:
	@printf "$(RED)[CLEAN]$(NC) Removendo arquivos gerados\n"
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.hex $(BUILD_DIR)/*.s

# Habilita paralelização automática com -j$(nproc)
MAKEFLAGS += -j$(shell nproc)
