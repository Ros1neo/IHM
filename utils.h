#ifndef UTILS_H
#define UTILS_H

static int TP_ID = 3;

// display events
#define DISPLAY_MOUSE_EVENTS false
#define DISPLAY_KEYBOARD_EVENTS false

// shader path
#define VERTEX_SHADER   "../IOGS_RV/simple.vert"
#define FRAGMENT_SHADER "../IOGS_RV/simple.frag"

// object path
#define OBJECT_BOX "../IOGS_RV/data/box.obj"
#define OBJECT_FRAME "../IOGS_RV/data/frame.off"

// texture path
#define TEXTURE_FRAME "../IOGS_RV/data/texture.png"
#define TEXTURE_WOOD "../IOGS_RV/data/wood.jpg"


enum RenderType { RENDERTYPE_NORMAL, RENDERTYPE_TEXTURE, RENDERTYPE_COLOR, RENDERTYPE_ID };

#endif // UTILS_H
