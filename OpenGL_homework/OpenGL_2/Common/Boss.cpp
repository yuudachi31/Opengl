#include "Boss.h"

Boss::Boss()
{
	
	for (int i = 0; i < corner; i++) {
		m_Points[3 * i] = vec4(0, 0, 0, 1.0f);
		m_Points[3 * i + 1] = vec4(1.2f *RADIUS * cosf(M_PI*2.0f* i / corner), 1.2f * RADIUS * sinf(M_PI*2.0f* i / corner), 0, 1.0f);
		m_Points[3 * i + 2] = vec4(0.7f*RADIUS * cosf(M_PI*2.0f* (i + 1) / corner), 0.7f * RADIUS * sinf(M_PI*2.0f* (i + 1) / corner), 0, 1.0f);

	}

	
	for (int i = 0; i < corner; i++)
	{
		m_Colors[3 * i]= vec4(1.0f,0.0,0.0,1.0);
		m_Colors[3 * i + 1]= vec4(0.0f, 0.0, 0.0, 1.0);
		m_Colors[3 * i + 2] = vec4(1.0f, 0.0, 0.0, 1.0);
	}
	Ra = 90.0f;
	Rsa = 0;
	BossMode = 1;
	BossC = false;
	/*m_Colors[0] = vec4(3.2f, 2.0f, 1.5f, 1.0);
	for (int i = 1; i <= CornerNUM + 1; i++)
	{
		m_Colors[i] = vec4(i*0.1f + 0.1f, 1.0f / i, 1.0f, 0.5f);
	}*/
	// Create and initialize a buffer object 
	CreateBufferObject();
	m_bUpdateProj = false;
	 BossHealth = 60;//Boss¦å¶q
}


void Boss::CreateBufferObject()
{
    glGenVertexArrays( 1, &m_uiVao );
    glBindVertexArray( m_uiVao );

    // Create and initialize a buffer object

    glGenBuffers( 1, &m_uiBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(m_Points) + sizeof(m_Colors), NULL, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(m_Points), m_Points ); 
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(m_Points), sizeof(m_Colors), m_Colors );
}
void Boss::ChangeColor() {
	for (int i = 0; i < corner; i++)
	{
		m_Colors[3 * i] = vec4(0.0f, 0.0, 1.0, 1.0);
		m_Colors[3 * i + 1] = vec4(1.0f, 1.0, 1.0, 1.0);
		m_Colors[3 * i + 2] = vec4(0.0f, 0.0, 1.0, 1.0);

	}

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_Points), sizeof(m_Colors), m_Colors);
}
void Boss::BossK() {
	corner -= 1.0f;
	if (corner < 7)corner = 7;
	for (int i = 0; i < corner; i++) {
		m_Points[3 * i] = vec4(0, 0, 0, 1.0f);
		m_Points[3 * i + 1] = vec4(1.2f *RADIUS * cosf(M_PI*2.0f* i / corner), 1.2f * RADIUS * sinf(M_PI*2.0f* i / corner), 0, 1.0f);
		m_Points[3 * i + 2] = vec4(0.7f*RADIUS * cosf(M_PI*2.0f* (i + 1) / corner), 0.7f * RADIUS * sinf(M_PI*2.0f* (i + 1) / corner), 0, 1.0f);

	}
	

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_Points), m_Points);
	
}
void Boss::Move(float dt) {
	if (BossHealth < 40 && BossHealth>20)BossMode = 2;
	if (BossHealth <=20 && BossHealth >=0)BossMode = 3;
	Rsa += M_PI*100*dt;
	if (BossMode == 1||BossMode == 2) {
		Bx = 9.0f* cosf(Ra += M_PI*0.1f*dt);
		By = 9.0f* sinf(Ra += M_PI*0.1f*dt);
	}
	if (BossMode == 3) {
		Bx = 9.0f* cosf(Ra += M_PI*0.4f*dt);
		By = 9.0f* sinf(Ra += M_PI*0.4f*dt);
	}
	if (Ra > 360)Ra = 0;
	if (Rsa > 360)Rsa = 0;
	m_mxTRS = Translate(vec4(Bx,By,0,1.0))*RotateZ(Rsa);
	m_bUpdateMV = true;
}
void Boss::animation(float &f1) {
	float x;
	float y;
	
	
		m_Points[0].x =0.5f* cosf(f1);
		m_Points[0].y =0.2f* sinf(f1);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_Points), m_Points);
}
void Boss::SetShader(mat4 &mxView, mat4 &mxProjection, GLuint uiShaderHandle)
{
    // Load shaders and use the resulting shader program
	if( uiShaderHandle == MAX_UNSIGNED_INT) m_uiProgram = InitShader("vsVtxColor.glsl", "fsVtxColor.glsl");
	else m_uiProgram = uiShaderHandle;

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( m_uiProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( m_uiProgram, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_Points)) );

	m_uiModelView = glGetUniformLocation( m_uiProgram, "ModelView" );
	m_mxView = mxView;
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxView );

    m_uiProjection = glGetUniformLocation( m_uiProgram, "Projection" );
	m_mxProjection = mxProjection;
	glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Boss::SetViewMatrix(mat4 &mat)
{
	m_mxView = mat;
	m_bUpdateMV = true;
}

void Boss::SetProjectionMatrix(mat4 &mat)
{
	m_mxProjection = mat;
	m_bUpdateProj = true;
}

void Boss::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bUpdateMV = true;
}

void Boss::SetColor(GLfloat vColor[4])
{
	for( int i = 0 ; i < 6 ; i++ ) {
		m_Colors[i].x = vColor[0];
		m_Colors[i].y = vColor[1];
		m_Colors[i].z = vColor[2];
		m_Colors[i].w = vColor[3];
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(m_Points), sizeof(m_Colors), m_Colors );
}

void Boss::SetVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[])
{
	m_Colors[0].x = vLFColor[0];
	m_Colors[0].y = vLFColor[1];
	m_Colors[0].z = vLFColor[2];
	m_Colors[0].w = vLFColor[3];
	m_Colors[3] = m_Colors[0];

	m_Colors[1].x = vLRColor[0];
	m_Colors[1].y = vLRColor[1];
	m_Colors[1].z = vLRColor[2];
	m_Colors[1].w = vLRColor[3];

	m_Colors[2].x = vTRColor[0];
	m_Colors[2].y = vTRColor[1];
	m_Colors[2].z = vTRColor[2];
	m_Colors[2].w = vTRColor[3];
	m_Colors[4] = m_Colors[2];

	m_Colors[5].x = vTLColor[0];
	m_Colors[5].y = vTLColor[1];
	m_Colors[5].z = vTLColor[2];
	m_Colors[5].w = vTLColor[3];

	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(m_Points), sizeof(m_Colors), m_Colors );
}

void Boss::Draw()
{
	glUseProgram( m_uiProgram );
	glBindVertexArray( m_uiVao );
	if( m_bUpdateMV ) {
		m_mxMVFinal = m_mxView*m_mxTRS;
		m_bUpdateMV = false;
	}
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

	if( m_bUpdateProj ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bUpdateProj = false;
	}
	glDrawArrays( GL_TRIANGLES, 0,  80);
}

void Boss::DrawW()
{
	glBindVertexArray( m_uiVao );

	if( m_bUpdateMV ) {
		m_mxMVFinal = m_mxView*m_mxTRS;
		m_bUpdateMV = false;
	}

	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );
	if( m_bUpdateProj ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bUpdateProj = false;
	}
	glDrawArrays( GL_TRIANGLES, 0, NUM);
}
