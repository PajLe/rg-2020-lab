#pragma once
#include "GLTexture.h"
#include "GLMaterial.h"

class CGLRenderer
{
public:
	CGLRenderer();
	virtual ~CGLRenderer();
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void MoveCamera(CPoint cursorPoint);
	void StopMovingCamera();

protected:
	HGLRC m_hrc; //OpenGL Rendering Context 
	short rotateX;
	short rotateY;
	double cameraDistanceFromCoordinateOrigin;

// attributes
private:
	// camera
	CPoint lastPoint;
	bool firstMouse;
	float cameraPitch;
	float cameraYaw;
	float cameraX;
	float cameraY;
	float cameraZ;

	// textures
	GLTexture* terrain;
	GLTexture* truck;

	// materials
	GLMaterial* truckMat;

// functions
private:
	// importants
	void SetRoomLightning();
	void DrawCoordinateLines();
	void DrawTerrain();
	void DrawWheels();
	void DrawTruckBody();

	// helpers
	void DrawWheel();

	// function splitters
	void DrawBottomLeftWheel();
	void DrawTopLeftWheel();
	void DrawBottomRightWheel();
	void DrawTopRightWheel();
	void DrawLeftTruckSide();
	void DrawRightTruckSide();

};
