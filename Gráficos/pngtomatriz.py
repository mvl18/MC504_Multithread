from PIL import Image

img = Image.open("papainoel.png")
img = img.convert("RGB")      # Garante formato RGB
img = img.resize((16, 16))    # Redimensiona (ou use sua própria)

# Paleta simplificada: dicionário de cores indexadas
color_palette = {}
color_index = 1

matrix = []

for y in range(img.height):
    row = []
    for x in range(img.width):
        rgb = img.getpixel((x, y))
        if rgb not in color_palette:
            color_palette[rgb] = color_index
            color_index += 1
        row.append(color_palette[rgb])
    matrix.append(row)

# Exibe a matriz
for row in matrix:
    print(row)

# Mostra legenda das cores
print("\nLegenda de cores:")
for rgb, idx in color_palette.items():
    print(f"{idx} = {rgb}")
