#include "pch.h"
#include "GLTexture.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"

GLTexture::GLTexture()
{
    id = 0;
}


GLTexture::~GLTexture()
{
    this->Release();
}

void GLTexture::PrepareTexture(bool lightEnabled)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    if (lightEnabled)
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
}

void GLTexture::LoadTexture(CString fileName)
{
    if (id)
    {
        this->Release();
    }

    // Alokacija
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Parametri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Ucitavanje
    DImage* img = new DImage();
    img->Load(fileName);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->Width(), img->Height(), GL_RGBA, GL_UNSIGNED_BYTE, img->GetDIBBits());

    delete img;
}

void GLTexture::Select()
{
    if (id)
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }
}

void GLTexture::Release()
{
    if (id)
    {
        glDeleteTextures(1, &id);
        id = 0;
    }
}