print ("Filtrando imagen de " .. filtro.ancho .. "x" .. filtro.alto .. "x" .. filtro.bytes .. ".")

for i = 0, 100, 1 do
   io.write(filtro.imagen[i] .. ".")
end
io.write("\n")
