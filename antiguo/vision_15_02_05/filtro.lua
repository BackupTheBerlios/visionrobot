
-- Funcion de inicio
function iniciar(_imagen,_ancho, _alto, _bytes,
		 _salida,_anchos, _altos, _bytess)
   captura = imagen.new (_imagen, _ancho, _alto, _bytes)
   salida = imagen.new (_salida, _anchos, _altos, _bytess)
   ancho = _ancho
   alto = _alto
   bytes = _bytes
   return "iniciado con " .. ancho .. "x" .. alto .. "x" ..bytes
end

-- Funcion de filtrado
function filtrar()
   local cont, ac_x, ac_y = captura:difuminar(salida, 2, 10, ir, sr, iv, sv, ia, sa)   
   salida:centrar(cont, ac_x, ac_y)
   return cont > 0, 0, salida
end

-- Funcion de cambio de parametros
function parametros(_sr, _ir, _sv, _iv, _sa, _ia)
   ir = _ir
   sr = _sr
   iv = _iv
   sv = _sv
   ia = _ia
   sa = _sa   
   return "colores: [" .. ir ..", "
      .. sr .. ", " .. iv .. ", " .. sv .. ", " .. ia ..
      ", " .. sa .. "]"   
end


function cerrar()
   imagen.delete(salida)
end
