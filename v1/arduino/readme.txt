Traje v1

SW de arduino para la primera versión del traje que incluye:
* 2 acc - de 2 ejes
* 4 switches
* 1 IR

Con este traje comenzón la primera versión de synbody.

main_control
serie de programas que incluyen escritura MIDI interpretando los valores de los sensores
En este tipo de aplicaciones Arduino realiza todo el control: Sensor Control y MIDI Control.

read_sensors_and_print
serie de programas que simplemente leen los valores de los sensores y lo escriben por el puerto serie. No interpretan estos valores. Eso se deja para la aplicacion en Processing.
En este tipo de aplicaciones Arduino solamente realiza la parte de Sensor Control y es la aplicacion en Processing "sensor2midi" la que implementa el MIDI Control System.

*La mejor version es siempre la ultima

