function filtrar()
   local alto, ancho, bytes, valor, punto
   ancho = 100
   bytes = 3
   alto = 100
   for j = 0, (ancho * bytes) - 1 do
      for i = 0, alto - 1 do
	 punto = ancho * bytes * j + i
	 valor = imagen.get(captura, punto)
	 imagen.set(tipo_orden, punto, valor)
      end
   end
end

