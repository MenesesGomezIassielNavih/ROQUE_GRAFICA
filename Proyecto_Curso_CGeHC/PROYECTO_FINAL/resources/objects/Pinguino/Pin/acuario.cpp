/*---------------------------------------------------------*/
/* ------------ TEORIA PROYECTO FINAL-----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno: David González Ramírez---------------*/
/*------------- No. Cuenta: 417061438 ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 70.0f, 130.0f));//0,10,90 vista de la camara principal
float MovementSpeed = 3.5f;//.1
float lastX = SCR_WIDTH / 8.0f;//2
float lastY = SCR_HEIGHT / 8.0f;//2
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting					  4		-10
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f); //Fuente de luz posicional, direccion de donde debe de salir 
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f); //Posicion desde donde esta el sol 

glm::vec3 pez01(0.0f, -1.0f, -1.0f);
glm::vec3 pez02(0.0f, -1.0f, -1.0f);
glm::vec3 tshark(0.0f, -1.0f, -1.0f);
glm::vec3 tfish(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa02(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa03(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa04(0.0f, -1.0f, -1.0f);

// posiciones

float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,

	// Variables proyecto
		// Pez 1
		movCola = 0.0f,
        posXpez = 0.0f,
		rotPez = 0.0,

        // Pez 2
	    posXpez2 = 0.0f,
		rotPezA = 0.0f,

		//Tiburon
		posXshark = 0.0f,
		rotshark = 0.0f,
	    movColaShark = 0.0f,
		movAletasShark = 0.0f,
		movBocaShark = 0.0f,


		//Pescado Gris
		posXfish = 0.0f,
		rotfish = 0.0f,

		//Medusa
        posXmedusa = 0.0f,

		// Niña

		posXniña = 0.0f,
		posZniña = 0.0f,
		movBrazoDerNiña = 0.0f,
		movBrazoIzqNiña = 0.0f,
		rotPiernaDerNiña = 0.0f,
		rotPiernaIzqNiña = 0.0f;
		

		
		
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,

	// Variables proyecto

		movColaInc = 0.0f,
		posXpezInc = 0.0f,
	    rotPezInc = 0.0,

	    posXpez2Inc = 0.0f,
		rotPezAInc = 0.0f,

		posXsharkInc = 0.0f,
		rotsharkInc = 0.0f,
		movColaSharkInc = 0.0f,
		movAletasSharkInc = 0.0f,
		movBocaSharkInc = 0.0f,

		posXfishInc = 0.0f,
		rotfishInc = 0.0f,

        posXmedusaInc = 0.0f,

		posXniñaInc = 0.0f,
		posZniñaInc = 0.0f,
		movBrazoDerNiñaInc = 0.0f,
		movBrazoIzqNiñaInc = 0.0f,
		rotPiernaDerNiñaInc = 0.0f,
		rotPiernaIzqNiñaInc = 0.0f;
	

//Colores declarados 
float	colorR = 2.4f,
		colorG = 0.86f,
		colorB = 1.5f;

float rotx = (-20.0f, 0.0f, 0.0f);
float roty = (0.0f, 40.0f, 0.0f);
float movy = 0.0f;

float rot = 0.0f;//Se agrega
bool anim = false;//Se agrega-----------------------------------------------
bool anim2 = false;//SEGUNDO ANIM
bool anim3 = false;


float t = 0.0f;
float tpez = 0.0f;

#define MAX_FRAMES 20
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

	//Variables proyecto
	float movCola;
	float posXpez;
	float rotPez;

	float posXpez2;
	float rotPezA;

	float posXshark;
	float rotshark;
	float movColaShark;
	float movAletasShark;
	float movBocaShark;

	float posXfish;
	float rotfish;

	float posXmedusa;

	float posXniña;
	float posZniña;
	float movBrazoDerNiña;
	float movBrazoIzqNiña;
	float rotPiernaDerNiña;
	float rotPiernaIzqNiña;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	// Proyecto
	KeyFrame[FrameIndex].movCola = movCola;
	KeyFrame[FrameIndex].posXpez = posXpez;
	KeyFrame[FrameIndex].rotPez = rotPez;

	KeyFrame[FrameIndex].posXpez2 = posXpez2;
	KeyFrame[FrameIndex].rotPezA = rotPezA;

	KeyFrame[FrameIndex].posXshark = posXshark;
	KeyFrame[FrameIndex].rotshark = rotshark;
	KeyFrame[FrameIndex].movColaShark = movColaShark;
	KeyFrame[FrameIndex].movAletasShark = movAletasShark;
	KeyFrame[FrameIndex].movBocaShark = movBocaShark;

	KeyFrame[FrameIndex].posXfish = posXfish;
	KeyFrame[FrameIndex].rotfish = rotfish;

	KeyFrame[FrameIndex].posXmedusa = posXmedusa;

	KeyFrame[FrameIndex].posXniña = posXniña;
	KeyFrame[FrameIndex].posZniña = posZniña;
	KeyFrame[FrameIndex].movBrazoDerNiña = movBrazoDerNiña;
	KeyFrame[FrameIndex].movBrazoIzqNiña = movBrazoIzqNiña;
	KeyFrame[FrameIndex].rotPiernaDerNiña = rotPiernaDerNiña;
	KeyFrame[FrameIndex].rotPiernaIzqNiña = rotPiernaIzqNiña;


	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	//Proyecto
	movCola = KeyFrame[0].movCola;
	posXpez = KeyFrame[0].posXpez;
	rotPez = KeyFrame[0].rotPez;

	posXpez2 = KeyFrame[0].posXpez2;
	rotPezA = KeyFrame[0].rotPezA;

	posXshark = KeyFrame[0].posXshark;
	rotshark = KeyFrame[0].rotshark;
	movColaShark = KeyFrame[0].movColaShark;
	movAletasShark = KeyFrame[0].movAletasShark;
	movBocaShark = KeyFrame[0].movBocaShark;

	posXfish = KeyFrame[0].posXfish;
	rotfish = KeyFrame[0].rotfish;

	posXmedusa = KeyFrame[0].posXmedusa;

	posXniña = KeyFrame[0].posXniña;
	posZniña = KeyFrame[0].posZniña;
	movBrazoDerNiña = KeyFrame[0].movBrazoDerNiña;
	movBrazoIzqNiña = KeyFrame[0].movBrazoIzqNiña;
	rotPiernaDerNiña = KeyFrame[0].rotPiernaDerNiña;
	rotPiernaIzqNiña = KeyFrame[0].rotPiernaIzqNiña;

}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	//Proyecto
	movColaInc = (KeyFrame[playIndex + 1].movCola - KeyFrame[playIndex].movCola) / i_max_steps;
	posXpezInc = (KeyFrame[playIndex + 1].posXpez - KeyFrame[playIndex].posXpez) / i_max_steps;
	rotPezInc = (KeyFrame[playIndex + 1].rotPez - KeyFrame[playIndex].rotPez) / i_max_steps;

	posXpez2Inc = (KeyFrame[playIndex + 1].posXpez2 - KeyFrame[playIndex].posXpez2) / i_max_steps;
	rotPezAInc = (KeyFrame[playIndex + 1].rotPezA - KeyFrame[playIndex].rotPezA) / i_max_steps;

	posXsharkInc = (KeyFrame[playIndex + 1].posXshark - KeyFrame[playIndex].posXshark) / i_max_steps;
	rotsharkInc = (KeyFrame[playIndex + 1].rotshark - KeyFrame[playIndex].rotshark) / i_max_steps;
	movColaSharkInc = (KeyFrame[playIndex + 1].movColaShark - KeyFrame[playIndex].movColaShark) / i_max_steps;
	movAletasSharkInc = (KeyFrame[playIndex + 1].movAletasShark - KeyFrame[playIndex].movAletasShark) / i_max_steps;
	movBocaSharkInc = (KeyFrame[playIndex + 1].movBocaShark - KeyFrame[playIndex].movBocaShark) / i_max_steps;

	posXfishInc = (KeyFrame[playIndex + 1].posXfish - KeyFrame[playIndex].posXfish) / i_max_steps;
	rotfishInc = (KeyFrame[playIndex + 1].rotfish - KeyFrame[playIndex].rotfish) / i_max_steps;

	posXmedusaInc = (KeyFrame[playIndex + 1].posXmedusa - KeyFrame[playIndex].posXmedusa) / i_max_steps;


	posXniñaInc = (KeyFrame[playIndex + 1].posXniña - KeyFrame[playIndex].posXniña) / i_max_steps;
	posZniñaInc = (KeyFrame[playIndex + 1].posZniña - KeyFrame[playIndex].posZniña) / i_max_steps;
	movBrazoDerNiñaInc = (KeyFrame[playIndex + 1].movBrazoDerNiña - KeyFrame[playIndex].movBrazoDerNiña) / i_max_steps;
	movBrazoIzqNiñaInc = (KeyFrame[playIndex + 1].movBrazoIzqNiña - KeyFrame[playIndex].movBrazoIzqNiña) / i_max_steps;
	rotPiernaDerNiñaInc = (KeyFrame[playIndex + 1].rotPiernaDerNiña - KeyFrame[playIndex].rotPiernaDerNiña) / i_max_steps;
	rotPiernaIzqNiñaInc = (KeyFrame[playIndex + 1].rotPiernaIzqNiña - KeyFrame[playIndex].rotPiernaIzqNiña) / i_max_steps;

}

void animate(void)
{
	pez01.x = 70.0f * cos(posXpez);
	pez01.z = 70.0f * sin(posXpez);
	posXpez += 0.001f;

	rotPez -= 0.06;
	//////////////////////////////////////////

	pez02.x = 70.0f * cos(posXpez2);
	pez02.z = 70.0f * sin(posXpez2);
	posXpez2 += 0.001f;

	rotPezA -= 0.06;
	////////////////////////////////////////////

	/*tshark.x = 200.0f * cos(posXshark);
	tshark.z = 200.0f * sin(posXshark);
	tshark.y = 35.0f * cos(3*posXshark);
	posXshark += 0.005f;

	rotshark -= 0.29;*/
	////////////////////////////////////////////

	tfish.x = 70.0f * cos(posXfish);
	tfish.z = 70.0f * sin(posXfish);
	posXfish += 0.001f;

	rotfish -= 0.06;
	////////////////////////////////////////////

	tmedusa.x = 70.0f * cos(posXmedusa);
	tmedusa.z = 70.0f * sin(posXmedusa);
	tmedusa.y = 30.0f * cos(5*posXmedusa);
	posXmedusa += 0.002f;
	////////////////////////////////////////////

	tmedusa02.x = 70.0f * cos(posXmedusa);
	tmedusa02.z = 70.0f * sin(posXmedusa);
	tmedusa02.y = 30.0f * -cos(5 * posXmedusa);
	posXmedusa += 0.002f;
	////////////////////////////////////////////

	tmedusa03.x = 70.0f * cos(posXmedusa);
	tmedusa03.z = 70.0f * sin(posXmedusa);
	tmedusa03.y = 30.0f * -sin(5 * posXmedusa);
	posXmedusa += 0.002f;
	////////////////////////////////////////////

	tmedusa04.x = 70.0f * cos(posXmedusa);
	tmedusa04.z = 70.0f * sin(posXmedusa);
	tmedusa04.y = 30.0f * sin(5 * posXmedusa);
	posXmedusa += 0.002f;
	

	if (anim)
	{
		if (rot < 45.0f)
			rot += 0.2f;
		else {
			anim = false;
			anim2 = true;
		}
	}

	if (anim2)
	{
		if (rot > -45.0f)
			rot -= 0.2f;
		else {
			anim2 = false;
			anim = true;
		}
	}



	rotx = 500.0f * cos(t);
	roty = 300.0f * -sin(t);

	//lightDirection.x = 300.2f * sin(t);
	//lightDirection.z = 100.2f * sin(t);

	t += 0.05f;

	

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			//Proyecto
			movCola += movColaInc;
			posXpez += posXpezInc;
			rotPez += rotPezInc;

			posXpez2 += posXpez2Inc;
			rotPezA += rotPezAInc;

			posXshark += posXsharkInc;
			rotshark += rotsharkInc;
			movColaShark += movColaSharkInc;
			movAletasShark += movAletasSharkInc;
			movBocaShark += movBocaSharkInc;

			posXfish += posXfishInc;
			rotfish += rotfishInc;

			posXmedusa += posXmedusaInc;


			posXniña += posXniñaInc;
			posZniña += posZniñaInc;
			movBrazoDerNiña += movBrazoDerNiñaInc;
			movBrazoIzqNiña += movBrazoIzqNiñaInc;
			rotPiernaDerNiña += rotPiernaDerNiñaInc;
			rotPiernaIzqNiña += rotPiernaIzqNiñaInc;


			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	
	// ------------------------------
	glfwInit();
	

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// Aqui cargamos el skybox
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// Aqui cargamos los modelos
	Model piso("resources/objects/Piso/piso.obj");
	
	Model pecera1("resources/objects/models/tanques/pecera1/PeceraYObjetos.obj");
	Model pecera2("resources/objects/models/tanques/pecera2/pecera2yElementos.obj");
	Model pecera3("resources/objects/models/tanques/pecera3/pecera3yTroncos.obj");
	
	Model tanque("resources/objects/Edificio/tanque.obj");
	Model tanque2("resources/objects/Edificio/tanque2.obj");
	
	Model colafish("resources/objects/Peses/colafish.obj");
	Model bodyfish("resources/objects/Peses/bodyfish.obj");
	Model pez2("resources/objects/Peses/pez2.obj");

	Model AletaDerTiburon("resources/objects/shark/AletaDerTiburon.obj");
	Model AletaIzqTiburon("resources/objects/shark/AletaIzqTiburon.obj");
	Model BocaTiburon("resources/objects/shark/BocaTiburon.obj");
	Model ColaTiburon("resources/objects/shark/ColaTiburon.obj");
	Model CuerpoDelTiburon("resources/objects/shark/CuerpoDelTiburon.obj");
	Model CuerpoPrinTiburon("resources/objects/shark/CuerpoPrinTiburon.obj");
	

	//PERSONAS

	Model persona1("resources/objects/Personas/Persona1/Persona1.obj");
	Model persona2("resources/objects/Personas/Persona2/12-1.obj");
	Model persona3("resources/objects/Personas/Persona3/12-2.obj");
	Model persona4("resources/objects/Personas/Persona4/12-3.obj");
	Model persona5("resources/objects/Personas/Persona5/12-4.obj");
	Model persona6("resources/objects/Personas/Persona6/12-5.obj");
	Model persona7("resources/objects/Personas/Persona7/12-6.obj");
	Model persona8("resources/objects/Personas/Persona8/12-7.obj");
	Model persona9("resources/objects/Personas/Persona9/Persona9.obj");
	
	//Persona 11 (niña)
	Model niñaTorso("resources/objects/Personas/Persona11/Torso_cabeza/TorsoCabeza.obj");
	Model niñaBrazoDer("resources/objects/Personas/Persona11/Brazo_derecho/BrazoDerecho.obj");
	Model niñaBrazoIzq("resources/objects/Personas/Persona11/Brazo_izquierdo/BrazoIzquierdo.obj");
	Model niñaPiernaDer("resources/objects/Personas/Persona11/Pierna_derecha/PiernaDerecha.obj");
	Model niñaPiernaIzq("resources/objects/Personas/Persona11/Pierna_izquierda/PiernaIzquierda.obj");


	// PLANTAS
	Model banca("resources/objects/Banca/Banca.obj");
	Model pez3("resources/objects/Peses/Pez3/13009_Coral_Beauty_Angelfish_v1_l3.obj");

	/*
	//ARBOLES
	Model arbol1("resources/objects/Arbol/Trees/Tree1.obj");
	Model arbol2("resources/objects/Arbol/Trees/Tree2.obj");
	Model arbol3("resources/objects/Arbol/Trees/Tree3.obj");
	Model arbol4("resources/objects/Arbol/Trees/Tree4.obj");
	Model arbol5("resources/objects/Arbol/Trees/Tree5.obj");
	Model arbol6("resources/objects/Arbol/Trees/Tree6.obj");*/

	//Pinguino
	
	Model bodyPin("resources/objects/Pinguino/PinguinoCuerpo.obj");
	Model patasPin("resources/objects/Pinguino/PinguinoPatas.obj");


	Model pezGris("resources/objects/ModeloPez/fish.obj");

	Model medusa("resources/objects/Medusa/medusa.obj");

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;

		//Proyecto
		KeyFrame[i].movCola = 0;
		KeyFrame[i].posXpez = 0;
		KeyFrame[i].rotPez = 0;

		KeyFrame[i].posXpez2 = 0;
		KeyFrame[i].rotPezA = 0;

		KeyFrame[i].posXshark = 0;
		KeyFrame[i].rotshark = 0;
		KeyFrame[i].movColaShark = 0;
		KeyFrame[i].movAletasShark = 0;
		KeyFrame[i].movBocaShark = 0;

		KeyFrame[i].posXfish = 0;
		KeyFrame[i].rotfish = 0;

		KeyFrame[i].posXmedusa = 0;


		KeyFrame[i].posXniña = 0;
		KeyFrame[i].posZniña = 0;
		KeyFrame[i].movBrazoDerNiña = 0;
		KeyFrame[i].movBrazoIzqNiña = 0;
		KeyFrame[i].rotPiernaDerNiña = 0;
		KeyFrame[i].rotPiernaIzqNiña = 0;
	
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);//fuente de luz direccional (simula el sol) configuramos la direccion en la que tiene que ir dirigida la luz izq der arriba o abajo
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));//caras menos iluminadas
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));//caras mas iluminadas (color blanco)
		staticShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));//reflejos vidrios de color (brillo de color espejos) (casa y carro)

		//lightPosition FUENTE DE LUZ QUE CAMBIA DE COLOR  (YA FUNCIONA)
		staticShader.setVec3("pointLight[0].position", lightPosition);//foco que emite la luz en todas las direcciones
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));//aqui tenia 0.9 (Creamos las variables de color)
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setFloat("pointLight[0].constant", 1.08f);//potencia de intensidad menor
		staticShader.setFloat("pointLight[0].linear", 0.009f);//potencia de intensidad mayor
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);//Aumento de distancia de los rallos de luz 

		//lightPosition QUE ENCIENDE Y APAGA CON TECLA (YA FUNCIONA)
		staticShader.setVec3("pointLight[1].position", glm::vec3(0.5f, 0.5f, 0.5f));//Aqui tenia 180 y 40
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.5f, 0.5f, 0.5f));//aqui .8 y .8
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));//aqui .9 en Y
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		//lightPosition simulacion de SOL movimiento de parabola 
		staticShader.setVec3("pointLight[2].position", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setVec3("spotLight.position", glm::vec3(0.0f, 0.0f, 10.0f));
		staticShader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight.constant", 1.0f);
		staticShader.setFloat("spotLight.linear", 0.0009f);
		staticShader.setFloat("spotLight.quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Proyecto
		
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		//------------------------------------------------------------

		//-----------------piso----------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.0f, 0.30f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);



		//Personas

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona1.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona2.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(200.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona3.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona4.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(400.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona5.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(500.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona6.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(600.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona7.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(700.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona8.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(800.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.065f, 0.065f, 0.065f));
		staticShader.setMat4("model", model);
		persona9.Draw(staticShader);

								
									// ANIMACION NIÑA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(900.0f, 0.0f, 480.0f));
		model = glm::translate(model, glm::vec3(posXniña, 0, posZniña));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		niñaTorso.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBrazoDerNiña), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		niñaBrazoDer.Draw(staticShader);

		
		model = glm::translate(tmp, glm::vec3(-1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBrazoIzqNiña), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		niñaBrazoIzq.Draw(staticShader);

		
		model = glm::translate(tmp, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPiernaDerNiña), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		niñaPiernaDer.Draw(staticShader);

		
		model = glm::translate(tmp, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPiernaIzqNiña), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		niñaPiernaIzq.Draw(staticShader);


		/*

		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabeza.Draw(staticShader);


		*/








		//Banca

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(550.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(800.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1050.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

	
		/*

										//Arboles

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setMat4("model", model);
		//arbol1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setMat4("model", model);
		//arbol2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(200.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setMat4("model", model);
		//arbol3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setMat4("model", model);
		//arbol4.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(400.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setMat4("model", model);
		//arbol5.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(500.0f, 0.0f, 480.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setMat4("model", model);
		//arbol6.Draw(staticShader);*/


		
		//Pinguino

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(950.0f, 0.0f, -360.0f));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		bodyPin.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(950.0f, 0.0f, -360.0f));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);












		//----------------tanques--------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -100.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f,15.0f,20.0f));
		staticShader.setMat4("model", model);
		pecera1.Draw(staticShader);



		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(60.0f, 0.0f, -150.0f));
		model = glm::translate(model, glm::vec3(-490.0f, 0.0f, 1150.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(20.0f, 15.0f, 20.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 45.0f, 100.0f));
		staticShader.setMat4("model", model);
		pecera2.Draw(staticShader);



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -350.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(120.0f, 15.0f, 120.0f));
		staticShader.setMat4("model", model);
		pecera3.Draw(staticShader);


		/*

		// PRIMERA ANIMACION PEZ
		
		// Body 

		//model = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-705.0f, 50.0f, 300.0f));
		//model = glm::translate(model, glm::vec3(-200.0f, 14.0f, 250.0f));
		//model = glm::translate(model, glm::vec3(posXpez , 0, 0));
		model = glm::translate(model, pez01);
		//tmp = model = glm::rotate(model, glm::radians(rotPez), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		bodyfish.Draw(staticShader);

		// Cola
		//model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(movCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 13.0f));
		staticShader.setMat4("model", model);
		colafish.Draw(staticShader);

		// Body 02
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-705.0f, 100.0f, 290.0f));
		model = glm::translate(model, pez01);
		tmp = model = glm::rotate(model, glm::radians(rotPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		bodyfish.Draw(staticShader);

		// Cola 02
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(movCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 13.0f));
		staticShader.setMat4("model", model);
		colafish.Draw(staticShader);

		// Body 03
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-705.0f, 150.0f, 280.0f));
		model = glm::translate(model, pez01);
		tmp = model = glm::rotate(model, glm::radians(rotPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		bodyfish.Draw(staticShader);

		// Cola 03
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(movCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 13.0f));
		staticShader.setMat4("model", model);
		colafish.Draw(staticShader);

		// Body 04
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-705.0f, 200.0f, 270.0f));
		model = glm::translate(model, pez01);
		tmp = model = glm::rotate(model, glm::radians(rotPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		bodyfish.Draw(staticShader);

		// Cola 04
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(movCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 13.0f));
		staticShader.setMat4("model", model);
		colafish.Draw(staticShader);
		
									//ANIMACION TIBURON
		
		// Boca Tiburon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(950.0f, 50.0f, -360.0f));
		model = glm::translate(model, tshark);
		tmp = model = glm::rotate(model, glm::radians(rotshark), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movBocaShark), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		BocaTiburon.Draw(staticShader);

	
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		CuerpoDelTiburon.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		CuerpoPrinTiburon.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(0.0f,0.0f,1.0f));
		model = glm::rotate(model, glm::radians(movColaShark), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		ColaTiburon.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movAletasShark), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		AletaDerTiburon.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movAletasShark), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		AletaIzqTiburon.Draw(staticShader);


		// SEGUNDA ANIMACION PEZ

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-700.0f, 20.0f, 350.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-700.0f, 70.0f, 340.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-700.0f, 120.0f, 330.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-700.0f, 170.0f, 320.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);


		//ANIMACION PEZ GRIS
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-715.0f, 80.0f, 400.0f));
		model = glm::translate(model, tfish);
		model = glm::rotate(model, glm::radians(rotfish), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		pezGris.Draw(staticShader);

		*/

		//ANIMACION PEZ 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-730.0f, 100.0f, 400.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

				/*					//ANIMACION MEDUSAS

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(950.0f, 40.0f, -360.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 02
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(930.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 03
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(910.0f, 40.0f, -360.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 04
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(890.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 05
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(870.0f, 40.0f, -360.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 06
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(850.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 07
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(830.0f, 40.0f, -360.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 08
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(810.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 09
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(790.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 10
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(770.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 11
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(750.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 12
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(730.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 13
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(710.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 14
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(690.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 15
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(670.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 16
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(650.0f, 50.0f, -360.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);*/



		//------------------------------------------------------------------------------

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-180.0f, 25.0f, -230.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 15.0f, 20.0f));
		staticShader.setMat4("model", model);
		tanque2.Draw(staticShader);

		//---------------peces-----------------------------

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
	/*
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
		
		*/


	//PROYECTO
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		movCola--;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		movCola++;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotPez--;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotPez++;

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotPezA--;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotPezA++;

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotshark--;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		rotshark++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		movColaShark --;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		movColaShark ++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		movAletasShark -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		movAletasShark += 0.2f;

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		movBocaShark -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		movBocaShark += 0.2f;

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		rotfish--;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		rotfish++;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posXniña++;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		posXniña--;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		posZniña--;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		posZniña++;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		movBrazoDerNiña += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		movBrazoDerNiña -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		movBrazoIzqNiña += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		movBrazoIzqNiña -= 0.2f;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		rotPiernaDerNiña += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		rotPiernaDerNiña -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		rotPiernaIzqNiña += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		rotPiernaIzqNiña -= 0.2f;

	

	//TECLAS PARA QUE CAMBIE DE COLOR
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		colorB++;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		colorG++;
	//TECLAS PARA QUE ENCIENDA DE UN SOLO COLOR Y SE APAGUE 
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		colorR += 5.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		colorR = 0.0f;
	//TECLAS DE MOVIMIENTO DONDE SOLO ENCIENDE Y APAGA UN SOLO COLOR 
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)//Aqui movemos la luz en el eje Y
		movy++;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		movy--;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}