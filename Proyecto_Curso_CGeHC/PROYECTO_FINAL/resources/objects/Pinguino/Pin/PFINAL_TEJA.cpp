/*---------------------------------------------------------*/
/* ------------ PROYECTO_FINAL_TEORIA   -----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno: Tapia Escobar José Alejandro---------------*/
/*------------- No. Cuenta: 418046832 ---------------*/
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

// posiciones

float
//PINGUNO
movPinx = 0.0f,
movPiny = 0.0f,
movPinz = 0.0f,
//PEZ
movPx = 0.0f,
movPz = 0.0f,
movPy = 0.0f,


orienta2 = 0.0f,
orienta = 0.0f;//orienta la direccion hacia donde ve el auto 


bool	rec1 = true,
rec2 = false,
rec3 = false,
rec4 = false,
rec5 = true,
rec6 = false,
rec7 = false,
rec8 = false,
animacion = false,
anim = false,
anim1 = false,
anim2 = false,
anim3 = false,
anim4 = false,
anim5 = false,
anim6 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f;

//Colores declarados 
float colorR = 2.4f,
colorG = 0.86f,
colorB = 1.5f;

//float rotx = (-20.0f, 0.0f, 0.0f);
//float roty = (0.0f, 40.0f, 0.0f);
//float movy = 0.0f;


float rot = 0.0f,
	  rot2 = 0.0f;//Se agrega

float t = 0.0f;

#define MAX_FRAMES 9
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

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)
{
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

			i_curr_steps++;
		}
	}



	//Cola Pez (Logica de Animacion)
	if(anim1){
		if (rot <= 25.0f)
			rot += 0.8f;
		else {
			anim1 = false;
			anim2 = true;
		}
	}

	if (anim2) {
		if (rot > -25.0f)
			rot -= 0.8f;
		else {
			anim2 = false;
			anim1 = true;
		}
	}


	//Recorrido pez (Logica de animacion)
	if (anim3) {

		if (rec1) {
			movPx = 0.0f;
			movPz += 0.4f;
			orienta = 0.0f;
			if (movPz >= 80.0f) {
				rec1 = false;
				rec2 = true;
			}
		}

		if (rec2) 
		{
			orienta = -90.0f;//-90
			movPz = 80.0f;
			movPx -= 0.4f;
			if (movPx <= -80.0f) {
				rec2 = false;
				rec3 = true;
			}
		}

		if (rec3) 
		{
			movPx = -80.0f;
			movPz -= 0.4f;
			orienta = 180.0f;
			if (movPz <= -80.0f) {
				rec3 = false;
				rec4 = true;
			}
		}

		if (rec4) {
			movPx += 0.4f;
			movPz = -80.0f;
			orienta = 90.0f;
			if (movPx >= 0.0f) {
				rec4 = false;
				rec1 = true;
			}
		}

	}

	//=========================================================================

	//Ping (Logica de Animacion)
	if (anim4) {
		if (rot2 <= 11.0f)
			rot2 += 0.6f;
		else {
			anim4 = false;
			anim5 = true;
		}
	}

	if (anim5) {
		if (rot2 > -11.0f)
			rot2 -= 0.6f;
		else {
			anim5 = false;
			anim4 = true;
		}
	}

	//Recorrido Pin (Logica de animacion)
	if (anim6) {

		if (rec5) {
			movPinx = 0.0f;
			movPinz += 0.4f;
			orienta2 = 0.0f;
			if (movPinz >= 90.0f) {
				rec5 = false;
				rec6 = true;
			}
		}

		if (rec6) 
		{
			orienta2 = -90.0f;//-90
			movPinz = 90.0f;
			movPinx -= 0.4f;
			if (movPinx <= -90.0f) {
				rec6 = false;
				rec7 = true;
			}
		}

		if (rec7) 
		{
			movPinx = -90.0f;
			movPinz -= 0.4f;
			orienta2 = 180.0f;//aqui era en 180 pero sale mal
			if (movPinz <= -90.0f) {
				rec7 = false;
				rec8 = true;
			}
		}

		if (rec8) {
			movPinx += 0.4f;
			movPinz = -90.0f;
			orienta2 = 90.0f;//aqui estaba en 90 pero estaba mal
			if (movPinx >= 0.0f) {
				rec8 = false;
				rec5 = true;
			}
		}

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
	
	Model piso("resources/objects/Maqueta/pisofin.obj");//
	Model estanque("resources/objects/Maqueta/estanque.obj");//
	Model igloo("resources/objects/Maqueta/igloo.obj");//
	Model tortugas("resources/objects/Maqueta/estanqueT.obj");//

	Model bodyp("resources/objects/Maqueta/bodyP.obj");
	Model patasp("resources/objects/Maqueta/patas.obj");

	Model bodyp2("resources/objects/Maqueta/bodyP.obj");
	Model patasp2("resources/objects/Maqueta/patas.obj");

	Model cola("resources/objects/Peses/colafish.obj");
	Model body("resources/objects/Peses/bodyfish.obj");

	Model cola2("resources/objects/Peses/cola2.obj");
	Model body2("resources/objects/Peses/body2.obj");

	Model cola3("resources/objects/Peses/cola3.obj");
	Model body3("resources/objects/Peses/body3.obj");

	/*Model cola4("resources/objects/Peses/cola4.obj");
	Model body4("resources/objects/Peses/body4.obj");*/

	






	//Model pecera1("resources/objects/Maqueta/pecera1.obj");//Pecera pequeña
	//Model pecera2("resources/objects/Maqueta/estanque2.obj");//Pecera tiburones 
	//Model estanque("resources/objects/Maqueta/estanque.obj");//Estanque tortugas
	//Model mc("resources/objects/Maqueta/mc.obj");//Mc´Donals
	//Model silla("resources/objects/Maqueta/sillas.obj");//Mesas y sillas
	//Model snake("resources/objects/Maqueta/snake.obj");//Serpentario
	//Model iglo("resources/objects/Maqueta/igloo.obj");//Iglo
	//Model coco("resources/objects/Maqueta/cocodrilo.obj");//cocodrilo
	
	//Model colafish("resources/objects/Peses/colafish.obj");
	//Model bodyfish("resources/objects/Peses/bodyfish.obj");
	//Model pez2("resources/objects/Peses/pez2.obj");
	


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
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

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Proyecto

		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		//------------------------------------------------------------

		//-----------------PROPUESTA_ESCENARIOS----------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -15.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		estanque.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		igloo.Draw(staticShader);

	/*	model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		tortugas.Draw(staticShader);*/

		//-------------------------------------------------

		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1050.0f, 67.0f, -900.0f));
		model = glm::translate(model, glm::vec3(15.0f + movPinx, movPiny, movPinz));
		tmp = model = glm::rotate(model, glm::radians(orienta2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		patasp.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bodyp.Draw(staticShader);	


		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1000.0f, 67.0f, -850.0f));
		model = glm::translate(model, glm::vec3(15.0f + movPinx, movPiny, movPinz));
		tmp = model = glm::rotate(model, glm::radians(orienta2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		patasp2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bodyp2.Draw(staticShader);


		//--------PEEEEZ         1-----------------------------
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//90
		////model = glm::translate(model, glm::vec3(-3200.0f, -100.0f, -300.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f + movPx, movPy, movPz));//15
		//tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(45.0f, 45.0f, 45.0f));
		//staticShader.setMat4("model", model);
		//body.Draw(staticShader);//falta hacer que se mueva la cola del pez 

		//model = glm::mat4(1.0f);
		//model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(45.0f, 45.0f, 45.0f));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//cola.Draw(staticShader);//falta hacer que se mueva la cola del pez


		//--------PEEEEZ         2-----------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1100.0f, 290.0f, 1100.0f));
		model = glm::translate(model, glm::vec3(15.0f + movPx, movPy, movPz));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		staticShader.setMat4("model", model);
		body2.Draw(staticShader);//falta hacer que se mueva la cola del pez 

		//model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 13.0f, 1.0f));//Ajustes de la cola 
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cola2.Draw(staticShader);//falta hacer que se mueva la cola del pez

		//--------PEEEEZ         3-----------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-950.0f, 100.0f, 800.0f));
		model = glm::translate(model, glm::vec3(15.0f + movPx, movPy, movPz));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		staticShader.setMat4("model", model);
		body3.Draw(staticShader);//falta hacer que se mueva la cola del pez 

		//model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cola3.Draw(staticShader);//falta hacer que se mueva la cola del pez

		
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

	//TECLAS PARA QUE CAMBIE DE COLOR
	/*if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		colorB++;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		colorG++;*/
	//TECLAS PARA QUE ENCIENDA DE UN SOLO COLOR Y SE APAGUE 
	/*if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		colorR += 5.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		colorR = 0.0f;*/
	//TECLAS DE MOVIMIENTO DONDE SOLO ENCIENDE Y APAGA UN SOLO COLOR 
	//if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)//Aqui movemos la luz en el eje Y
	//	movy++;
	//if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	//	movy--;

	/*if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		if (movPin_z <= 0.0f) {
			movPin_z += 0.3;
		}
		else {
			movPin_z -= 0.3;
		}*/

	//AQUI SE INICIA LA ANIMACION 
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)//pingu
	//	animacion ^= true;

	//if (key == GLFW_KEY_8 && action == GLFW_PRESS)//coco
	//	anim ^= true;

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)//Pez
		anim1 ^= true;

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)//Pez
		anim3 ^= true;

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)//Pin
		anim4 ^= true;

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)//Pin
		anim6 ^= true;




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