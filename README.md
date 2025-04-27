# Repositório para o Trabalho da Disciplina de Sistemas Operacionais (MC504)

## Projeto Multithread - O Problema do Papai Noel

Escolhemos o problema "The Santa Claus Problem" conforme descrito na página 137 do [livro](https://greenteapress.com/wp/semaphores/).

## Descrição do Projeto

Este projeto implementa um simulador do problema do Papai Noel, utilizando múltiplas threads para modelar as interações e eventos envolvidos no cenário. O objetivo é estudar o comportamento de sistemas concorrentes e a sincronização entre threads em um ambiente multithreaded, utilizando semáforos e mutexes para controlar o acesso a recursos criticos.

## Compilação

Para compilar o projeto, você precisará ter instalado as bibliotecas SDL2, SDL2_images, SDL2_ttf e pthread. As instruções específicas para instalação variam dependendo do seu sistema operacional:

- **Fedora**:
  ```bash
  sudo dnf install SDL2 SDL2-devel SDL2-image SDL2-image-devel SDL2-ttf SDL2-ttf-devel
  ```

- **Arch Linux**:
  ```bash
  sudo pacman -S sdl2 sdl2_image sdl2_ttf libpthread
  ```

- **Debian/Ubuntu**:
  ```bash
  sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libpthread-dev
  ```

Depois de instalar as bibliotecas necessárias, execute o seguinte comando para compilar o projeto:
```bash
make
```

## Execução

Para executar o projeto, utilize o seguinte comando:
```bash
./build/santa
```

## Estrutura do Repositório

- **Makefile**: Script para compilar o código-fonte.
- **src/**: Diretório contendo todos os arquivos de origem do projeto.
- **lib/**: Diretório contendo cabeçalhos necessários para a compilação e execução do projeto.
- **assets/**: Diretório contendo imagens e outros recursos utilizados no projeto.

## Autores

- [Giancarlo Bonvenuto](https://github.com/gBonvenuto)
- [José Victor](https://github.com/GoliasVictor)
- [Julio Avelar](https://github.com/JN513)
- [Matheus Veiga](https://github.com/mvl18)
