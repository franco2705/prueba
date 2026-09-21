# Cube 3D Base (Nintendo Switch)

Una base pequeña y autocontenida para empezar proyectos homebrew de Nintendo
Switch. Dibuja un cubo 3D giratorio mediante un framebuffer de `libnx`; no
depende de un motor ni de una biblioteca gráfica adicional.

## Requisitos

* `devkitPro`, `devkitA64` y `libnx` instalados.
* Las variables de entorno de devkitPro disponibles en la terminal (en
  particular, `DEVKITPRO`).

## Compilar y ejecutar

```sh
make
```

El resultado es `cube3d.nro`. Copialo a, por ejemplo,
`sdmc:/switch/cube3d/cube3d.nro` y ábrelo desde el Homebrew Menu.

```sh
make clean
```

Elimina los artefactos generados.

## Controles

| Botón | Acción |
| --- | --- |
| Stick izquierdo / cruceta | Girar el cubo manualmente |
| `A` | Reiniciar la orientación |
| `+` | Salir |

## Próximos pasos

`source/main.c` concentra el ciclo de juego, la entrada, la proyección y el
renderizado. Es un punto directo para reemplazar el cubo por entidades,
añadir una cámara, cargar modelos o migrar el renderizador a deko3d cuando el
proyecto necesite aceleración por GPU.
