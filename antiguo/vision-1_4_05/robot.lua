function ciclo(r, orden)
   if orden == "avanzar" then
      robot.avanzar(r, 10)
   elseif orden == "parar" then
      robot.avanzar(r, 0)      
   else
      return "Orden no reconocida"
   end
   return "Recibido " .. orden .. "."
end
