
function param(parametro)
   
   if parametro == "nulo" then
      return 0
   elseif parametro == "baja" then
      return 25
   elseif parametro == "media" then
      return 50
   elseif parametro == "alta" then
      return 75
   else 
      return 0
   end
end

-- Ciclo del robot

function ciclo(orden, parametro)

   if orden == "avanzar" then

      robot.avanzar(param(parametro))

   elseif orden == "girar" then

      robot.girar_derecha(param(parametro))

   elseif orden == "girar_negativo" then

      robot.girar_izquierda(param(parametro))

   elseif orden == "parar" then

      robot.parar()

   else
      return "Orden no reconocida"
   end

   return "Recibido " .. orden .. ", " .. parametro .. "."

end
