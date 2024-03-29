#include "fire.h"

fire ::fire()
{
	OnUse = false;
	Exact = false;
	m_Points[0] = vec4( -0.2f,  0.2f, 0.0f, 1.0f);  //1
	m_Points[1] = vec4(  0.2f,  0.2f, 0.0f, 1.0f);
	m_Points[2] = vec4(  0.2f, -0.2f, 0.0f, 1.0f);
	m_Points[3] = vec4(0.2f, -0.2f, 0.0f, 1.0f);    //2
	m_Points[4] = vec4(-0.2f, -0.2f, 0.0f, 1.0f);
	m_Points[5] = vec4(-0.2f, 0.2f, 0.0f, 1.0f);
	m_Points[6] = vec4(0.0f, 0.0f, 0.0f, 1.0f);    //3
	m_Points[7] = vec4(0.2f, 10.0f, 0.0f, 1.0f);
	//m_Points[8] = vec4(-0.2f, 0.7f, 0.0f, 1.0f);
	//m_Points[9] = vec4(0.0f, 0.0f, 0.0f, 1.0f);    //4
	//m_Points[10] = vec4(1.2f, 0.0f, 0.0f, 1.0f);
	//m_Points[11] = vec4(0.35f, 0.39f, 0.0f, 1.0f);
	//m_Points[12] = vec4(0.0f, 0.0f, 0.0f, 1.0f);  //5
	//m_Points[13] = vec4(-1.2f, 0.0f, 0.0f, 1.0f);
	//m_Points[14] = vec4(-0.35f, 0.39f, 0.0f, 1.0f);
	//m_Points[15] = vec4(1.2f, 0.0f, 0.0f, 1.0f);  //6
	//m_Points[16] = vec4(1.0f, -0.2f, 0.0f, 1.0f);
	//m_Points[17] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_Points[18] = vec4(-1.2f, 0.0f, 0.0f, 1.0f);  //7
	//m_Points[19] = vec4(-1.0f, -0.2f, 0.0f, 1.0f);
	//m_Points[20] = vec4(0.0f, 0.0f, 0.0f, 1.0f);


	m_Colors[0] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[1] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[2] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[3] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[4] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[5] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[6] = vec4(0.3f, 0.7f, 0.7f, 1.0f);
	m_Colors[7] = vec4(1.0f, 0.8f, 0.2f, 1.0f);
	m_Colors[8] = vec4(0.88f, 0.48f, 0.24f, 1.0f);
	m_Colors[9] = vec4(1.0f, 0.8f, 0.2f, 1.0f);
	m_Colors[10] = vec4(1.0f, 0.8f, 0.2f, 1.0f);
	m_Colors[11] = vec4(0.88f, 0.48f, 0.24f, 1.0f);
	m_Colors[12] = vec4(1.0f, 0.8f, 0.2f, 1.0f);
	m_Colors[13] = vec4(0.88f, 0.48f, 0.24f, 1.0f);
	m_Colors[14] = vec4(0.88f, 0.48f, 0.24f, 1.0f);
	m_Colors[15] = vec4(1.0f, 0.8f, 0.2f, 1.0f);
	m_Colors[16] = vec4(1.0f, 0.1f, 0.05f, 1.0f);
	m_Colors[17] = vec4(1.0f, 0.8f, 0.2f, 1.0f);
	m_Colors[18] = vec4(0.88f, 0.48f, 0.24f, 1.0f);
	m_Colors[19] = vec4(1.0f, 0.1f, 0.05f, 1.0f);
	m_Colors[20] = vec4(1.0f, 0.8f, 0.2f, 1.0f);  // (r, g, b, a)
	Link = NULL;
	R_angle = 1.0f;
	animateT = 0;
	Sx = 0.0f;
	Sy = 0.0f;
	Sz = 0.0f;
	// Create and initialize a buffer object 
	CreateBufferObject();
	m_bUpdateProj = false;
}
void fire::Shoot(float bx, float by) {
	BLoc[0] = bx; BLoc[1] = by;
//	printf("got");
}

void fire::Move(float dt) {
	animateT += dt;
	R_angle += dt * 360*2;
	if (R_angle > 360)R_angle = 0;
	if (animateT < 0.3f) {
		Sx += 2*dt;
		Sy += 2*dt;
		Loc = vec4(BLoc[0], BLoc[1], 0, 0);
		Bfly = Translate(Loc);
		m_mxTRS = Bfly*RotateZ(R_angle)*Scale(Sx, Sy, Sz);;

	}
	if (animateT >0.3f ) {
		Loc = vec4(BLoc[0], BLoc[1], 0, 0);
		Bfly = Translate(Loc);
		if (Sx > 0 || Sy > 0) {
			Sx -= dt;
			Sy -= dt;
		}
		//FScale = vec4(1.0f-=dt,1.0f-=dt,0,1.0);
		m_mxTRS = Bfly*RotateZ(R_angle)*Scale(Sx,Sy,Sz);
	}
	if (animateT > 1.0f) {
		Sx = 0.0f;
		Sy = 0.0f;
		Sz = 0.0f;
		Exact = false;
		animateT = 0;
	}
		m_bUpdateMV = true; 
	
	}
	

void fire::animation(float &f1) {

	m_Points[1] = vec4(0.8f + 0.2f*f1, 1.2f + 0.2*f1, 0.0f, 1.0f);
	m_Points[2] = vec4(0.8f + 0.2*f1, -1.2f + 0.2f*f1, 0.0f, 1.0f);
	m_Points[4] = vec4(-0.8f + 0.2f*f1, 1.2f + 0.2f*f1, 0.0f, 1.0f);
	m_Points[5] = vec4(-0.8f + 0.2f*f1, -1.2f + 0.2f*f1, 0.0f, 1.0f);
	

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_Points), m_Points);
}
void fire::CreateBufferObject()
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

void fire::SetShader(mat4 &mxView, mat4 &mxProjection, GLuint uiShaderHandle)
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

void fire::SetViewMatrix(mat4 &mat)
{
	m_mxView = mat;
	m_bUpdateMV = true;
}

void fire::SetProjectionMatrix(mat4 &mat)
{
	m_mxProjection = mat;
	m_bUpdateProj = true;
}

void fire::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bUpdateMV = true;
}

void fire::SetColor(GLfloat vColor[4])
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

void fire::SetVtxColors(GLfloat vLFColor[], GLfloat vLRColor[], GLfloat vTRColor[], GLfloat vTLColor[])
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

void fire::Draw()
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
	glDrawArrays( GL_TRIANGLES, 0, QUAD_NUM );
}

void fire::DrawW()
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
	glDrawArrays( GL_TRIANGLES, 0, QUAD_NUM );
}
