
-- Funcion de inicio
function iniciar(_imagen,_ancho, _alto, _bytes,
		 _salida,_anchos, _altos, _bytess)
   captura = imagen.new (_imagen, _ancho, _alto, _bytes)
   salida = imagen.new (_salida, _anchos, _altos, _bytess)
   ancho = _ancho
   alto = _alto
   bytes = _bytes
   io.write ("Se inicia el filtro con " .. ancho .. "x" .. alto .. "x" ..bytes ..".\n")
end

-- Difumina un punto
function difuminar(x, y, param_difuminado)
   local param1 = param_difuminado * bytes
   local param2 = param1 + (bytes - 1)
   local rojo = 0
   local verde = 0
   local azul = 0
   local y, x
   if y - param_difuminado >= 0 and
      y + param_difuminado <= alto then
      for i = y - param_difuminado, y + param_difuminado do
	 if x - param1 >= 0 and
	    x + param2 <= ancho * bytes then
	    for j = x - param1, x + param2, bytes do
	       local punto = ancho * bytes * i + j
	       rojo = rojo + captura[punto]
	       verde = verde + captura[punto + 1]
	       azul = azul + captura[punto + 2]
	    end
	 end
      end
   end
   return rojo, verde, azul, param_difuminado
end

-- Aplana un color
function aplanar(rojo, verde, azul, param_difuminado)
   local numero = 2 * param_difuminado + 1
   numero = numero * numero
   rojo = math.floor(rojo / numero)
   verde = math.floor(verde / numero)
   azul = math.floor(azul / numero)
   return rojo, verde, azul
end

-- Hace una reduccion a un color
function resta(color, reduccion)
   while (math.mod(color, reduccion)) > 0 and color > 1 do
      color = color - 1
   end
   return color
end

-- Reduce un punto
function reducir(rojo, verde, azul, reduccion)
   return resta(rojo, reduccion), resta(verde, reduccion), resta(azul, reduccion)
end

-- Genera en la salida blanco o negro en funcion
-- de la imagen filtrada y de los colores de los parametros
function clasificar(rojo, verde, azul, x, y)
   -- Diferentes tipos de colores
   rojo, azul = azul, rojo
   local pos = y * ancho * bytes + x
   local valor, dev
   if rojo >= ir and rojo <= sr and
      verde >= iv and verde <= sv and
      azul >= ia and azul <= sa then
      valor = 255
      dev = 1
   else
      valor = 0 
      dev = 0
   end
   salida[pos] = valor
   salida[pos + 1] = valor
   salida[pos + 2] = valor
   return dev
end

-- Funcion de filtrado
function filtrar()
   local cont
   for y = 0, alto - 1 do
      for x = 0, (ancho * bytes) - 1, bytes do
	 cont = cont + clasificar(reducir(aplanar(difuminar(x, y, 2)), 10), x, y)	 
      end
   end
   return cont > 0
end

-- Funcion de cambio de parametros
function parametros(_ir, _sr, _iv, _sv, _ia, _sa)
   io.write("Parametros que le llegan al filtro: [" .. ir ..", "
	    .. sr .. ", " .. iv .. ", " .. sv .. ", " .. ia ..
	       ", " ..sa .. "]\n")
   ir = _ir
   sr = _sr
   iv = _iv
   sv = _sv
   ia = _ia
   sa = _sa   
end


