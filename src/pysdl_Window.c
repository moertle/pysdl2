#include "pysdl.h"

static int        PySDL_Window_Type_init    (PySDL_Window*, PyObject*, PyObject*);
static void       PySDL_Window_Type_dealloc (PySDL_Window* );

static PyObject * PySDL_Window_GetWindowID         (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GetWindowTitle      (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GetWindowPosition   (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GetWindowSize       (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GetWindowSurface    (PySDL_Window*, PyObject*);

static PyObject * PySDL_Window_SetWindowFullscreen (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_SetWindowIcon       (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_SetWindowTitle      (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_SetWindowPosition   (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_SetWindowSize       (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_SetWindowResizable  (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_SetWindowBrightness (PySDL_Window*, PyObject*);

static PyObject * PySDL_Window_ShowWindow          (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_HideWindow          (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_RaiseWindow         (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_MaximizeWindow      (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_MinimizeWindow      (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_RestoreWindow       (PySDL_Window*, PyObject*);

static PyObject * PySDL_Window_UpdateWindowSurface (PySDL_Window*, PyObject*);

static PyObject * PySDL_Window_CreateRenderer      (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GL_CreateContext    (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GL_DeleteContext    (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GL_MakeCurrent      (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GL_SwapWindow       (PySDL_Window*, PyObject*);
static PyObject * PySDL_Window_GL_GetDrawableSize  (PySDL_Window*, PyObject*);

static PyMethodDef PySDL_Window_methods[] = {
    { "GetWindowID",         (PyCFunction)PySDL_Window_GetWindowID,         METH_NOARGS  },
    { "GetWindowTitle",      (PyCFunction)PySDL_Window_GetWindowTitle,      METH_NOARGS  },
    { "GetWindowPosition",   (PyCFunction)PySDL_Window_GetWindowPosition,   METH_NOARGS  },
    { "GetWindowSize",       (PyCFunction)PySDL_Window_GetWindowSize,       METH_NOARGS  },
    { "GetWindowSurface",    (PyCFunction)PySDL_Window_GetWindowSurface,    METH_NOARGS  },

    { "SetWindowFullscreen", (PyCFunction)PySDL_Window_SetWindowFullscreen, METH_VARARGS },
    { "SetWindowIcon",       (PyCFunction)PySDL_Window_SetWindowIcon,       METH_O       },
    { "SetWindowTitle",      (PyCFunction)PySDL_Window_SetWindowTitle,      METH_O       },
    { "SetWindowPosition",   (PyCFunction)PySDL_Window_SetWindowPosition,   METH_VARARGS },
    { "SetWindowSize",       (PyCFunction)PySDL_Window_SetWindowSize,       METH_VARARGS },
    { "SetWindowResizable",  (PyCFunction)PySDL_Window_SetWindowResizable,  METH_O       },
    { "SetWindowBrightness", (PyCFunction)PySDL_Window_SetWindowBrightness, METH_O       },

    { "ShowWindow",          (PyCFunction)PySDL_Window_ShowWindow,          METH_NOARGS  },
    { "HideWindow",          (PyCFunction)PySDL_Window_HideWindow,          METH_NOARGS  },
    { "RaiseWindow",         (PyCFunction)PySDL_Window_RaiseWindow,         METH_NOARGS  },
    { "MaximizeWindow",      (PyCFunction)PySDL_Window_MaximizeWindow,      METH_NOARGS  },
    { "MinimizeWindow",      (PyCFunction)PySDL_Window_MinimizeWindow,      METH_NOARGS  },
    { "RestoreWindow",       (PyCFunction)PySDL_Window_RestoreWindow,       METH_NOARGS  },

    { "UpdateWindowSurface", (PyCFunction)PySDL_Window_UpdateWindowSurface, METH_NOARGS  },

    { "CreateRenderer",      (PyCFunction)PySDL_Window_CreateRenderer,      METH_NOARGS  },
    { "GL_CreateContext",    (PyCFunction)PySDL_Window_GL_CreateContext,    METH_NOARGS  },
    { "GL_DeleteContext",    (PyCFunction)PySDL_Window_GL_DeleteContext,    METH_NOARGS  },
    { "GL_MakeCurrent",      (PyCFunction)PySDL_Window_GL_MakeCurrent,      METH_NOARGS  },
    { "GL_SwapWindow",       (PyCFunction)PySDL_Window_GL_SwapWindow,       METH_NOARGS  },
    { "GL_GetDrawableSize",  (PyCFunction)PySDL_Window_GL_GetDrawableSize,  METH_NOARGS  },
    { NULL }
};

PyTypeObject PySDL_Window_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "SDL2.Window",
    .tp_basicsize = sizeof(PySDL_Window),
    .tp_dealloc   = (destructor)PySDL_Window_Type_dealloc,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc       = "SDL2.Window(title,(width,height),(x,y),flags)",
    .tp_methods   = PySDL_Window_methods,
    .tp_init      = (initproc)PySDL_Window_Type_init,
    .tp_new       = PyType_GenericNew,
};

static int PySDL_Window_Type_init(PySDL_Window *self, PyObject *args, PyObject *kwds) {
    char *title = NULL;
    int w = 0;
    int h = 0;
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int f = 0;
    int ok;

    static char *kwlist[] = {"title", "size", "position", "flags", NULL};

    ok = PyArg_ParseTupleAndKeywords(args, kwds, "s|(ii)(ii)I", kwlist,
        &title, &w, &h, &x, &y, &f);
    if(!ok) {
        return -1;
    }

    self->window = SDL_CreateWindow(title, x, y, w, h, f);
    if(NULL == self->window) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return -1;
    }

    self->glContext = NULL;

    return 0;
}

static void PySDL_Window_Type_dealloc(PySDL_Window *self) {
    if(NULL != self->glContext) {
        SDL_GL_DeleteContext(self->glContext);
        self->glContext = NULL;
    }
    if(NULL != self->window) {
        SDL_DestroyWindow(self->window);
        self->window = NULL;
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * PySDL_Window_GetWindowID(PySDL_Window *self, PyObject *ign) {
    Uint32 id = SDL_GetWindowID(self->window);
    if(0 == id) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }
    return PyLong_FromUnsignedLong(id);
}

static PyObject * PySDL_Window_GetWindowTitle(PySDL_Window *self, PyObject *ign) {
    return PyUnicode_FromString(SDL_GetWindowTitle(self->window));
}

static PyObject * PySDL_Window_GetWindowPosition(PySDL_Window *self, PyObject *ign) {
    int x, y;
    SDL_GetWindowPosition(self->window, &x, &y);
    return Py_BuildValue("(ii)", x, y);
}

static PyObject * PySDL_Window_GetWindowSize(PySDL_Window *self, PyObject *ign) {
    int w, h;
    SDL_GetWindowSize(self->window, &w, &h);
    return Py_BuildValue("(ii)", w, h);
}

static PyObject * PySDL_Window_GetWindowSurface(PySDL_Window *self, PyObject *ign) {
    PySDL_Surface *pysdl_Surface;

    pysdl_Surface = (PySDL_Surface *)PyObject_CallObject((PyObject *)&PySDL_Surface_Type, NULL);
    if(NULL == pysdl_Surface) {
        PyErr_SetString(PyExc_TypeError, "Could not create SDL2.Surface object");
        return NULL;
    }

    pysdl_Surface->shouldFree = 0;

    pysdl_Surface->surface = SDL_GetWindowSurface(self->window);
    if(NULL == pysdl_Surface->surface) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }

    return (PyObject *)pysdl_Surface;
}

static PyObject * PySDL_Window_SetWindowFullscreen(PySDL_Window *self, PyObject *args) {
    int flags = 0;
    int ok;

    ok = PyArg_ParseTuple(args, "|i", &flags);

    ok = SDL_SetWindowFullscreen(self->window, flags);
    if(0 > ok) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_SetWindowIcon(PySDL_Window *self, PyObject *args) {
    PySDL_Surface *pysdl_Surface = (PySDL_Surface *)args;
    SDL_SetWindowIcon(self->window, pysdl_Surface->surface);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_SetWindowTitle(PySDL_Window *self, PyObject *args) {
    SDL_SetWindowTitle(self->window, PyUnicode_AsUTF8(args));
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_SetWindowPosition(PySDL_Window *self, PyObject *args) {
    int x, y;
    int ok;

    ok = PyArg_ParseTuple(args, "(ii)", &x, &y);
    if(0 > ok) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }

    SDL_SetWindowPosition(self->window, x, y);

    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_SetWindowSize(PySDL_Window *self, PyObject *args) {
    int w, h;
    int ok;

    ok = PyArg_ParseTuple(args, "(ii)", &w, &h);
    if(0 > ok) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }

    SDL_SetWindowSize(self->window, w, h);

    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_SetWindowResizable(PySDL_Window *self, PyObject *args) {
    SDL_SetWindowResizable(self->window, args == Py_True);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_SetWindowBrightness(PySDL_Window *self, PyObject *args) {
    SDL_SetWindowBrightness(self->window, PyFloat_AsDouble(args));
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_ShowWindow(PySDL_Window *self, PyObject *ign) {
    SDL_ShowWindow(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_HideWindow(PySDL_Window *self, PyObject *ign) {
    SDL_HideWindow(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_RaiseWindow(PySDL_Window *self, PyObject *ign) {
    SDL_RaiseWindow(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_MaximizeWindow(PySDL_Window *self, PyObject *ign) {
    SDL_MaximizeWindow(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_MinimizeWindow(PySDL_Window *self, PyObject *ign) {
    SDL_MinimizeWindow(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_RestoreWindow(PySDL_Window *self, PyObject *ign) {
    SDL_RestoreWindow(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_UpdateWindowSurface(PySDL_Window *self, PyObject *ign) {
    SDL_UpdateWindowSurface(self->window);
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_CreateRenderer(PySDL_Window *self, PyObject *ign) {
    PySDL_Renderer *pysdl_Renderer;

    int index = -1;
    int flags = 0;

    flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    pysdl_Renderer = (PySDL_Renderer *)PyObject_CallObject((PyObject *)&PySDL_Renderer_Type, NULL);
    if(NULL == pysdl_Renderer) {
        PyErr_SetString(PyExc_TypeError, "Could not create SDL2.Renderer object");
        return NULL;
    }

    pysdl_Renderer->renderer = SDL_CreateRenderer(self->window, index, flags);
    if(NULL == pysdl_Renderer->renderer) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }

    return (PyObject *)pysdl_Renderer;
}

static PyObject * PySDL_Window_GL_CreateContext(PySDL_Window *self, PyObject *ign) {
    self->glContext = SDL_GL_CreateContext(self->window);
    if(NULL == self->glContext) {
        PyErr_SetString(pysdl_Error, SDL_GetError());
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_GL_DeleteContext(PySDL_Window *self, PyObject *ign) {
    if(NULL != self->glContext) {
        SDL_GL_DeleteContext(self->glContext);
        self->glContext = NULL;
    }
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_GL_MakeCurrent(PySDL_Window *self, PyObject *ign) {
    if(NULL != self->glContext) {
        SDL_GL_MakeCurrent(self->window, self->glContext);
    }
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_GL_SwapWindow(PySDL_Window *self, PyObject *ign) {
    Py_BEGIN_ALLOW_THREADS
        SDL_GL_SwapWindow(self->window);
    Py_END_ALLOW_THREADS
    Py_RETURN_NONE;
}

static PyObject * PySDL_Window_GL_GetDrawableSize(PySDL_Window *self, PyObject *ign) {
    int w, h;
    SDL_GL_GetDrawableSize(self->window, &w, &h);
    return Py_BuildValue("(ii)", w, h);
}
