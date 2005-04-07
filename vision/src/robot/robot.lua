--
-- robot.lua
--
-- Guión de control del puerto paralelo para la interacción
-- con el robot
--
-- Autor: Carlos León
--

-- Definición de los pines del puerto paralelo
REFERENCIA = 4
AVANZAR = 5
RETROCEDER = 6
GIRAR_IZQUIERDA = 7
GIRAR_DERECHA = 8
      
-- Pone a cero todos los movimientos
function reposo()
   robot.alta(REFERENCIA)
   robot.baja(AVANZAR)
   robot.baja(RETROCEDER)
   robot.baja(GIRAR_IZQUIERDA)
   robot.baja(GIRAR_DERECHA)
end

-- Parar el giro
function parar_giro()
   robot.baja(GIRAR_DERECHA)
   robot.baja(GIRAR_IZQUIERDA)
end

function establecer_argumento_giro(argumento)
   local ms
   if argumento == "baja" then
      ms = 300
   elseif argumento == "media" then
      ms = 600
   else 
      ms = 900
   end
   robot.timer(ms, "parar_giro")
end

-- Giro a la izquierda
function girar_izquierda(argumento)
   if argumento ~= "nula" then
      robot.baja(GIRAR_DERECHA)
      robot.alta(GIRAR_IZQUIERDA)
      establecer_argumento_giro(argumento)
   end
end

-- Giro a la derecha
function girar_derecha(argumento)
   if argumento ~= "nula" then
      robot.baja(GIRAR_IZQUIERDA)
      robot.alta(GIRAR_DERECHA)
      establecer_argumento_giro(argumento)
   end
end

-- Avance
function avanzar(argumento)
    if argumento == "nula" or argumento == "baja" then
       robot.baja(AVANZAR)
       robot.alta(RETROCEDER)
    else 
       robot.baja(RETROCEDER)
       robot.alta(AVANZAR)
    end
end

-- Ciclo del robot
function ciclo(orden, parametro)
   
   if orden == "avanzar" then
      avanzar(parametro)
   elseif orden == "girar" then
      girar_izquierda(parametro)
   elseif orden == "girar_negativo" then
      girar_derecha(parametro)
   elseif orden == "parar" then
      reposo()
   else
      reposo()
      return "Orden no reconocida"
   end

   return "Recibido " .. orden .. ", " .. parametro .. "."

end

-- Bajamos los valores del puerto paralelo
function fin() 
   robot.baja(REFERENCIA)
   robot.baja(AVANZAR)
   robot.baja(RETROCEDER)
   robot.baja(GIRAR_IZQUIERDA)
   robot.baja(GIRAR_DERECHA)
end

-- Configura la salida del puerto paralelo
function configurar()
   robot.salida(REFERENCIA)
   robot.salida(AVANZAR)
   robot.salida(RETROCEDER)
   robot.salida(GIRAR_IZQUIERDA)
   robot.salida(GIRAR_DERECHA)
end

function iniciar() 
   -- Establecemos los pines de salida
   configurar()

   -- Configuramos el estado de reposo
   reposo()
end

