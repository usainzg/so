# so
Repo para la asignatura de Sistemas Operativos (UPV/EHU).

# Uso de la aplicación:
- -c \<frecuencia en MHz> para elegir frecuencia de reloj del sistema.
- -t \<periodo en ms> para elegir el periodo de tiempo del timer.
- -p \<...> para elegir la frecuencia variable del process generator.
- -C \<n_cpus_per_system> para elegir el número de CPUs del sistema.
- -O \<n_cores_per_cpu> para elegir el número de cores de las CPU.
- -T \<n_threads_per_core> para elegir el número de threads por cada core.

En caso de no especificar alguno de los parámetros mediante las opciones listadas, se aplicarán los datos por defecto en los parámetros no seteados.

# Datos por defecto del sistema:
- Frecuencia del reloj del sistema (clock): 100 MHz
- Periodo de tiempo del timer: 1 ms
- Frecuencia del process generator:  
- Número de CPUs: 1 CPU
- Número de cores por cada CPU: 1 core.
- Número de threads por cada core: 1 thread.
