-- Funcion de filtrado de los guantes
function filtrar(captura, salida, param)
   local cont, ac_x, ac_y = imagen.difuminar(captura, salida, 4, 20, param)   
   local valido = cont > 0
   if valido then
      imagen.centrar(salida, cont, ac_x, ac_y)
   end
   return valido, nil, salida
end

-- Restricciones:
--   - No sabe leer carteles en perspectiva. Asi que el cartel ha de estar paralelo a la camara.
--   - La distancia idonea esta entre los 30 y los 50 centimetros.
--   - La luz es mejor que sea difusa.
function filtrar_letras(captura, salida, param)
   local imagen_devuelta1 = imagen.crear_copia(captura)
   local valido = imagen.centrar2(imagen_devuelta1, param)
   local imagen_devuelta2

   if valido then
      local bounds = imagen.buscar_limites(imagen_devuelta1,1, param)
      imagen_devuelta2 = imagen.rotar(imagen_devuelta1, bounds)
      imagen.borrar_bounds(bounds);
      bounds = imagen.buscar_limites(imagen_devuelta2,1, param)
      imagen.clean(imagen_devuelta2, bounds)
      imagen.borrar_bounds(bounds);
      imagen.make_up(imagen_devuelta2)
      imagen.centrar2(imagen_devuelta2, nil)
   end

   return valido, nil, imagen_devuelta2
end
