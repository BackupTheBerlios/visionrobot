function iniciar(_imagen,_ancho, _alto, _bytes,
		 _salida,_anchos, _altos, _bytess)
   captura = imagen.new (_imagen, ancho, alto, bytes)
   salida = imagen.new (_salida, anchos, altos, bytess)
   ancho = _ancho
   alto = _alto
   bytes = _bytes
   io.write ("Se inicia el filtro con " .. ancho .. "x" .. alto .. "x" ..bytes ..".\n")
end

function filtrar()
   local punto, valor
   for j = 0, alto - 1 do
      for i = 0, (ancho * bytes) - 1 do
	 punto = ancho * bytes * j + i
	 valor = imagen.get(captura, punto)
	 imagen.set(salida, punto, 0)
      end
   end
end

function parametros(oir, osr, oiv, osv, oia, osa)
   io.write("Parametros que le llegan al filtro: [" .. oir ..", "
	    .. osr .. ", " .. oiv .. ", " .. osv .. ", " .. oia ..
	       ", " ..osa .. "]\n")
end


