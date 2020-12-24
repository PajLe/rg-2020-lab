#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void MoveCamera(CPoint cursorPoint);
	void StopMovingCamera();
	void RotateX(float angle);
	void RotateY(float angle);

	int mod(int k, int n);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	short rotateX;
	short rotateY;

	void DrawSphere(float r);

private:
	CPoint lastPoint;
	bool firstMouse;
	float cameraPitch;
	float cameraYaw;
	float cameraX;
	float cameraY;
	float cameraZ;

	void DrawGrid();
	void DrawWholeFlower();
	void DrawFlowerpot();
	void DrawBottomRectPrism();
	void DrawBottomSphere();
	void DrawLeftCylinder();
	void DrawFirstLeftSphere();
	void DrawLeftRectPrism();
	void DrawSecondLeftSphere();
	void DrawFirstRightCylinder();
	void DrawFirstRightSphere();
	void DrawSecondRightOctaPrism();
	void DrawSecondRightSphere();
	void DrawRightRectPrism();
	void DrawRightTopmostSphere();
	void DrawRotatingRectPrism();
	void DrawFirstRotatingSphere();
	void DrawFirstLeftCone();
	void DrawFirstLeftConeSphere();
	void DrawSecondLeftCone();
	void DrawSecondLeftConeSphere();
	void DrawRightOctaPrism();
	void DrawRightSphere();
	void DrawRightCone();
	void DrawTopRightSphere();
};
