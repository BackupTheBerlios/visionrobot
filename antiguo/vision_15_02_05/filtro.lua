function iniciar(_imagen, _tipo_orden, _orden_param, _ancho, _alto, _bytes) 
   captura = imagen.new (_imagen, ancho, alto, bytes)
   tipo_orden = imagen.new (_tipo_orden, ancho, alto, bytes)
   orden_param = imagen.new (_orden_param, ancho, alto, bytes)
   ancho = _ancho
   alto = _alto
   bytes = _bytes
end

function filtrar(oir, osr, oiv, osv, oia, osa,
		 pir, psr, piv, psv, pia, psa)
   local azul, verde, rojo, punto
   for j = 0, alto - 1 do
      for i = 0, (ancho * bytes) - 1, bytes do
	 punto = ancho * bytes * j + i
	 rojo = imagen.get(captura, punto)
	 verde = imagen.get(captura, punto + 1)
	 azul = imagen.get(captura, punto + 2)
	 if rojo >= oir and rojo <= osr and
	    verd >= oiv and verde <= osv and
	    azul >= oia and azul <= osa then
	    imagen.set(tipo_orden, punto, 255)
	    imagen.set(tipo_orden, punto + 1, 255)
	    imagen.set(tipo_orden, punto + 2, 255)
	 else 
	    imagen.set(tipo_orden, punto, 0)
	    imagen.set(tipo_orden, punto + 1, 0)
	    imagen.set(tipo_orden, punto + 2, 0)
	 end
      end
   end
end

