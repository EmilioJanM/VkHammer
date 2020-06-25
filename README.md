# VkHammer

1.- Descargar la última versión del sdk de Vulkan e instalarlo:  https://vulkan.lunarg.com/sdk/home#windows
 
2.-Abrir el .sln (proyecto) de VkHammer con Visual Studio

3.-En la ventana de propiedades modificar los directorios de inclusión(ref 1) para que quede con el directorio de VkHammer-master/Include

4.-Hacer el paso anterior pero ahora con los directorios de bibliotecas adicionales (ref 3)  para que quede con el directorio de VkHammer-master/libraries
image.png


Estando en el proyecto en VkHammer.cpp (VkHammer/VkHammerClass) a partir de la línea 185 se cargan los modelos

Los comportamientos de los planetas se definen en la clase ModelLoader.h (Geometry/ModelLoader)

Jueguen, no se olviden de poner el proyecto en release de x64, no debug
