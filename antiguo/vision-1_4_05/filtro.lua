-- Funcion de filtrado
function filtrar(captura, salida, param)
   local cont, ac_x, ac_y = imagen.difuminar(captura, salida, 2, 10, param)   
   local valido = cont > 0
   if valido then
      imagen.centrar(salida, cont, ac_x, ac_y)
   end
   return valido, nil
end

