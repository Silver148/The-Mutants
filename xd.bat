@echo off
setlocal

:: Nombre de la carpeta a agregar
set "PATH=C:\w64devkit\bin"

:: 1. Comprueba si se está ejecutando como administrador (método rudimentario)
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo ❌ **Error: Permisos Insuficientes.**
    echo Este script necesita ser ejecutado como **Administrador** para modificar las variables de entorno del sistema.
    echo.
    echo Haga clic derecho en el archivo .bat y seleccione **"Ejecutar como administrador"**.
    echo.
    pause
    goto :eof
)

echo.
echo ✅ **Iniciando Modificación del Path del Sistema...**

:: 2. Comando principal: Utiliza WMIC para modificar la variable Path del sistema
:: El comando revisa si la ruta ya existe antes de añadirla para evitar duplicados.
:: Si no existe, antepone la nueva ruta al Path existente.
:: Nota: WMIC ha sido marcado como obsoleto por Microsoft, pero es el método
:: de línea de comandos más directo para esta tarea en muchos sistemas.

:: Obtener el Path actual del sistema
for /f "skip=1 tokens=*" %%a in ('wmic ENVIRONMENT where "name='Path' and username='<system>'" get VariableValue /value') do (
    set "CURRENT_PATH=%%a"
    goto :check_path
)

:check_path
:: Limpiar el valor obtenido de "VariableValue="
set "CURRENT_PATH=%CURRENT_PATH:VariableValue=%"

:: 3. Verificar si la ruta ya existe
echo.
echo Ruta actual: %CURRENT_PATH%
echo.

echo Buscando si %NEW_PATH% ya está en el Path...
echo.

:: Se usa findstr para buscar la cadena (ignorando mayúsculas/minúsculas)
echo "%CURRENT_PATH%" | findstr /i "%NEW_PATH%" >nul 2>&1
if %errorlevel% equ 0 (
    echo **Ruta ya existente.** No se requiere ninguna acción.
) else (
    echo **Ruta no encontrada.** Agregando %NEW_PATH% al principio del Path...

    :: Comando real de modificación
    wmic ENVIRONMENT where "name='Path' and username='<system>'" set VariableValue="%NEW_PATH%;%CURRENT_PATH%"

    if %errorlevel% equ 0 (
        echo.
        echo **¡Éxito!** La ruta "%NEW_PATH%" ha sido agregada a la variable **Path del Sistema**.
        echo.
        echo **NOTA IMPORTANTE:** Para que el cambio surta efecto en todas las aplicaciones,
        echo es posible que deba **reiniciar** las aplicaciones o el sistema.
    ) else (
        echo.
        echo ❌ **Error:** No se pudo modificar la variable Path.
        echo Por favor, verifique si la ejecución como Administrador fue correcta.
    )
)

echo.
echo **Proceso Terminado.**
endlocal
pause