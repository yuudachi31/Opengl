// Model-View matrix and Projection matrix
// Orthogonal or Orthographic projection
// Windows Events - Keyboard & Mouse
// Rotate Rectangle

#include "header/Angel.h"
#include "Common/CQuad.h"
#include "Common/Propeller.h"
#include "Common/plane2.h"
#include "Common/Background1.h"
#include "Common/enemy.h"
#include "Common/enemy2.h"
#include "Common/enemy3.h"
#include "Common/bullet.h"
#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE (SCREEN_SIZE/2) 
#define VP_HALFWIDTH 12.0f
#define EnemyNum 4
#define BULLETNUM 20
// 必須在 glewInit(); 執行完後,在執行物件實體的取得
plane2 *Plane;
Background1 *bg1;
Propeller *g_pQuad[2];	// 宣告 Quad 指標物件，結束時記得釋放
enemy *Enemy1[EnemyNum];
enemy2 *Enemy2;
enemy3 *Enemy3[2];
bullet *phead,*pget,*ptail,*puse;

// For Model View and Projection Matrix
mat4 mxT, mxRot;
mat4 Em3R;
mat4 g_mxModelView(1.0f);
mat4 g_mxIdentity(1.0f);
mat4 g_mxProjection;
mat4 mxGT;
mat4 mEnemyMove;
mat4 mEnemy2Move;
mat4 mEnemy3Move;
mat4 BackgroundMove;
// For Rotation
int     g_iAxis = 2; // 旋轉軸，0:X, 1:Y, 2:Z
int PorM = 1;//enemy2 動畫
GLfloat g_fAngle = 0;   // X/Y/Z 三軸的旋轉角度
GLfloat g_fDir = 1;     // 旋轉方向
float g_TotTime = 0;
float em2T = 0;//Enemy2 動畫
float em2y = -1.6f;
float em2x = 1.2f;//Enemy2移動
float bgTime = 0;   //背景計時
float Em3A=0;//Enemy旋轉
float bg_y = 12.0f; //背景
float  g_fQuadT[2][3] = { 0 }; //不知
float Aangel = 0;//敵人動畫
bool    m_bAutoRotation = false; // Controlled by Space Key
float EnemyLoc[EnemyNum][3] = { 0 };     //Enemy1位置
float Enemy2Loc[3] = { -7,10,0 };
float Enemy3Loc[2][2] = { 0 };

float cooldown = 1;  //子彈冷卻
GLfloat g_fTx = 0, g_fTy = 0;
vec4 Enemyvec[4];   //Enemy位置
//----------------------------------------------------------------------------
// 函式的原型宣告
void IdleProcess();
void CreatePlayer();
void CreateEnemy();
void EnemyAnimate(float dt);
void CreateEnemy2();
void CreateEnemy3();
void CreateBackground();
void CreateBullet();

void init( void )
{
	// 必須在 glewInit(); 執行完後,在執行物件實體的取得
	/*g_pQuad = new CQuad;*/

	//  產生 projection 矩陣，此處為產生正投影矩陣
	g_mxProjection = Ortho(-VP_HALFWIDTH, VP_HALFWIDTH, -VP_HALFWIDTH, VP_HALFWIDTH, -1.0f, 1.0f);
	//g_pQuad->SetShader(g_mxModelView, g_mxProjection);
	//g_pQuad->SetAngleAndAxis(0, 2);
	CreateBackground();
	CreateEnemy();
	CreateEnemy2();
	CreateEnemy3();
	CreatePlayer();
	CreateBullet();
		glClearColor( 0.0, 0.0, 0.0, 1.0); // black background
}

void BGmovingf(float dt) {
	
	vec4 bts;
	bgTime += dt;
	if (bgTime > 1.5f)bgTime = 0;
	bg_y = 30*(bgTime - 0.5f);
	bts = vec4(0, -bg_y, 0, 0);
	BackgroundMove = Translate(bts);
	bg1->SetTRSMatrix(BackgroundMove);

}
void EnemyMoving(float dt) {
	mat4 EnemyTrans;
	
	for (int i = 0; i < EnemyNum; i++) {
		EnemyLoc[i][1] -= dt;
		if (EnemyLoc[i][1] < -12) {
			EnemyLoc[i][1] = 12.0f;
		}
		 
		EnemyLoc[i][0] += dt;
			if (EnemyLoc[i][0] > 12) {
				EnemyLoc[i][0] = -12.0f;
			}
			if (Enemy1[i]->EnemyHealth == 0) {
				EnemyLoc[i][1] += 16.0f;
				Enemy1[i]->EnemyHealth = 5;
			}
			Enemyvec[i] = vec4(EnemyLoc[i][0], EnemyLoc[i][1], 0, 0);
		EnemyTrans = Translate(Enemyvec[i]);
		Enemy1[i]->SetTRSMatrix(EnemyTrans);
	}

}
void Enemy2Moving(float dt) {
	if (Enemy2Loc[0] > 11.5f)em2x = -1.2f;
	if (Enemy2Loc[0] < -11.5f)em2x = 1.2f;
	if (Enemy2Loc[1] > 11.5f)em2y = -1.6f;
	if (Enemy2Loc[1] < -11.5f)em2y = 1.6f;
	Enemy2Loc[0] += em2x*3*dt;
	Enemy2Loc[1] += em2y*3*dt;
	mEnemy2Move = Translate(vec4(Enemy2Loc[0], Enemy2Loc[1], 0, 0));
	Enemy2->SetTRSMatrix(mEnemy2Move);
}
void ColdT(float dt) {
	if(cooldown < 1)cooldown += 5*dt;
	if (cooldown > 1) { cooldown =1 ;/* printf("down")*/; }
}
void Enemy3Moving(float dt) {
	
	Em3A = g_TotTime * 100*M_PI;
	Em3R = RotateZ(Em3A);

	if (Enemy3Loc[0][1] < -12.0f)Enemy3Loc[0][1] = 12.0f;
	if (Enemy3Loc[1][1] < -12.0f)Enemy3Loc[1][1] = 12.0f;
	Enemy3Loc[0][1] -= 2.5f*dt;
	Enemy3Loc[1][1] -= 2.5f*dt;
	mEnemy3Move = Translate(vec4(Enemy3Loc[0][0], Enemy3Loc[0][1], 0, 0));
	Enemy3[0]->SetTRSMatrix(mEnemy3Move*Em3R);
	mEnemy3Move = Translate(vec4(Enemy3Loc[1][0], Enemy3Loc[1][1], 0, 0));
	Enemy3[1]->SetTRSMatrix(mEnemy3Move*Em3R);
}
void EnemyAnimate(float dt) {
	
	
	Aangel += 4*dt;
	if (Aangel > 2* M_PI)Aangel = 0;
	for (int i = 0; i < EnemyNum; i++) {
		Enemy1[i]->animation(Aangel);
	}
	
		em2T += 15.0f*dt;
		if (em2T > 2 * M_PI)em2T = 0;
	Enemy2->animation(em2T);
}

void PropellerRotate(float dt){
	
	vec4 vT[2];
	g_TotTime += dt;
	if (g_TotTime >= 2.0*M_PI)g_TotTime = 0;
	g_fAngle = g_TotTime*800.0f;
	vT[0] = vec4(-0.9f, 0, 0, 0);
	vT[1] = vec4(0.9f, 0, 0, 0);
	mxRot = RotateZ(g_fAngle);

	for (int i = 0; i < 2; i++) {
		mxT = Translate(vT[i]);
		g_pQuad[i]->SetTRSMatrix(mxGT*mxT*mxRot);
	}

	//g_fAngle += g_fDir * 1.0f;     // 旋轉角度遞增(遞減) 0.125 度
	//if( g_fAngle > 360.0 ) g_fAngle -= 360;
	//else if (g_fAngle < 0.0) g_fAngle += 360.0;
	//else ;

	//// 計算旋轉矩陣並更新到 Shader 中
	


	//glutPostRedisplay();     // 目前的視窗需要被重畫
}
//----------------------------------------------------------------------------
void CreateBackground()
{

	GLuint uiShaderHandle;
	bg1 = new Background1;
	bg1->SetShader(g_mxModelView, g_mxProjection);
	uiShaderHandle = bg1->GetShaderHandle();
} 
void CreateEnemy()
{
	GLuint uiShaderHandle;
	for (int i = 0; i < EnemyNum; i++) {
		Enemy1[i] = new enemy;
		Enemy1[i]->SetShader(g_mxModelView, g_mxProjection);
		
	}
	for (int i = 0; i < EnemyNum; i++) {	
			EnemyLoc[i][0] = 10.0f*i+7.0f;
			if (EnemyLoc[i][0] > 12.0)
				EnemyLoc[i][0] -= 24.0f;
	}
	for (int i = 0; i < EnemyNum; i++) {
		EnemyLoc[i][1] = 5 * i + 7.0f;
	}
	
}
void CreateEnemy2()
{
	GLuint uiShaderHandle;
	
	Enemy2 = new enemy2;
	Enemy2->SetShader(g_mxModelView, g_mxProjection);
	uiShaderHandle = Enemy2->GetShaderHandle();
}
void CreateEnemy3() {
	GLuint uiShaderHandle;
	for (int i = 0; i < 2; i++) {
		Enemy3[i] = new enemy3;
		Enemy3[i]->SetShader(g_mxModelView, g_mxProjection);

	}
	Enemy3Loc[0][0] = -9.5f;
	Enemy3Loc[0][1] = 12.0f;
	Enemy3Loc[1][0] = 9.5f;
	Enemy3Loc[1][1] = 12.0f;
}
void CreatePlayer()
{
	GLuint uiShaderHandle;
	vec4 vColor = vec4(0, 0, 0, 1);
	vec4 vT;
	mat4 mxT;
	int idx = 0;

	// x-> -10 -8 -6 -4 -2 0  2  4  6  8  10
	// y->  10  8  6  4  2 0 -2 -4 -6 -8 -10
	Plane = new plane2;
	Plane->SetShader(g_mxModelView, g_mxProjection);
	uiShaderHandle = Plane->GetShaderHandle();
	for (int i = 0; i < 2; i++) {
		
			
			g_pQuad[idx] = new Propeller;
			if (idx == 0) { // 第一個物件，必須建立 Shader
				g_fQuadT[idx][0] = -1;
				g_pQuad[idx]->SetShader(g_mxModelView, g_mxProjection, uiShaderHandle);
				
				
				
			}
			else{
				g_pQuad[idx]->SetShader(g_mxModelView, g_mxProjection, uiShaderHandle);
				g_fQuadT[idx][0] = -20;
				
			}
			mxT = Translate(g_fQuadT[idx][0], g_fQuadT[idx][1], g_fQuadT[idx][2]);
			g_pQuad[idx]->SetTRSMatrix(mxT);
			idx++;
		}
	}
void CreateBullet() {
	GLuint uiShaderHandle;
	phead = new bullet; phead->Link = NULL;
	ptail = phead;
	ptail->SetShader(g_mxModelView, g_mxProjection);
	for (int i = 0; i < BULLETNUM; i++) {
		pget = new bullet; pget->Link = NULL;
		ptail->Exact = false;
		ptail->Link = pget;
		ptail = pget;
		ptail->SetShader(g_mxModelView, g_mxProjection);
		
		
	}
	pget = phead;
	
}
void shoot() {
	pget = phead;
	while (pget->Exact == true) {
		pget = pget->Link;
}
	
	if (cooldown == 1) {
		pget->Shoot(g_fTx, g_fTy);
		pget->Exact = true;
		cooldown = 0;
	}
}
void HitOrNot(float dt) {
	pget = phead;
	while (pget != NULL) {
		if (pget->Exact == true) {
			for (int i = 0; i < EnemyNum; i++) {
				if (pget->Loc.x < Enemyvec[i].x + 0.7f && pget->Loc.x  > Enemyvec[i].x - 0.7f && pget->Loc.y > Enemyvec[i].y - 0.7f&&pget->Loc.y < Enemyvec[i].y + 0.7f)
				{
					pget->Exact = false; printf("hit""\n");
					Enemy1[i]->EnemyHealth -= 1;
				}
			}
		}
		pget = pget->Link;
	}
	
}
void GL_Display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	bg1->Draw();
	for (int i = 0; i < EnemyNum; i++) {
		Enemy1[i]->DrawW();
	}
	Enemy2->DrawW();
	Enemy3[0]->DrawW();
	Enemy3[1]->DrawW();
	Plane->DrawW();
	g_pQuad[0]->DrawW();
	g_pQuad[1]->DrawW();
	
	

	pget = phead;
	while (pget != NULL) {               //參考自洪欣儀110619018
		if(pget->Exact==true)
		pget->DrawW();
		pget = pget->Link;
	}
		
	
		
		
	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	HitOrNot(delta);
	BGmovingf(delta);
	EnemyAnimate(delta);
	PropellerRotate(delta);
	EnemyMoving(delta);
	Enemy2Moving(delta);
	Enemy3Moving(delta);
	ColdT(delta);
	pget = phead;
	for (int i = 0; i < BULLETNUM; i++) {
		if (pget->Exact == true)pget->Move(delta);
		pget = pget->Link;
	}

	GL_Display();
}
void Win_PassiveMotion(int x, int y) {
	

	g_fTx = 12.0f*(x - HALF_SIZE) / (HALF_SIZE);
	g_fTy = -12.0f*(y - HALF_SIZE) / (HALF_SIZE);
	mxGT = Translate(g_fTx, g_fTy, 0);
	Plane->SetTRSMatrix(mxGT);
	for (int i = 0; i < 2; i++) {
		
		g_pQuad[i]->SetTRSMatrix(mxGT*mxT*mxRot);
	}
}
//----------------------------------------------------------------------------
void reset()
{ 
	g_iAxis = 2; 
	g_fAngle = 0;
	g_fDir = 1;    
	m_bAutoRotation = false;
	g_pQuad[0]->SetAngleAndAxis(0, 2);
	g_pQuad[1]->SetAngleAndAxis(0, 2);
	g_pQuad[0]->SetTRSMatrix(g_mxModelView);
	g_pQuad[1]->SetTRSMatrix(g_mxModelView);
	glutPostRedisplay();
}
//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case  SPACE_KEY:
		m_bAutoRotation = !m_bAutoRotation;
		break;
	case 49: // 1 key
		Plane->MODE1();
		break;
	case 50:
		Plane->MODE2();
		break;
	case 51:
		Plane->MODE3();
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
		delete g_pQuad;
        exit( EXIT_SUCCESS );
        break;
    }
}
//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			if ( state == GLUT_DOWN ) {  // 目前的滑鼠狀態是按住，換成 X 軸
				shoot();
			}
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
			if ( state == GLUT_DOWN ) {
				
			}
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			if ( state == GLUT_DOWN ){ // 目前按下的是滑鼠右鍵，換成 Z 軸
				
			} 
			break;
		default:
			break;
	} 
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	mat4 mxRot;
	if( !m_bAutoRotation ) { // 沒有自動旋轉下才有作用
		switch(key) {
			case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
				g_fAngle += 2.0;		// 逆時針為正方向
				break;
			case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵
				g_fAngle -= 2.0;		// 順時針為負方向
				break;
			default:
				break;
		}

		// 計算旋轉矩陣並更新到 Shader 中
		/*if( g_iAxis == 0 )  mxRot =  RotateX(g_fAngle);
		else if( g_iAxis == 1 )  mxRot =  RotateY(g_fAngle);
		else mxRot =  RotateZ(g_fAngle);*/

		/*g_pQuad->SetTRSMatrix(mxRot);*/
		g_pQuad[0]->SetAngleAndAxis(g_fAngle, g_iAxis);
		g_pQuad[1]->SetAngleAndAxis(g_fAngle, g_iAxis);

		glutPostRedisplay();  
	}
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
}
void Win_MouseMotion(int x, int y) {
	g_fTx = 12.0f*(x - HALF_SIZE) / (HALF_SIZE);
	g_fTy = -12.0f*(y - HALF_SIZE) / (HALF_SIZE);
	shoot();
	mxGT = Translate(g_fTx, g_fTy, 0);

	
	Plane->SetTRSMatrix(mxGT);
	for (int i = 0; i < 2; i++) {

		g_pQuad[i]->SetTRSMatrix(mxGT*mxT*mxRot);
	}
	}
//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutPassiveMotionFunc(Win_PassiveMotion);
	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "My Game" );

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutKeyboardFunc(Win_Keyboard);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutSpecialFunc( Win_SpecialKeyboard);	
	glutDisplayFunc(GL_Display);
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}
