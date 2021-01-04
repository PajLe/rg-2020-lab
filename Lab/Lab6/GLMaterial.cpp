#include "pch.h"
#include "GLMaterial.h"
#include <GL/GL.h>

GLMaterial::GLMaterial()
{
	m_vAmbient[0] = 0.2;  m_vAmbient[1] = 0.2;
	m_vAmbient[2] = 0.2;  m_vAmbient[3] = 1.0;

	m_vDiffuse[0] = 0.8;  m_vDiffuse[1] = 0.8;
	m_vDiffuse[2] = 0.8;  m_vDiffuse[3] = 1.0;

	m_vSpecular[0] = 1.0; m_vSpecular[1] = 1.0;
	m_vSpecular[2] = 1.0; m_vSpecular[3] = 1.0;

	m_vEmission[0] = 0.0; m_vEmission[1] = 0.0;
	m_vEmission[2] = 0.0; m_vEmission[3] = 1.0;

	m_fShininess = 64.0;

}

GLMaterial::~GLMaterial()
{
}

void GLMaterial::SelectFront(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_vAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_vDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_vSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_vEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, m_fShininess);
}

void GLMaterial::SelectBack(void)
{
	glMaterialfv(GL_BACK, GL_AMBIENT, m_vAmbient);
	glMaterialfv(GL_BACK, GL_DIFFUSE, m_vDiffuse);
	glMaterialfv(GL_BACK, GL_SPECULAR, m_vSpecular);
	glMaterialfv(GL_BACK, GL_EMISSION, m_vEmission);
	glMaterialf(GL_BACK, GL_SHININESS, m_fShininess);
}

void GLMaterial::Select(void)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_vAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_vDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_vSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_vEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_fShininess);
}

void GLMaterial::SetAmbient(float r, float g, float b, float a)
{
	m_vAmbient[0] = r;  m_vAmbient[1] = g;
	m_vAmbient[2] = b;  m_vAmbient[3] = a;
}

void GLMaterial::SetDiffuse(float r, float g, float b, float a)
{
	m_vDiffuse[0] = r;  m_vDiffuse[1] = g;
	m_vDiffuse[2] = b;  m_vDiffuse[3] = a;
}

void GLMaterial::SetSpecular(float r, float g, float b, float a)
{
	m_vSpecular[0] = r;  m_vSpecular[1] = g;
	m_vSpecular[2] = b;  m_vSpecular[3] = a;
}

void GLMaterial::SetEmission(float r, float g, float b, float a)
{
	m_vEmission[0] = r;  m_vEmission[1] = g;
	m_vEmission[2] = b;  m_vEmission[3] = a;
}

void GLMaterial::SetShininess(float s)
{
	if (s < 0.0)
		m_fShininess = 0.0;
	else if (s > 128.0)
		m_fShininess = 128.0;
	else
		m_fShininess = s;
}
