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
	void SwitchNormals();
	void SwitchRedLight();
	void SwitchGreenLight();
	void SwitchBlueLight();

	int mod(int k, int n);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	short rotateX;
	short rotateY;
	double cameraDistanceFromCoordinateOrigin;
	bool normalsOn;
	bool redLightOn;
	bool greenLightOn;
	bool blueLightOn;

	void DrawHalfSphere(float r);

private:
	CPoint lastPoint;
	bool firstMouse;
	float cameraPitch;
	float cameraYaw;
	float cameraX;
	float cameraY;
	float cameraZ;

	// lab 5
	void DrawLights();
	void SetRoomLightning();
	void DrawCoordinateLines();
	void DrawRoom();
	void DrawLeftWall();
	void DrawFrontWall();
	void DrawRightWall();
	void DrawBackWall();
	void DrawTopWall();
	void DrawBottomWall();
	void DrawBase();
	void DrawCylinder();
	void DrawBox();
	void DrawVase();
	float DrawVasePart1(float fullConeHeight, float partConeHeight);
	float DrawVasePart2(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart3(float firstR, float partConeHeight);
	float DrawVasePart4(float firstR, float partConeHeight);
	float DrawVasePart5(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart6(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart7(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart8(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart9(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart10(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart11(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart12(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart13(float firstR, float fullConeHeight, float partConeHeight);
	float DrawVasePart14(float firstR, float fullConeHeight, float partConeHeight);
};
