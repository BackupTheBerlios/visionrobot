-- Funcion de filtrado de los guantes
function filtrar(captura, salida, param)
   local cont, ac_x, ac_y = imagen.difuminar(captura, salida, 4, 20, param)   
   local valido = cont > 0
   if valido then
      imagen.centrar(salida, cont, ac_x, ac_y)
   end
   return valido, nil
end

-- Restricciones:
--   - No sabe leer carteles en perspectiva. Asi que el cartel ha de estar paralelo a la camara.
--   - La distancia idonea esta entre los 30 y los 50 centimetros.
--   - La luz es mejor que sea difusa.
function filtrar_letras(captura, salida, param)
--   imagen.copiar(captura, salida)
--   imagen.centrar2(salida, param)
--   local bounds = imagen.buscar_limites(salida, 1, param)
--   imagen.rotar(salida, bounds)
--   imagen.clean(salida, bounds)
--   imagen.make_up(salida)
   return true, nil
end
