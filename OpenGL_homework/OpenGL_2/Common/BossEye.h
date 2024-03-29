#ifndef BossEye_H
#define BossEye_H
#include "../header/Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
#define RADIUS 3
 // 2 faces, 2 triangles/face 
#define Radius 0.7
class BossEye
{
private:

	// VAO
	GLuint m_uiVao;
	// VBO
	GLuint m_uiBuffer;
	//  for Shader
	GLuint m_uiProgram ;
	// Vertex Position Attribute
	GLuint m_uiModelView, m_uiProjection;
	// Matrix 
	mat4 m_mxView, m_mxProjection;
	mat4 m_mxMVFinal, m_mxTRS;

	// 紀錄是否有矩陣的更新
	bool  m_bUpdateMV;
	bool  m_bUpdateProj;
	
	void CreateBufferObject();
public:
	int corner = 15;
	int NUM = corner * 3;
	bool EyeC;
	vec4 m_Points[48];
	vec4 m_Colors[48];
	BossEye();
	int BossEyeHealth;//BossEye血量
	void SetShader(mat4 &mxModelView, mat4 &mxProjection, GLuint uiShaderHandle=MAX_UNSIGNED_INT);
	GLuint GetShaderHandle() { return m_uiProgram;}
	void SetViewMatrix(mat4 &mat);
	void SetProjectionMatrix(mat4 &mat);
	void  animation(float &f1);
	void SetTRSMatrix(mat4 &mat);
	void SetColor(GLfloat vColor[4]); // Single color
	void SetVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[]); // four Vertices' Color
	void Move(float dt ,float BossHealth);
	void ChangeColor();
	float Bx, By;
	float Ra;
	void Draw();
	void DrawW();
};




#endif