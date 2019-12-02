#ifndef enemy_H
#define enemy_H
#include "../header/Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
#define CornerNUM 6
#define QUAD_NUM  CornerNUM + 2 // 2 faces, 2 triangles/face 
#define Radius 0.7
class enemy
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
	int NUM;
	vec4 m_Points[QUAD_NUM];
	vec4 m_Colors[QUAD_NUM];
	enemy();
	int EnemyHealth;//Enemy血量
	void SetShader(mat4 &mxModelView, mat4 &mxProjection, GLuint uiShaderHandle=MAX_UNSIGNED_INT);
	GLuint GetShaderHandle() { return m_uiProgram;}
	void SetViewMatrix(mat4 &mat);
	void SetProjectionMatrix(mat4 &mat);
	void virtual animation(float &f1);
	void SetTRSMatrix(mat4 &mat);
	void SetColor(GLfloat vColor[4]); // Single color
	void SetVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[]); // four Vertices' Color

	void Draw();
	void DrawW();
};




#endif