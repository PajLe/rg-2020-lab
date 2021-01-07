#pragma once
class GLTexture
{
public :
    u_int id;
    void PrepareTexture(bool lightEnabled);
    void LoadTexture(CString fileName);
    void Select();
    void Release();

    ~GLTexture();
    GLTexture();
};

