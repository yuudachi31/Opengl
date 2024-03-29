// Multi Objects with same shader program

#include "header/Angel.h"
#include "Common/CQuad.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE (SCREEN_SIZE/2) 
#define VP_HALFWIDTH  12.0f
#define GRID_SIZE 11 

// For grid quad
CQuad *g_pGridQuad[GRID_SIZE*GRID_SIZE]; // 鋪成一個 11X11 的棋盤格子

// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxIdentity(1.0f);
mat4 g_mxProjection;

// 只對 Z 軸旋轉
GLfloat g_fZAngle = 0;   // Z 軸的旋轉角度
GLfloat g_fDir = 1;     // 旋轉方向
bool    m_bAutoRotation = false; // Controlled by Space Key

//----------------------------------------------------------------------------
// 函式的原型宣告
void IdleProcess();
void CreateGridQuad();

void init( void )
{
	//  產生 projection 矩陣，此處為產生正投影矩陣
    g_mxProjection = Ortho(-VP_HALFWIDTH, VP_HALFWIDTH, -VP_HALFWIDTH, VP_HALFWIDTH, -1.0f, 1.0f);

	// 必須在 glewInit(); 執行完後,在執行物件實體的取得
	CreateGridQuad();

    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
}

void AutomaticRotation(void){
	mat4 mxT, mxRy;
	vec4 vT;
	int idx = 0;

	g_fZAngle += g_fDir * 0.25f;    // 旋轉角度遞增(遞減) 0.25
	// 計算旋轉矩陣並更新到 Shader 中
	mxRy = RotateZ(g_fZAngle); //  degree 

	for( int i = 0 ; i < GRID_SIZE ; i++ ) {
		vT.y = 10 - i*2.0f;
		for( int j = 0 ; j < GRID_SIZE ; j++ ) {
			vT.x = -10 + j*2.0f;
			mxT = Translate(vT);	
			g_pGridQuad[idx]->SetTRSMatrix(mxT*mxRy);
			idx++;
		}
	}
}
//----------------------------------------------------------------------------

void GL_Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT ); // clear the window

	g_pGridQuad[0]->Draw();
	for( int i = 1 ; i < GRID_SIZE*GRID_SIZE ; i++ )  {
		g_pGridQuad[i]->DrawW();
	}

	//for( int i = 0 ; i < GRID_SIZE*GRID_SIZE ; i++ )  {
	//	g_pGridQuad[i]->Draw();
	//}
	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	if( m_bAutoRotation ) AutomaticRotation();
	GL_Display();
}

//----------------------------------------------------------------------------
void reset()
{ 
	mat4 mxT;
	vec4 vT;
	int idx = 0;

	g_fZAngle = 0;
	g_fDir = 1;    
	m_bAutoRotation = false;

	for( int i = 0 ; i < GRID_SIZE ; i++ ) {
		vT.y = 10 - i*2.0f;
		for( int j = 0 ; j < GRID_SIZE ; j++ ) {
			vT.x = -10 + j*2.0f;
			mxT = Translate(vT);	
			g_pGridQuad[idx]->SetTRSMatrix(mxT);
			idx++;
		}
	}
}

//----------------------------------------------------------------------------
void CreateGridQuad()
{
	GLuint uiShaderHandle;
	vec4 vColor = vec4( 0, 0, 0, 1);
	vec4 vT;
	mat4 mxT;
	int idx = 0;

	// x-> -10 -8 -6 -4 -2 0  2  4  6  8  10
	// y->  10  8  6  4  2 0 -2 -4 -6 -8 -10

	for( int i = 0 ; i < GRID_SIZE ; i++ ) {
		vT.y = 10 - i*2.0f;
		for( int j = 0 ; j < GRID_SIZE ; j++ ) {
			vT.x = -10 + j*2.0f;
			g_pGridQuad[idx] = new CQuad;
			if( idx == 0 )  { // 第一個物件，必須建立 Shader
				g_pGridQuad[idx]->SetShader(g_mxModelView, g_mxProjection);
				uiShaderHandle = g_pGridQuad[idx]->GetShaderHandle(); // 取得 Shader Handle
				vColor.x = 0.15f; vColor.y = 0.3f; vColor.z = 0.3f; vColor.w = 1.0f;
				g_pGridQuad[idx]->SetColor(vColor);
			}
			else {
				g_pGridQuad[idx]->SetShader(g_mxModelView, g_mxProjection, uiShaderHandle);
				vColor.x = j*0.08f + 0.15f; vColor.y = i*0.06f + 0.3f; vColor.z =  0.3f; vColor.w = 1.0f;
				g_pGridQuad[idx]->SetColor(vColor);
			}
			mxT = Translate(vT);	
			g_pGridQuad[idx]->SetTRSMatrix(mxT);
			idx++;
		}
	}
}
//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case  SPACE_KEY:
		m_bAutoRotation = !m_bAutoRotation;
		break;
	case 68: // D key
	case 100: // d key
		g_fDir = -1 * g_fDir;
		break;
	case 82: // R key
	case 114: // r key
		reset();
		break;
    case 033:
		glutIdleFunc( NULL );
		for( int i = 0 ; i < GRID_SIZE*GRID_SIZE ; i++ ) delete g_pGridQuad[i];
        exit( EXIT_SUCCESS );
        break;
    }
}
//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			if ( state == GLUT_DOWN );  
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 
			if ( state == GLUT_DOWN ) ;
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			if ( state == GLUT_DOWN ) ; 
			break;
		default:
			break;
	} 
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	mat4 mxT, mxRy;
	vec4 vT;
	int idx = 0;
	mat4 rx, ry, rz;
	if( !m_bAutoRotation ) { // 沒有自動旋轉下才有作用
		switch(key) {
			case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
				g_fZAngle += 2.0;
				break;
			case GLUT_KEY_RIGHT:		// 目前按下的是向右方向鍵
				g_fZAngle -= 2.0;
				break;
			default:
				break;
		}
		// 計算旋轉矩陣並更新到 Shader 中
		mxRy = RotateZ(g_fZAngle); //  degree 
		for (int i = 0; i < GRID_SIZE; i++) {
			vT.y = 10 - i*2.0f;
			for (int j = 0; j < GRID_SIZE; j++) {
				vT.x = -10 + j*2.0f;
				mxT = Translate(vT);
				g_pGridQuad[idx]->SetTRSMatrix(mxT*mxRy);
				idx++;
			}
		}
	}
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Draw two triangles" );

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}
