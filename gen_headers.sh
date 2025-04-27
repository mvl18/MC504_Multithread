#!/bin/bash

# Diretórios
IMGS_DIR="assets/imgs"
FONTS_DIR="assets/fonts"
OUTPUT_DIR="assets"

# Garante que o diretório de saída existe
mkdir -p "$OUTPUT_DIR"

# Função para processar arquivo
process_file() {
    local input_file="$1"
    local output_file="$2"
    
    echo "Gerando $output_file a partir de $input_file..."
    xxd -i "$input_file" | sed 's/assets_imgs_//g' | sed 's/assets_fonts_//g' | tr -d '\n' > "$output_file"
}

# Processa PNGs
for img in "$IMGS_DIR"/*.png; do
    [ -e "$img" ] || continue
    filename=$(basename "$img" .png)
    output="$OUTPUT_DIR/${filename}_png.h"
    process_file "$img" "$output"
done

# Processa TTFs
for font in "$FONTS_DIR"/*.ttf; do
    [ -e "$font" ] || continue
    filename=$(basename "$font" .ttf)
    output="$OUTPUT_DIR/${filename}_ttf.h"
    process_file "$font" "$output"
done

echo "Todos os headers foram gerados com sucesso!"
