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

protected:
	HGLRC m_hrc; //OpenGL Rendering Context 
	short rotateX;
	short rotateY;
	double cameraDistanceFromCoordinateOrigin;
	bool normalsOn;
	bool redLightOn;
	bool greenLightOn;
	bool blueLightOn;

	void DrawHalfSphere(float r);
	float DrawUpsideDownConePart(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawConePart(float baseR, float fullConeHeight, float partConeHeight);
	float DrawCylinderPart(float baseR, float cylHeight);

private:
	CPoint lastPoint;
	bool firstMouse;
	float cameraPitch;
	float cameraYaw;
	float cameraX;
	float cameraY;
	float cameraZ;

	// importants
	void DrawLights();
	void SetRoomLightning();
	void DrawCoordinateLines();
	void DrawRoom();
	void DrawBase();
	void DrawCylinderPartOfTheBase();
	void DrawBox();
	void DrawVase();

	// helpers
	void DrawLeftWall();
	void DrawFrontWall();
	void DrawRightWall();
	void DrawBackWall();
	void DrawTopWall();
	void DrawBottomWall();

	float DrawVasePart1(float fullConeHeight, float partConeHeight);
	float DrawVasePart2(float baseR, float fullConeHeight, float partConeHeight);
	float DrawVasePart3(float baseR, float cylHeight);
	float DrawVasePart4(float baseR, float cylHeight);
	float DrawVasePart5(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawVasePart6(float baseR, float fullConeHeight, float partConeHeight);
	float DrawVasePart7(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawVasePart8(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawVasePart9(float baseR, float fullConeHeight, float partConeHeight);
	float DrawVasePart10(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawVasePart11(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawVasePart12(float baseR, float fullConeHeight, float partConeHeight);
	float DrawVasePart13(float smallerR, float fullConeHeight, float partConeHeight);
	float DrawVasePart14(float smallerR, float fullConeHeight, float partConeHeight);
};
