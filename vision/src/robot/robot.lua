
-- Ciclo del robot

function ciclo(orden, parametro)

   if orden == "avanzar" then

      robot.avanzar(parametro)

   elseif orden == "girar" then

      robot.girar_derecha(parametro)

   elseif orden == "girar_negativo" then

      robot.girar_izquierda(parametro)

   elseif orden == "parar" then

      robot.parar(parametro)

   else
      return "Orden no reconocida"
   end

   return "Recibido " .. orden .. ", " .. parametro .. "."

end
