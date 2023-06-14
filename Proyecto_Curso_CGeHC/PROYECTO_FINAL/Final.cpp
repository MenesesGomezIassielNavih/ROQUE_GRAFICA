/*---------------------------------------------------------*/
/* ----------------  Proyecto Final              ----------*/
/*-----------------       -------------------------*/
/*---------------------------------------------------------*/

#include <Windows.h>
#include <mmsystem.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		3
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <time.h>
#include <math.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
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
#pragma comment(lib, "winmm.lib")
//CARGA FBX
#include <assimp/importer.hpp>
#include <assimp/scene.h>
//MATERIALES
#include <material.h>
//FIGURAS 2D
#include<Mesh_1.h>
#include <Circle.h>
#include <Triangle.h>
#include <Plane.h>
#include <Shader1.h>
#include <Vertex1.h>
//AUDIO
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")


//GLOBALES
bool sound = true;
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
float ratio;
int slices;
int stacks;

//OLAS
float sineTime = 0.0f;
float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

//GLuint VAO, VBO, IBO;
GLuint VAO[50], VBO[50], IBO[50];
GLuint indexCount[50];
//GLuint indexCount[6];

typedef struct _VertexColor {
	_VertexColor() {
	}
	_VertexColor(glm::vec3 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}
	glm::vec3 position;
	glm::vec3 color;
} VertexColor;

std::vector<VertexColor> vertexC;
std::vector<GLuint> index;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

//Declaracion para objetos con textura
void myData(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);


// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;
void getResolution(void);

//MALLAS para 2D
Mesh_1* meshCircle;
Mesh_1* meshTriangle;
Mesh_1* meshPlane;

bool activaSkyboxDia = true;

float door_offset = 0.0f;

///////////////////TEXTURIZADO
unsigned int
					t_brick_jpg,
					t_wood_png,
					t_ventana_jpg,
					t_puerta_jpg,
					t_techo_jpg,
					t_azul_png;

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}
	stbi_image_free(data);
}

void LoadTextures()
{
	t_brick_jpg = generateTextures("resources/Texturas1/bricks.jpg", 0);
	t_wood_png = generateTextures("resources/Texturas1/wood01.png", 0);
	t_ventana_jpg = generateTextures("resources/Texturas1/azulVentana.jpg", 0);
	t_puerta_jpg = generateTextures("resources/Texturas1/maderaPuerta.jpg", 0);
	t_techo_jpg = generateTextures("resources/Texturas1/techo.jpg", 0);
	t_azul_png = generateTextures("resources/Texturas1/azulTransparente.png", 1);
}



void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//texture coords
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		6.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		6.0f, 5.0f,	//V5
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0
		-0.5f, 0.5f, 0.5f,		0.0f, 5.0f,	//V4
		0.5f, 0.5f, 0.5f,		6.0f,5.0f,	//V5

		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	6.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		6.0f, 5.0f,	//V7
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 5.0f,	//V6
		-0.5f, 0.5f, -0.5f,		6.0f, 5.0f,	//V7

		-0.5f, 0.5f, 0.5f,		6.0f, 5.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 5.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	//V3
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	//V3
		-0.5f, 0.5f, 0.5f,		6.0f, 5.0f,	//V4
		-0.5f, -0.5f, 0.5f,		6.0f, 0.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 5.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		6.0f, 0.0f,	//V2
		0.5f, 0.5f, 0.5f,		0.0f, 5.0f,	//V5
		0.5f, 0.5f, -0.5f,		6.0f, 5.0f,	//V6
		0.5f, -0.5f, -0.5f,		6.0f, 0.0f,	//V2

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V6
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,	//V4
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V6

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V1
	};


	glGenVertexArrays(1, &VAO[8]);
	glGenBuffers(1, &VBO[8]);
	//glGenBuffers(2, EBO);


	//PARA CUBO
	glBindVertexArray(VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void myDataRenderizar()
{
	glBindVertexArray(VAO[8]);
	glBindTexture(GL_TEXTURE_2D, t_brick_jpg);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

///////////////////   FIN    TEXTURIZADO
void cuboSencillo()
{
	GLfloat verticesCubo[] = {
		//Position				//texture coords
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		0.5f, 0.5f, 0.5f,		1.0f,1.0f,	//V5

		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7

		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	//V3
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	//V3
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V4
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V6
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V6
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,	//V4
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V6

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V1
	};


	glGenVertexArrays(1, &VAO[21]);
	glGenBuffers(1, &VBO[21]);
	//glGenBuffers(2, EBO);


	//PARA CUBO
	glBindVertexArray(VAO[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[21]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void cuboSencilloRenderizar()
{
	glBindVertexArray(VAO[21]);
	glBindTexture(GL_TEXTURE_2D, t_brick_jpg);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

//PRIMITIVAS INICIALIZACION
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front                 // Coordenadas de textura
		-0.5f, -0.5f,  0.5f,     // 0.33f, 0.33, 0.33f,
		0.5f, -0.5f,  0.5f,      // 0.33f, 0.33, 0.33f,
		0.5f,  0.5f,  0.5f,		 // 0.33f, 0.33, 0.33f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	indexCount[0] = 36;
	glGenVertexArrays(1, &VAO[0]); //generar 1 VAO
	glBindVertexArray(VAO[0]);//asignar VAO

	glGenBuffers(1, &IBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubo_indices) * indexCount[0], cubo_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubo_vertices) * 8, cubo_vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearCuboRenderizar() {
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearPiramide()
{
	unsigned int piramide_indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat piramide_vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	indexCount[1] = 12;
	glGenVertexArrays(1, &VAO[1]); //generar 1 VAO
	glBindVertexArray(VAO[1]);//asignar VAO

	glGenBuffers(1, &IBO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(piramide_indices) * indexCount[1], piramide_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(piramide_vertices) * 4, piramide_vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearPiramideRenderizar() {
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4
	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	indexCount[2] = 18;
	glGenVertexArrays(1, &VAO[2]); //generar 1 VAO
	glBindVertexArray(VAO[2]);//asignar VAO

	glGenBuffers(1, &IBO[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(piramidecuadrangular_indices) * indexCount[2], piramidecuadrangular_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(piramidecuadrangular_vertices) * 5, piramidecuadrangular_vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearPiramideCuadrangularRenderizar() {
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearSphere(float ratio, int slices, int stacks)
{
	std::vector<VertexColor> vertexC;
	std::vector<GLuint> index;

	vertexC.resize(((slices + 1) * (stacks + 1)));
	index.resize((slices * stacks + slices) * 6);
	for (int i = 0; i <= stacks; ++i) {
		float V = i / (float)stacks;
		float phi = V * M_PI;
		for (int j = 0; j <= slices; ++j) {
			float U = j / (float)slices;
			float theta = U * M_PI * 2.0;
			float X = cos(theta) * sin(phi);
			float Y = cos(phi);
			float Z = sin(theta) * sin(phi);
			vertexC[i * (slices + 1) + j].position = ratio
				* glm::vec3(X, Y, Z);
			vertexC[i * (slices + 1) + j].color = glm::sphericalRand(1.0);
		}
	}
	for (int i = 0; i < slices * stacks + slices; ++i) {
		index[i * 6] = i;
		index[i * 6 + 1] = i + slices + 1;
		index[i * 6 + 2] = i + slices;
		index[i * 6 + 3] = i + slices + 1;
		index[i * 6 + 4] = i;
		index[i * 6 + 5] = i + 1;
	}
	
	indexCount[3] = index.size();
	glGenVertexArrays(1, &VAO[3]);
	glGenBuffers(1, &VBO[3]);
	glGenBuffers(1, &IBO[3]);
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	size_t stride;
	size_t offset1 = 0;
	size_t offset2 = 0;
	size_t offset3 = 0;
	glBufferData(GL_ARRAY_BUFFER, vertexC.size() * sizeof(glm::vec3) * 2,
		vertexC.data(),
		GL_STATIC_DRAW);
	stride = sizeof(vertexC[0]);
	offset1 = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);
	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO
}

void CrearSphereRenderizar() {
	glBindVertexArray(VAO[3]);
	glDrawElements(GL_TRIANGLES, indexCount[3], GL_UNSIGNED_INT,
		(GLvoid*)(sizeof(GLuint) * 0));
	glBindVertexArray(0);
}


void CrearCilindro(int res, float R) {
	//constantes utilizadas en los ciclos for
	int n, i;
	//cÃ¡lculo del paso interno en la circunferencia y variables que almacenarÃ¡n cada coordenada de cada vÃ©rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vÃ©rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el cÃ­rculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}
	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	//Se generan los indices de los vÃ©rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);
	indexCount[4] = indices.size();
	glGenVertexArrays(1, &VAO[4]); //generar 1 VAO
	glBindVertexArray(VAO[4]);//asignar VAO

	glGenBuffers(1, &IBO[4]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaÃ±o, los datos y en este caso es estÃ¡tico pues no se modificarÃ¡n los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearCilindroRenderizar()
{
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[4]);
	glDrawElements(GL_TRIANGLE_FAN, indexCount[4], GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearCono(int res, float R) {
	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarÃ¡n cada coordenada de cada vÃ©rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;
	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);
	for (i = 0; i < res + 2; i++) indices.push_back(i);

	indexCount[5] = indices.size();
	glGenVertexArrays(1, &VAO[5]); //generar 1 VAO
	glBindVertexArray(VAO[5]);//asignar VAO

	glGenBuffers(1, &IBO[5]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaÃ±o, los datos y en este caso es estÃ¡tico pues no se modificarÃ¡n los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearConoRenderizar() {
	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[5]);
	glDrawElements(GL_TRIANGLE_FAN, indexCount[5], GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearTorus(float R, float r, int nR, int nr)
{

	vector <float> vertices;
	vector<unsigned int> indices;

	int i = 0.0f, j = 0.0f, k = 0.0f;
	int _nR = 0, _nr = 0;
	float _R = R, _r = r;
	_nR = nR;
	_nr = nr;

	float v = 0.0f;
	float du = (2 * PI * _R) / _nR;
	float dv = (2 * PI * _r) / _nr;
	float u = 0.0f;

	float uu = 0.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;

	for (j = 0; j <= _nR; j++)
	{
		for (i = 0; i <= _nr; i++)
		{
			x = (_R + _r * cos(v)) * cos(u);
			y = _r * sin(v);
			z = (_R + _r * cos(v)) * sin(u);;

			// agrega vertice 
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			v += dv;
		}
		u += du;
	}

	//generacion de indices

	for (i = 0; i < _nR * _nr; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 12);
		indices.push_back(i + 1);
		indices.push_back(i + 12);
		indices.push_back(i + 13);
		indices.push_back(i + 1);
	}

	indexCount[6] = indices.size();
	glGenVertexArrays(1, &VAO[6]); //generar 1 VAO
	glBindVertexArray(VAO[6]);//asignar VAO

	glGenBuffers(1, &IBO[6]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[6]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(vertices[0]*3), 0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0] * 6), (void*)(sizeof(vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearTorusRenderizar()
{
	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[6]);
	glDrawElements(GL_TRIANGLES, indexCount[6], GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void CrearTetera()
{
	vector <float> vertices;
	vector<unsigned int> indices;

	int i = 0.0f, j = 0.0f, k = 0.0f;
	int _nR = 0, _nr = 0;
	float R = 4.0f, r = 0.0f;
	_nR = 100;
	_nr = 50;

	float v = 0.0f;
	float du = (2 * PI) / _nR;
	float dv = (2 * PI) / _nr;
	float u = 0.0f;

	float uu = 0.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	//Para el vertice del centro de la base

	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	//ingresando los vertices de la base

	R = 4.0f;
	y = 0.0f;

	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}

	//ingresando los indices de la triangulacion de la base
	int n = 0;
	for (i = 0; i < 12 * _nr; i++)
	{
		indices.push_back(n);
		indices.push_back(n + i + 1);
		indices.push_back(n + i + 2);
	}
	//ingresando los vertices de los aros de cada nivel
	R = 5.0f;
	y = 1.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 5.0f;
	y = 2.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 6.0f;
	y = 3.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 6.0f;
	y = 4.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 5.5f;
	y = 5.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 5.3f;
	y = 6.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 5.2f;
	y = 7.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 5.1f;
	y = 8.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 4.8f;
	y = 9.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	R = 4.3f;
	y = 10.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);
		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}

	R = 3.0f;
	y = 11.0f;
	u = 0.0f;
	for (i = 0; i <= _nr; i++)
	{
		x = R * cos(v);
		z = R * sin(v);

		// agrega vertice 
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		v += dv;
	}
	for (i = 1; i < 12 * _nr; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 50);
		indices.push_back(i + 1);
		indices.push_back(i + 50);
		indices.push_back(i + 51);
		indices.push_back(i + 1);

	}

	indexCount[13] = indices.size();
	glGenVertexArrays(1, &VAO[13]); //generar 1 VAO
	glBindVertexArray(VAO[13]);//asignar VAO

	glGenBuffers(1, &IBO[13]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[13]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(vertices[0]*3), 0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0] * 6), (void*)(sizeof(vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearTeteraRenderizar()
{
	glBindVertexArray(VAO[13]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[13]);
	glDrawElements(GL_TRIANGLES, indexCount[13], GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void CrearAsaTetera()
{
	vector <float> vertices;
	vector<unsigned int> indices;

	int i = 0.0f, j = 0.0f, k = 0.0f;
	int _nR = 0, _nr = 0;
	float _R = 3.0, _r = 0.5;
	_nR = 36;
	_nr = 12;

	float v = 0.0f;
	float du = (2 * PI) / _nR;
	float dv = (2 * PI) / _nr;
	float u = 0.0f;

	float uu = 0.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;


	for (j = 0; j <= _nR; j++)
	{
		if (j == _nR / 3 * 2)
			break;
		for (i = 0; i <= _nr; i++)
		{
			x = (_R + _r * cos(v)) * cos(u);
			y = _r * sin(v);
			z = (_R + _r * cos(v)) * sin(u);;

			// agrega vertice 
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			v += dv;
		}
		u += du;
	}

	for (size_t i = 0; i <= _nR * _nr; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 12);
		indices.push_back(i + 1);
		indices.push_back(i + 12);
		indices.push_back(i + 13);
		indices.push_back(i + 1);

	}

	indexCount[14] = indices.size();
	glGenVertexArrays(1, &VAO[14]); //generar 1 VAO
	glBindVertexArray(VAO[14]);//asignar VAO

	glGenBuffers(1, &IBO[14]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[14]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(vertices[0]*3), 0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0] * 6), (void*)(sizeof(vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void CrearAsaTeteraRenderizar()
{

	glBindVertexArray(VAO[14]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[14]);
	glDrawElements(GL_TRIANGLES, indexCount[14] / 3 * 2, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void CrearBoquillaTetera()
{
	vector <float> vertices;
	vector<unsigned int> indices;

	int i = 0.0f, j = 0.0f, k = 0.0f;
	int _nR = 0, _nr = 0;
	float R = 4.0f, r = 1.2f;
	_nR = 100;
	_nr = 50;

	float v = 0.0f;
	float du = (2 * PI) / _nR;
	float dv = (2 * PI) / _nr;
	float u = 0.0f;

	float uu = 0.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;

	for (j = 0; j <= _nR; j++)
	{

		for (i = 0; i <= _nr; i++)
		{
			z = (R + r * cos(v)) * cos(u);
			y = r * sin(v);
			x = (R + r * cos(v)) * sin(u);
			//z = (R + r * cos(v)) * sin(u);;
			// agrega vertice 
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			v += dv;
		}
		u += du;
	}
	for (i = 0; i <= _nR * _nr; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 50);
		indices.push_back(i + 1);
		indices.push_back(i + 50);
		indices.push_back(i + 51);
		indices.push_back(i + 1);
	}
	indexCount[16] = indices.size();
	glGenVertexArrays(1, &VAO[16]); //generar 1 VAO
	glBindVertexArray(VAO[16]);//asignar VAO

	glGenBuffers(1, &IBO[16]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[16]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(vertices[0]*3), 0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0] * 6), (void*)(sizeof(vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CrearBoquillaTeteraRenderizar()
{
	glBindVertexArray(VAO[16]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[16]);
	glDrawElements(GL_TRIANGLES, indexCount[16] / 3 - 3, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



// camera



Camera camera(glm::vec3(0.0f, 250.0f, 90.0f));
Camera camera2(glm::vec3(1500.0f, 1300.0f, 2500.0f));
Camera camera3(glm::vec3(0.0f, 0.0f, 0.0f));

glm::vec3 position(0.0f, 0.0f, 0.0f);
glm::vec3 forwardView(0.0f, -1.0f, 0.0f);
//glm::vec3 forwardView(-0.47f, -0.41f, -0.78f);
float     scaleV = 4.0f;
float rotateCharacter = 0.0f;


bool    activeCamera = true,
activeCamera2 = false,
activeCamera3 = false;


float MovementSpeed = 0.4f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);
double myVariable = 0.0;   //sirve para el SOL
double myVariable2 = 0.0f;
double myVariable3 = 0.0f;
double myVariable4 = 0.0f;
double myVariable5 = 0.0f;

glm::vec3 pez01(0.0f, -1.0f, -1.0f);
glm::vec3 pez02(0.0f, -1.0f, -1.0f);
glm::vec3 tfish(0.0f, -1.0f, -1.0f);
glm::vec3 tfish3(0.0f, -1.0f, -1.0f);

glm::vec3 pez011(0.0f, -1.0f, -1.0f);
glm::vec3 pez021(0.0f, -1.0f, -1.0f);
glm::vec3 tfish1(0.0f, -1.0f, -1.0f);
glm::vec3 tfish31(0.0f, -1.0f, -1.0f);

glm::vec3 tshark(0.0f, -1.0f, -1.0f);
glm::vec3 tshark01(0.0f, -1.0f, -1.0f);
glm::vec3 tshark02(0.0f, -1.0f, -1.0f);
glm::vec3 tshark03(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa02(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa03(0.0f, -1.0f, -1.0f);
glm::vec3 tmedusa04(0.0f, -1.0f, -1.0f);



//variables mov   guinos
float movPingu_x = 0.0f,
movPingu_y = 0.0f,
movPingu_z = 0.0f,
orientaPingu = 0.0f,
movPingu_x2 = 0.0f,
movPingu_y2 = 0.0f,
movPingu_z2 = 30.0f,
orientaPingu2 = 0.0f,
animalaspingu = 0.0f,
alaspingu = 0.0f,
animpataspingu = 0.0f,
pataspingu = 0.0f;



//animaciones pinguinos
bool animacionP = true,
	reco1 = true,
	reco2 = false,
	reco3 = false,
	reco4 = false,
	recoo5 = false,
	recoo6 = false,
	recoo7 = false,
animacionP2 = true,
	reco5 = true,
	reco6 = false,
	reco7 = false,
	reco8 = false,
	reco9 = false,
	reco10 = false,
	reco11 = false;


//variables mov   foca
float	movFoca_x = 0.0f,
		movFoca_y = 0.0f,
		movFoca_z = 0.0f,
		orientaFoca = 0.0f,
		movFoca_x2 = 0.0f,
		movFoca_y2 = 0.0f,
		movFoca_z2 = 0.0f,
		orientaFoca2 = 0.0f,
		anim_aletasSupFoca = 0.0f,
		aletasSupFoca = 0.0f,
		anim_aletasInfFoca = 0.0f,
		aletasInfFoca = 0.0f;


bool animacionFoca = true,
		recoF1 = true,
		recoF2 = false,
		recoF3 = false,
		recoF4 = false,
		recooF5 = false,
		recooF6 = false,
		recooF7 = false,
		animacionFoca2 = true,
		recoF5 = true,
		recoF6 = false,
		recoF7 = false,
		recoF8 = false,
		recoF9 = false,
		recoF10 = false,
		recoF11 = false;



//variables mov   Ballena
float	movBallena_x = 0.0f,
		movBallena_y = 0.0f,
		movBallena_z = 0.0f,
		orientaBallena = 0.0f,
		movBallena_x2 = 0.0f,
		movBallena_y2 = 0.0f,
		movBallena_z2 = 0.0f,
		orientaBallena2 = 0.0f,
		orientaBallena3 = 0.0f,
		anim_aletasSupBallena = 0.0f,
		aletasSupBallena = 0.0f,
		anim_aletasInfBallena = 0.0f,
		aletasInfBallena = 0.0f;


bool animacionBallena = true,
		recoB1 = true,
		recoB2 = false,
		recoB3 = false,
		recoB4 = false,
		recooB5 = false,
		recooB6 = false,
		recooB7 = false,
		animacionBallena2 = true,
		recoB5 = true,
		recoB6 = false,
		recoB7 = false,
		recoB8 = false,
		recoB9 = false,
		recoB10 = false,
		recoB11 = false,
		recoB12 = false,
		recoB13 = false,
		recoB14 = false,
		recoB15 = false,

		rotaB5 = true,
		rotaB6 = false,
		rotaB7 = false,
		rotaB8 = false;

		


//variables movimiento Shopper
float movShopper_x = 0.0f,
movShopper_y = 0.0f,
movShopper_z = 0.0f,
orientaShopper = 0.0f,
movShopper_x2 = 0.0f,
movShopper_y2 = 0.0f,
movShopper_z2 = 0.0f,
orientaShopper2 = 0.0f,
brazoDerShopper = 0.0f,
brazoIzqShopper = 0.0f,
pieDerShopper = 0.0f,
pieIzqShopper = 0.0f;

bool	animBrazoDerShopper = false,
		animBrazoIzqShopper = false,
		animPieDerShopper = false,
		animPieIzqShopper = false;




//animaciones Shopper
bool animacionSH = true,
recoSH1 = true,
recoSH2 = false,
recoSH3 = false,
recoSH4 = false,
recooSH5 = false,
recooSH6 = false,
recooSH7 = false,
animacionSH_1 = true,
recoSH5 = true,
recoSH6 = false,
recoSH7 = false,
recoSH8 = false,
recoSH9 = false,
recoSH10 = false,
recoSH11 = false;







// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;



//Torniquetes 
float giroTorniquete_x = 0.0f,
giroTorniquete_y = 0.0f,
giroTorniquete_z = 0.0f;


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

// Variables proyecto
float
// Pez 1
movCola = 0.0f,
posXpez = 0.0f,
posXpez1 = 0.0f,
rotPez = 0.0,
rotPez1 = 0.0f,

// Pez 2
posXpez2 = 0.0f,
posXpez21 = 0.0f,
rotPezA = 0.0f,
rotPezA1 = 0.0f,

//Pescado Gris
posXfish = 0.0f,
posXfish1 = 0.0f,
rotfish = 0.0f,
rotfish1 = 0.0f,

//Pescado morado

posXfish3 = 0.0f,
posXfish31 = 0.0f,
rotfish3 = 0.0f,
rotfish31 = 0.0f;

float
//Medusa
posXmedusa = 0.0f,

//Tiburon
posXshark = 0.0f,
posXshark01 = 0.0f,
posXshark02 = 0.0f,
posXshark03 = 0.0f,
rotshark = 0.0f,
rotshark01 = 0.0f,
rotshark02 = 0.0f,
rotshark03 = 0.0f,
movColaShark = 0.0f,
movAletasShark = 0.0f,
movBocaShark = 0.0f;

float
// Pinguino
posXPin = 0.0f,
posZPin = 0.0f,
rotPin = 0.0f,
rotPatasPin = 0.0f,
movCabezaPin = 0.0f,

posXGirl = 0.0f,
posZGirl= 0.0f,
movBrazoDerGirl = 0.0f,
movBrazoIzqGirl = 0.0f,
rotPiernaDerGirl = 0.0f,
rotPiernaIzqGirl = 0.0f,
rotCabezaGirl = 0.0f;

float

// Variables proyecto

movColaInc = 0.0f,
posXpezInc = 0.0f,
rotPezInc = 0.0,

posXpez2Inc = 0.0f,
rotPezAInc = 0.0f,


posXfishInc = 0.0f,
rotfishInc = 0.0f,

posXfish3Inc = 0.0f,
rotfish3Inc = 0.0f;

/*
posXniñaInc = 0.0f,
posZniñaInc = 0.0f,
movBrazoDerNiñaInc = 0.0f,
movBrazoIzqNiñaInc = 0.0f,
rotPiernaDerNiñaInc = 0.0f,
rotPiernaIzqNiñaInc = 0.0f,
rotCabezaNiñaInc = 0.0f,

posXPinInc = 0.0f,
posZPinInc = 0.0f,
rotPinInc = 0.0f,
rotPatasPinInc = 0.0f,
movCabezaPinInc = 0.0f;*/







//Variables para animación delfin
float pos_x;
float pos_y;
float pos_z;

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

	//Variables proyecto
	float movCola;
	float posXpez;
	float rotPez;

	float posXpez2;
	float rotPezA;

	float posXfish;
	float rotfish;

	float posXfish3;
	float rotfish3;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

float proceduralTime = 0.0f;   //OLAS
float wavesTime = 0.0f;

float proceduralTime2 = 0.0f;   //LUNA
float moonTime = 0.0f;


void saveFrame(void){
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


	KeyFrame[FrameIndex].posXfish = posXfish;
	KeyFrame[FrameIndex].rotfish = rotfish;

	KeyFrame[FrameIndex].posXfish3 = posXfish3;
	KeyFrame[FrameIndex].rotfish3 = rotfish3;


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


	posXfish = KeyFrame[0].posXfish;
	rotfish = KeyFrame[0].rotfish;

	posXfish3 = KeyFrame[0].posXfish3;
	rotfish3 = KeyFrame[0].rotfish3;


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


	posXfishInc = (KeyFrame[playIndex + 1].posXfish - KeyFrame[playIndex].posXfish) / i_max_steps;
	rotfishInc = (KeyFrame[playIndex + 1].rotfish - KeyFrame[playIndex].rotfish) / i_max_steps;

	posXfish3Inc = (KeyFrame[playIndex + 1].posXfish3 - KeyFrame[playIndex].posXfish3) / i_max_steps;
	rotfish3Inc = (KeyFrame[playIndex + 1].rotfish3 - KeyFrame[playIndex].rotfish3) / i_max_steps;


}

void animate(void)
{
	////////////////////////////////    SOL
	lightDirection.x = 100 * cos(myVariable);
	lightDirection.y = 100 * sin(myVariable);
	myVariable += 0.01f;

	if (lightDirection.y <= 0)
	{
		activaSkyboxDia = false;
	}
	else
	{
		activaSkyboxDia = true;
	}

	////////////////////////////////  ANIMACION PINGUINO
	if (animacionP2) {
		//animacion alas

		if (animalaspingu == 0) {//CAMBIAR PIVOTE
			alaspingu += 5.0f;
			if (alaspingu >= 15) {
				animalaspingu = 1;
			}
		}
		else {
			alaspingu -= 5.0f;
			if (alaspingu <= -15) {
				animalaspingu = 0;
			}
		}

		//animacion patas
		if (animpataspingu == 0) {
			pataspingu += 10.0f;
			if (pataspingu >= 25) {
				animpataspingu = 1;
			}
		}
		else {
			pataspingu -= 10.0f;
			if (pataspingu <= -25) {
				animpataspingu = 0;
			}
		}

		//animacion recorrido




		//animacion recorrido
		if (reco5) {
			movPingu_x2 = 400.0f;
			movPingu_z2 += 1.0f;
			orientaPingu2 = 0.0f;
			if (movPingu_z2 >= 80.0f) {
				reco5 = false;
				reco6 = true;

			}

		}

		if (reco6) {
			movPingu_x2 += 1.0f;
			movPingu_z2 -= 1.43f;
			orientaPingu2 = 145.0f;
			if (movPingu_x2 >= 435.0f) {
				reco6 = false;
				reco7 = true;
				//reco7 = true;

			}

		}


		if (reco7) {
			movPingu_x2 -= 1.0f;
			movPingu_z2 -= 0.0f;
			orientaPingu2 = -135.0f;
			if (movPingu_x2 <= 400.0f) {
				movPingu_x2 = 400.0f;
				movPingu_z2 = 30.0f;
				reco7 = false;
				reco8 = true;

			}

		}



		if (reco8) {
			movPingu_x2 -= 1.0f;
			movPingu_y2 = -(movPingu_x2 - 375.0f) * (movPingu_x2 - 375.0f) / 25.0f + 50.0f;
			orientaPingu2 = -135.0f;
			if (movPingu_x2 <= 330.0f) {
				movPingu_y2 = 0.0f;
				movPingu_x2 = 275.0f;
				reco8 = false;
				reco9 = true;

			}

		}

		if (reco9) {
			movPingu_x2 -= 1.0f;
			movPingu_y2 = -(movPingu_x2 - 120.0f) * (movPingu_x2 - 120.0f) / 25.0f + 50.0f;
			orientaPingu2 = -135.0f;
			if (movPingu_x2 <= 90.0f) {
				movPingu_x2 = 90.0f;
				movPingu_y2 = 0.0f;
				movPingu_z2 = 65.0f;
				reco9 = false;
				reco10 = true;

			}

		}

		if (reco10) {
			movPingu_x2 -= 1.0f;
			movPingu_z2 += 1.0f;
			orientaPingu2 = 90.0f;
			if (movPingu_z2 <= 100.0f) {
				reco10 = false;
				reco11 = true;

			}

		}

		if (reco11) {
			movPingu_x2 += 5.0f;
			//movPingu_z2 += 1.0f;
			orientaPingu2 = 90.0f;
			if (movPingu_x2 >= 400.0f) {
				reco11 = false;
				reco5 = true;

			}

		}

	}


	///////////////////////////////////////////
	if (animacionFoca2)
	{
		//animacion aletas superiores
		if (anim_aletasSupFoca == 0) {//CAMBIAR PIVOTE
			aletasSupFoca += 3.0f;
			if (aletasSupFoca >= 15) {
				anim_aletasSupFoca = 1;
			}
		}
		else {
			aletasSupFoca -= 3.0f;
			if (aletasSupFoca <= -15) {
				anim_aletasSupFoca = 0;
			}
		}

		//animacion aletas Inferiores

		if (anim_aletasInfFoca == 0) {
			aletasInfFoca += 5.0f;
			if (aletasInfFoca >= 25) {
				anim_aletasInfFoca = 1;
			}
		}
		else {
			aletasInfFoca -= 5.0f;
			if (aletasInfFoca <= -25) {
				anim_aletasInfFoca = 0;
			}
		}




		//animacion recorrido
		
		if (recoF5) {
			myVariable2 += 0.01;
			//movFoca_x2 =150+200.0f*sin(myVariable);
			//movFoca_z2 = 150.0f*sin(2*myVariable);
			movFoca_x2 = 200.0f * cos(4 * myVariable2);
			movFoca_z2 = 150.0f * sin(4 * myVariable2);
			movFoca_y2 = myVariable2 * 20.0f;
			myVariable3 = movFoca_y2;

			orientaFoca2 -= 2.29f;
			if (myVariable2 >= 2.0f * PI) {
				myVariable2 = 0.0f;
				movFoca_z2 = 30.0f;
				movFoca_y2 = myVariable3;
				recoF5 = false;
				recoF6 = true;
			}
		}

		if (recoF6) {
			movFoca_y2 -= 2.0f;
			//movFoca_z2 += 1.0f;
			orientaFoca2 = 0.0f;
			if (movFoca_y2 <= 0.0f) {
				recoF6 = false;
				recoF5 = true;

			}

		}
	}
		//////////////////////////////////
		//Ballena
		/////////

	if (animacionBallena2)
		{
			//animacion aletas superiores
			if (anim_aletasSupBallena == 0) {//CAMBIAR PIVOTE
				aletasSupBallena += 1.0f;
				if (aletasSupBallena >= 15) {
					anim_aletasSupBallena = 1;
				}
			}
			else {
				aletasSupBallena -= 1.0f;
				if (aletasSupBallena <= -15) {
					anim_aletasSupBallena = 0;
				}
			}

			//animacion aletas Inferiores

			if (anim_aletasInfBallena == 0) {
				aletasInfBallena += 1.0f;
				if (aletasInfBallena >= 15) {
					anim_aletasInfBallena = 1;
				}
			}
			else {
				aletasInfBallena -= 1.0f;
				if (aletasInfBallena <= -15) {
					anim_aletasInfBallena = 0;
				}
			}













			//animacion recorrido
			if (recoB5) {
				movBallena_x2 -= 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = 0.0f;
				if (movBallena_x2 <= -3000.0f) {
				recoB5 = false;
				recoB8 = true;

				}

			}
			/*
			if (recoB6) {
				movBallena_x2 = 0.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				if (orientaBallena2 <= 0.0f) {
					orientaBallena2 += 5.0f;
				}
				if (orientaBallena2 >= 0.0f) {
					recoB6 = false;
					recoB8 = true;

				}
			}
			*/
			if (recoB8) {
				movBallena_z2 -= 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -90.0f;
				if (movBallena_z2 <= -600.0f) {
					recoB8 = false;
					recoB9 = true;

				}
			}

			if (recoB9) {
				movBallena_x2 += 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				if (movBallena_x2 >= -450.0f) {
					recoB9 = false;
					recoB10 = true;

				}

			}

			if (recoB10) {
				movBallena_y2 -= 10.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				orientaBallena3 = 0.0f;
				if (movBallena_y2 < -1200.0f) {
					recoB10 = false;
					recoB11 = true;

				}

			}

			if (recoB11) {
				movBallena_y2 += 10.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				orientaBallena3 = -90.0f;
				if (movBallena_y2 >300.0f) {
					recoB11 = false;
					recoB12 = true;

				}

			}

			if (recoB12) {
				movBallena_y2 -= 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				orientaBallena3 = 0.0f;
				if (movBallena_y2 < -1200.0f) {
					recoB12 = false;
					recoB13 = true;

				}

			}


			if (recoB13) {
				movBallena_x2 += 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				if (movBallena_x2 >= 6000.0f) {
					recoB13 = false;
					recoB14 = true;

				}

			}


			if (recoB14) {
				movBallena_y2 += 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				orientaBallena3 = -90.0f;
				if (movBallena_y2 > 300.0f) {
					recoB14 = false;
					recoB15 = true;

				}

			}


			if (recoB15) {
				movBallena_y2 -= 50.0f;
				//movFoca_z2 += 1.0f;
				orientaBallena2 = -180.0f;
				if (orientaBallena3<=0.0f) {
					orientaBallena3 += 5.0f;
				}
				if (movBallena_y2 < -1200.0f) {
					recoB15 = false;
					recoB5 = true;

				}

			}





	}


			////////////////////////////////  ANIMACION SHOPPER
	if (animacionSH) {
		//animacion brazos

		if (!animBrazoDerShopper) {//CAMBIAR PIVOTE
			brazoDerShopper += 8.0f;

			if (brazoDerShopper >= 25) {
				animBrazoDerShopper = true;
			}
		}
		else {
			brazoDerShopper -= 8.0f;
			if (brazoDerShopper <= -25) {
				animBrazoDerShopper = false;
			}
		}

		//animacion pies
		if (!animPieDerShopper) {
			pieDerShopper += 8.0f;
			if (pieDerShopper >= 25) {
				animPieDerShopper = true;
			}
		}
		else {
			pieDerShopper -= 8.0f;
			if (pieDerShopper <= -25) {
				animPieDerShopper = false;
			}
		}

	}




	///////////////////////////////////////////
	pez01.x = 70.0f * cos(posXpez);
	pez01.z = 70.0f * sin(posXpez);
	pez01.y = 20.0f * cos(5 * posXpez);
	posXpez += 0.005f;

	rotPez -= 0.29;
	//////////////////////////////////////////

	pez02.x = 70.0f * cos(posXpez2);
	pez02.z = 70.0f * sin(posXpez2);
	pez02.y = 20.0f * -cos(5 * posXpez2);
	posXpez2 += 0.005f;

	rotPezA -= 0.29;
	////////////////////////////////////////////

	tfish.x = 70.0f * cos(posXfish);
	tfish.z = 70.0f * sin(posXfish);
	tfish.y = 20.0f * -sin(5 * posXfish);
	posXfish += 0.005f;

	rotfish -= 0.29;
	////////////////////////////////////////////

	tfish3.x = 70.0f * cos(posXfish3);
	tfish3.z = 70.0f * sin(posXfish3);
	tfish3.y = 20.0f * sin(5 * posXfish3);
	posXfish3 += 0.005f;

	rotfish3 -= 0.29;
	////////////////////////////////////////////

	pez011.x = 70.0f * -cos(posXpez1);
	pez011.z = 70.0f * sin(posXpez1);
	pez011.y = 20.0f * cos(5 * posXpez1);
	posXpez1 += 0.005f;

	rotPez1 += 0.29;
	//////////////////////////////////////////

	pez021.x = 70.0f * -cos(posXpez21);
	pez021.z = 70.0f * sin(posXpez21);
	pez021.y = 20.0f * -cos(5 * posXpez21);
	posXpez21 += 0.005f;

	rotPezA1 += 0.29;
	////////////////////////////////////////////

	tfish1.x = 70.0f * -cos(posXfish1);
	tfish1.z = 70.0f * sin(posXfish1);
	tfish1.y = 20.0f * -sin(5 * posXfish1);
	posXfish1 += 0.005f;

	rotfish1 += 0.29;
	////////////////////////////////////////////

	tfish31.x = 70.0f * -cos(posXfish31);
	tfish31.z = 70.0f * sin(posXfish31);
	tfish31.y = 20.0f * sin(5 * posXfish31);
	posXfish31 += 0.005f;

	rotfish31 += 0.29;

	
	////////////////////////////////////////////

	tshark.x = 250.0f * cos(posXshark);
	tshark.z = 250.0f * sin(posXshark);
	tshark.y = 30.0f * cos(3 * posXshark);
	posXshark += 0.005f;

	rotshark -= 0.29;
	////////////////////////////////////////////

	tshark01.x = 250.0f * -cos(posXshark01);
	tshark01.z = 250.0f * sin(posXshark01);
	tshark01.y = 30.0f * cos(3 * posXshark01);
	posXshark01 += 0.005f;

	rotshark01 += 0.29;
	////////////////////////////////////////////

	tshark02.x = 340.0f * cos(posXshark02);
	tshark02.z = 340.0f * sin(posXshark02);
	tshark02.y = 30.0f * cos(3 * posXshark02);
	posXshark02 += 0.005f;

	rotshark02 -= 0.29;
	////////////////////////////////////////////

	tshark03.x = 340.0f * -cos(posXshark03);
	tshark03.z = 340.0f * sin(posXshark03);
	tshark03.y = 30.0f * cos(3 * posXshark03);
	posXshark03 += 0.005f;

	rotshark03 += 0.29;

	
	////////////////////////////////////////////

	tmedusa.x = 70.0f * cos(posXmedusa);
	tmedusa.z = 70.0f * sin(posXmedusa);
	tmedusa.y = 30.0f * cos(5 * posXmedusa);
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

			posXfish += posXfishInc;
			rotfish += rotfishInc;

			posXfish3 += posXfish3Inc;
			rotfish3 += rotfish3Inc;

			i_curr_steps++;
		}
	}

	//Vehi­culo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void music() {
	if (sound) {
		bool played = PlaySound(L"acuario.wav", NULL, SND_LOOP | SND_ASYNC);
		sound = false;
	}
}


int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

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
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");
	Shader texturaShader("Shaders/shaders_Textura/shader_texture_color.vs", "Shaders/shaders_Textura/shader_texture_color.fs");
	Shader fbxShader("Shaders/shaders_1/10_vertex_simple.vs", "Shaders/shaders_1/10_fragment_simple.fs");
	Shader wavesShader("Shaders/shaders_1/13_wavesAnimation.vs", "Shaders/shaders_1/13_wavesAnimation.fs");
	Shader lunaShader ("Shaders/shaders_1/12_ProceduralAnimation.vs", "Shaders/shaders_1/12_ProceduralAnimation.fs");
	Shader1* my3Shader;
	my3Shader = new Shader1("Shaders/shaders_1/02-simplePVM.vs", "Shaders/shaders_1/02-simplePVM.fs");
	Shader1* myOtherShader;
	myOtherShader = new Shader1("Shaders/shaders_1/02-simplePVM.vs", "Shaders/shaders_1/02-simplePVM.fs");
	Shader myShader2("Shaders/shaders_Textura/shader_texture_color.vs", "Shaders/shaders_Textura/shader_texture_color.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg",


	};

	vector<std::string> faces1
	{
		"resources/skybox/right_noche.jpg",
		"resources/skybox/left_noche.jpg",
		"resources/skybox/top_noche.jpg",
		"resources/skybox/bottom_noche.jpg",
		"resources/skybox/front_noche.jpg",
		"resources/skybox/back_noche.jpg"

	};

	Skybox skybox = Skybox(faces);

	Skybox skybox1 = Skybox(faces1);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	skyboxShader.setInt("skybox1", 1);


	//MODELOS MIGUEL PROYECTO

	//Model pecera1("resources/objects/models/tanques/pecera1/PeceraYObjetos.obj");
	Model pecera2("resources/objects/models/tanques/pecera2/pecera2yElementos.obj");
	Model pecera3("resources/objects/models/tanques/pecera3/pecera3yTroncos.obj");

	


	Model colafish("resources/objects/Peses/colafish.obj");
	Model bodyfish("resources/objects/Peses/bodyfish.obj");
	Model pez2("resources/objects/Peses/pez2.obj");
	Model pez3("resources/objects/Peses/Pez3/13009_Coral_Beauty_Angelfish_v1_l3.obj");
	Model pezGris("resources/objects/ModeloPez/fish.obj");

	Model medusa("resources/objects/Medusa/medusa.obj");

	//Tiburon
	Model AletaDerTiburon("resources/objects/shark/AletaDerTiburon.obj");
	Model AletaIzqTiburon("resources/objects/shark/AletaIzqTiburon.obj");
	Model BocaTiburon("resources/objects/shark/BocaTiburon.obj");
	Model ColaTiburon("resources/objects/shark/ColaTiburon.obj");
	Model CuerpoDelTiburon("resources/objects/shark/CuerpoDelTiburon.obj");
	Model CuerpoPrinTiburon("resources/objects/shark/CuerpoPrinTiburon.obj");

	//Pinguino

	Model CabezaPin("resources/objects/Pinguino/CabezaPinguino.obj");
	Model CuerpoPin("resources/objects/Pinguino/PinguinoCuerpo.obj");
	Model TorsoPin("resources/objects/Pinguino/TorsoPinguino.obj");
	Model patasPin("resources/objects/Pinguino/PinguinoPatas.obj");




		
	//Persona 11 (niña)

	Model CabezaGirl("resources/objects/Personas/Persona111/Cabeza/Cabeza.obj");
	Model GirlBrazoDer("resources/objects/Personas/Persona111/Brazo_derecho/BrazoDerecho.obj");
	Model GirlBrazoIzq("resources/objects/Personas/Persona111/Brazo_izquierdo/BrazoIzquierdo.obj");
	Model GirlPiernaDer("resources/objects/Personas/Persona111/Pierna_derecha/PiernaDerecha.obj");
	Model GirlPiernaIzq("resources/objects/Personas/Persona111/Pierna_izquierda/PiernaIzquierda.obj");
	Model Torso("resources/objects/Personas/Persona111/Torso/Torso.obj");
	Model GirlTorso("resources/objects/Personas/Persona111/Torso_cabeza/TorsoCabeza.obj");

	//PERSONAS

	Model persona2("resources/objects/Personas/Persona2/12-1.obj");
	Model persona3("resources/objects/Personas/Persona3/12-2.obj");
	Model persona4("resources/objects/Personas/Persona4/12-3.obj");
	Model persona5("resources/objects/Personas/Persona5/12-4.obj");
	Model persona6("resources/objects/Personas/Persona6/12-5.obj");
	Model persona7("resources/objects/Personas/Persona7/12-6.obj");
	Model persona8("resources/objects/Personas/Persona8/12-7.obj");
	Model persona9("resources/objects/Personas/Persona9/Persona9.obj");


	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model cubo("resources/objects/cubo/cube02.obj");
	ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);

	ModelAnim perCaminando("resources/objects/Mixamo/Mixamo/RumbaDancing.dae");
	perCaminando.initShaders(animShader.ID);

	//MODELOS NAVIH PROYECTO
	Model igloo("resources/objects/Igloo/igloo.obj");
	Model vegetacion("resources/objects/Vegetacion/vegetacion.obj");
	Model acuario("resources/objects/Acuario/acuario.obj");
	Model acuario_vidrio("resources/objects/Acuario/acuario_vidrio.obj");
	Model banio("resources/objects/Banio2/banio2.obj");
	Model torniquete_base("resources/objects/Torniquete/torniquete_base.obj");
	Model torniquete_tubo("resources/objects/Torniquete/torniquete_tubo.obj");
	Model taquilla("resources/objects/Taquilla/taquilla.obj");
	Model muro("resources/objects/Muro/muro.obj");
	Model vallae("resources/objects/vallap/vallae.obj");
	Model boa_hancock("resources/objects/BoaHancock/boahancock.obj");





	Model azul("resources/objects/Esfera/azul/azul.obj");
	Model blanco("resources/objects/Esfera/blanco/blanco.obj");
	Model gris("resources/objects/Esfera/gris/gris.obj");
	Model naranja("resources/objects/Esfera/naranja/naranja.obj");
	Model negro("resources/objects/Esfera/negro/negro.obj");
	Model rojo("resources/objects/Esfera/rojo/rojo.obj");
	Model rosa("resources/objects/Esfera/rosa/rosa.obj");
	Model zunesha("resources/objects/Zunesha/zunesha.obj");

	//pinguino en partes
	Model pingu1("resources/objects/pinguino2/pingu.obj");
	Model pingupi1("resources/objects/pinguino2/pieizq.obj");
	Model pingupd1("resources/objects/pinguino2/pieder.obj");
	Model pinguai1("resources/objects/pinguino2/alaizq.obj");
	Model pinguad1("resources/objects/pinguino2/alader.obj");


	//Foca en partes
	Model foca("resources/objects/foaccaaa/foca/foca_torso.obj");
	Model focaPi("resources/objects/foaccaaa/foca/foca_aleta_Inf_Derecha.obj");
	Model focaPd("resources/objects/foaccaaa/foca/foca_aleta_Inf_Izquierda.obj");
	Model focaAi("resources/objects/foaccaaa/foca/foca_aleta_S_Derecha.obj");
	Model focaAd("resources/objects/foaccaaa/foca/foca_aleta_S_Izquierda.obj");

	//Ballena partes
	Model ballenaTorso("resources/objects/ballena_Laboon/ballenaLabbon_torso.obj");
	Model ballenaCola("resources/objects/ballena_Laboon/ballenaLabbon_cola.obj");
	
	// Spersonaje avatar Shopper
	//Model shopper("resources/objects/Shopper/renofinal9.obj");
	Model shopperTorso("resources/objects/Shopper/reno_final3_cabeza_torso.obj");
	Model shopperBraDer("resources/objects/Shopper/reno_final3_brazo_derecho.obj");
	Model shopperBraIzq("resources/objects/Shopper/reno_final3_brazo_izquierdo.obj");
	Model shopperPieDer("resources/objects/Shopper/reno_final3_pie_derecho.obj");
	Model shopperPieIzq("resources/objects/Shopper/reno_final3_pie_izquierdo.obj");



	//MODELOS DAVID PROYECTO
	Model moonFBX("resources/FBX/Moon/moon.fbx");
	Model olaFBX("resources/FBX/Ola/grid.fbx");




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

		KeyFrame[i].posXfish = 0;
		KeyFrame[i].rotfish = 0;

		KeyFrame[i].posXfish3 = 0;
		KeyFrame[i].rotfish3 = 0;
		

	}

	//Carga de lasprimitivas 2D
	Circle circle(1.0f);
	meshCircle = new Mesh_1(circle.vertices, circle.indices);

	Triangle triangulo(1.0f);
	meshTriangle = new Mesh_1(triangulo.vertices, triangulo.indices);

	Plane plano(1.0f);
	meshPlane = new Mesh_1(plano.vertices, plano.indices);

	//Carga de las PRIMITIVAS
	CrearCubo();
	CrearPiramide();
	CrearPiramideCuadrangular();
	CrearCilindro(512, 1.0f);
	CrearCono(20, 1.0f);
	CrearSphere(1.0, 40, 40);
	LoadTextures();
	myData();
	CrearTorus(8.0f, 1.0f, 200, 12);
	CrearTetera();
	CrearAsaTetera();
	CrearBoquillaTetera();
	cuboSencillo();



	//CONFIGURACION AUDIO IRRKLANG
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	if (!SoundEngine)
		return 0;
	SoundEngine->play2D("resources/Sound/ElDelfinAzul.mp3", true);
	//SoundEngine->play2D("resources/Sound/acuario.wav", true);


	
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		if (activaSkyboxDia)
		{
			skyboxShader.setInt("skybox", 0);
		}
		else
		{
			skyboxShader.setInt("skybox1", 1);
		}
		

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


		staticShader.use();
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model2;
		glm::mat4 tmp35 = glm::mat4(1.0f);

		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();

			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);


		}
		else if (activeCamera2) {
			projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera2.GetViewMatrix();
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3)
		{
			projection = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3.GetViewMatrix();
			staticShader.setVec3("viewPos", camera3.Position);
		}

		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		// don't forget to enable shader before setting uniforms
		//staticShader.use();
		//Setup Advanced Lights

		//configuracion de CAMARA PARALELA
		//camera.Position.y = 60.0f;
		//staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);////
		

		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		//staticShader.setVec3("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);

		// view/projection transformations
		//camera.Position.y = 100.0f;
		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		}
		else if (activeCamera2) {
			projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera2.GetViewMatrix();
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3)
		{
			projection = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3.GetViewMatrix();
			staticShader.setVec3("viewPos", camera3.Position);
		}


		//projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		//view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		//GLint modelLoc = glGetUniformLocation(staticShader.setFloat("material_shininess", 32.0f), "model");


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		//--------------zunesha----------------------
		//Zunesha Elefante
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -500.0f, -1700.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		staticShader.setMat4("model", model);
		zunesha.Draw(staticShader);


		//--------------Boa Hancock----------------------
		//Zunesha Elefante
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, 400.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		boa_hancock.Draw(staticShader);



		
		//-----------------piso----------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.65f, 0.0f, 0.40f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);





		//elementos delimitantes del escenari

		//Taquilla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 114.0f, 910.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(32.0f));
		staticShader.setMat4("model", model);
		taquilla.Draw(staticShader);

		//Muro entre taquillas Izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -25.0f, 900.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3200.0f));
		staticShader.setMat4("model", model);
		muro.Draw(staticShader);

		//Muro entre taquillas Derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, -25.0f, 900.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3200.0f));
		staticShader.setMat4("model", model);
		muro.Draw(staticShader);

		/*Area de torniquetes Entrada
		Torniquete base*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-270.0f, 50.0f, 970.0f));
		tmp2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(1.1f, 19.5f, -60.0f));
		model = glm::rotate(model, glm::radians(giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-175.0f, 50.0f, 970.0f));
		tmp2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(1.1f, 19.5f, -60.0f));
		model = glm::rotate(model, glm::radians(giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);


		/*Area de torniquetes Salida
		Torniquete base*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(178.0f, 50.0f, 860.0f));
		tmp2 = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(-1.1f, 19.5f, -60.0f));
		model = glm::rotate(model, glm::radians(-giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(273.0f, 50.0f, 860.0f));
		tmp2 = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(-1.1f, 19.5f, -60.0f));
		model = glm::rotate(model, glm::radians(-giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00085f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);


		//valla entrada frente derecha
		model = glm::translate(glm::mat4(1.0f), glm::vec3(560.0f, 0.0f, 910.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1140.0f, 0.0f, 910.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(1525.0f, 0.0f, 910.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::scale(model, glm::vec3(0.25f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);


		//valla entrada frente izquierda
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-600.0f, 0.0f, 910.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1180.0f, 0.0f, 910.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1525.0f, 0.0f, 910.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::scale(model, glm::vec3(0.25f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);



		//valla lateral derecha
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f, 0.0f, 610.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f, 0.0f, -570.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f, 0.0f, -920.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);


		//valla lateral izquierda
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1590.0f, 0.0f, 610.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1590.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1590.0f, 0.0f, -570.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1590.0f, 0.0f, -920.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);



		//valla trasera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1280.0f, 0.0f, -1000.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(700.0f, 0.0f, -1000.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -1000.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-460.0f, 0.0f, -1000.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1040.0f, 0.0f, -1000.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1460.0f, 0.0f, -1000.0f));
		model = glm::scale(model, glm::vec3(5.25f));
		model = glm::scale(model, glm::vec3(0.55f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		vallae.Draw(staticShader);










		//----------------tanques--------------------------

		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(60.0f, 0.0f, -150.0f));
		//model = glm::translate(model, glm::vec3(-490.0f, 0.0f, 1150.0f));
		model = glm::translate(model, glm::vec3(-660.0f, 0.0f, 1650.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(20.0f, 15.0f, 20.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(100.0f, 45.0f, 100.0f));
		model = glm::scale(model, glm::vec3(125.0f, 45.0f, 135.0f));
		staticShader.setMat4("model", model);
		pecera2.Draw(staticShader);



		//--------------------------------
		// Personaje Foca
		//-------------------------------

		//Foca animada
		glm::mat4 tmp31 = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f,0.0f, 0.0f));
		tmp31 = model = glm::translate(model, glm::vec3(-450.0f, 50.0f, 500.0f));
		model = glm::translate(tmp31, glm::vec3(movFoca_x2, movFoca_y2, movFoca_z2));
		model = glm::rotate(model, glm::radians(orientaFoca2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		foca.Draw(staticShader);

		model = glm::translate(tmp31, glm::vec3(movFoca_x2, movFoca_y2, movFoca_z2));
		model = glm::rotate(model, glm::radians(orientaFoca2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-aletasInfFoca), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		focaPi.Draw(staticShader);

		
		model = glm::translate(tmp31, glm::vec3(movFoca_x2, movFoca_y2, movFoca_z2));

		model = glm::rotate(model, glm::radians(orientaFoca2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -15.0f));
		model = glm::rotate(model, glm::radians(aletasInfFoca), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		focaPd.Draw(staticShader);


		model = glm::translate(tmp31, glm::vec3(movFoca_x2, movFoca_y2, movFoca_z2));
		model = glm::rotate(model, glm::radians(orientaFoca2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-5.5f, -1.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-aletasSupFoca), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		focaAi.Draw(staticShader);

		model = glm::translate(tmp31, glm::vec3(movFoca_x2, movFoca_y2, movFoca_z2));
		model = glm::rotate(model, glm::radians(orientaFoca2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 5.0f));
		model = glm::rotate(model, glm::radians(aletasSupFoca), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		focaAd.Draw(staticShader);




		// Ballena animada

		
		glm::mat4 tmp51 = glm::mat4(1.0f);
	
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2500.0f, 0.0f, 0.0f));
		tmp51 = model = glm::translate(model, glm::vec3(-450.0f, -10.0f, 500.0f));
		model = glm::translate(tmp51, glm::vec3(movBallena_x2, movBallena_y2, movBallena_z2));
	
		model = glm::rotate(model, glm::radians(orientaBallena2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaBallena3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		ballenaTorso.Draw(staticShader);

		
		model = glm::translate(tmp51, glm::vec3(movBallena_x2, movBallena_y2, movBallena_z2));
		model = glm::rotate(model, glm::radians(orientaBallena2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaBallena3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(610.0f, -60.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-aletasInfBallena), glm::vec3(0.0f, 0.0f, 1.0f)); //1.0,0.0,0.0
		model = glm::scale(model, glm::vec3(18.0f));
		staticShader.setMat4("model", model);
		ballenaCola.Draw(staticShader);








		// Pecera tiburones

		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -350.0f));
		model = glm::translate(model, glm::vec3(-310.0f, 0.0f, -350.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(120.0f, 15.0f, 120.0f));
		model = glm::scale(model, glm::vec3(145.0f, 15.0f, 220.0f));
		staticShader.setMat4("model", model);
		pecera3.Draw(staticShader);


		// Lugar Pinguinos
		//clima frio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1008.0f, /*-15.0f*/0.0f, -250.0f));
		model = glm::scale(model, glm::vec3(2.9f, 2.5f, 2.8f));
		staticShader.setMat4("model", model);
		igloo.Draw(staticShader);


		//--------------------------------
		// Personaje pinguino1
		//-------------------------------

		//pinguinoanimado 2
		glm::mat4 tmp30 = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1008.0f, /*-15.0f*/0.0f, -250.0f));
		tmp30 = model = glm::translate(model, glm::vec3(-190.0f, 30.0f, 70.0f));
		model = glm::translate(tmp30, glm::vec3(movPingu_x2, movPingu_y2, movPingu_z2));
		
		model = glm::rotate(model, glm::radians(orientaPingu2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		pingu1.Draw(staticShader);


		
		model = glm::translate(tmp30, glm::vec3(movPingu_x2, movPingu_y2+5.0f, movPingu_z2));
		model = glm::rotate(model, glm::radians(orientaPingu2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-pataspingu), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		pingupi1.Draw(staticShader);

		
		model = glm::translate(tmp30, glm::vec3(movPingu_x2, movPingu_y2+5.0f, movPingu_z2));

		model = glm::rotate(model, glm::radians(orientaPingu2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(pataspingu), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		pingupd1.Draw(staticShader);


		model = glm::translate(tmp30, glm::vec3(movPingu_x2, movPingu_y2, movPingu_z2));
		model = glm::rotate(model, glm::radians(orientaPingu2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-alaspingu), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		pinguai1.Draw(staticShader);

		model = glm::translate(tmp30, glm::vec3(movPingu_x2, movPingu_y2, movPingu_z2));
		model = glm::rotate(model, glm::radians(orientaPingu2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(alaspingu), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		pinguad1.Draw(staticShader);

		//////////////AVATAR
		//position = position + scaleV*forwardView;
		//camera.Front = forwardView;
		//camera.Position = position;
		

		//shopper fijo

		if (activeCamera)
		{
			tmp35 = model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			model2 = glm::rotate(model2, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			//model2 = glm::translate(model2, glm::vec3(0.0f, -0.2f, -1.0f));

			model2 = glm::scale(model2, glm::vec3(10.0f));
			staticShader.setMat4("model", model2);
			//staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
			shopperTorso.Draw(staticShader);


			model2 = glm::translate(tmp35, glm::vec3(-9.0f, 27.0f, 0.0f));
			model2 = glm::rotate(model2, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			//model2 = glm::translate(model2, glm::vec3(-0.34f,5.0f+ 0.26f, -2.8f));
			model2 = glm::rotate(model2, glm::radians(brazoDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(9.f));
			staticShader.setMat4("model", model2);
			//staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
			shopperBraDer.Draw(staticShader);

			model2 = glm::translate(tmp35, glm::vec3(-4.0f, 9.0f, 0.0f));
			model2 = glm::rotate(model2, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			//model2 = glm::translate(model2, glm::vec3(-0.18f,5.0f -0.33f, -2.5f));
			//	model2 = glm::translate(model2, glm::vec3(0.0f, -0.03f,0.0f ));
			model2 = glm::rotate(model2, glm::radians(pieDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(9.0f));
			staticShader.setMat4("model", model2);
			//staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
			shopperPieDer.Draw(staticShader);

			model2 = glm::translate(tmp35, glm::vec3(8.0f, 27.0f, 0.0f));
			model2 = glm::rotate(model2, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));

			model2 = glm::rotate(model2, glm::radians(-brazoDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(9.0f));
			staticShader.setMat4("model", model2);
			//staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
			shopperBraIzq.Draw(staticShader);

			model2 = glm::translate(tmp35, glm::vec3(4.0f, 9.0f, 0.0f));
			model2 = glm::rotate(model2, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));

			model2 = glm::rotate(model2, glm::radians(-pieDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(9.0f));
			staticShader.setMat4("model", model2);
			//staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
			shopperPieIzq.Draw(staticShader);
		}

		//////////////AVATAR
		//position = position + scaleV*forwardView;
		//camera.Front = forwardView;
		//camera.Position = position;
		else if (activeCamera3)
		{
			tmp35 = model2 = glm::translate(glm::mat4(1.0f), glm::vec3(camera3.Position.x, camera3.Position.y, camera3.Position.z));
			model2 = glm::rotate(model2, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::translate(model2, glm::vec3(0.0f, -0.2f, -1.0f));

			model2 = glm::scale(model2, glm::vec3(0.1f));
			staticShader.setMat4("model", model2);
			staticShader.setVec3("viewPos", camera3.Position.x, camera3.Position.y, camera3.Position.z);
			shopperTorso.Draw(staticShader);



			model2 = glm::rotate(tmp35, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::translate(model2, glm::vec3(-0.24f, 0.26f, -2.8f));
			model2 = glm::rotate(model2, glm::radians(brazoDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(0.25f));
			staticShader.setMat4("model", model2);
			staticShader.setVec3("viewPos", camera3.Position.x, camera3.Position.y, camera3.Position.z);
			shopperBraDer.Draw(staticShader);


			model2 = glm::rotate(tmp35, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::translate(model2, glm::vec3(-0.10f, -0.33f, -2.5f));
			//	model2 = glm::translate(model2, glm::vec3(0.0f, -0.03f,0.0f ));
			model2 = glm::rotate(model2, glm::radians(pieDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(0.25f));
			staticShader.setMat4("model", model2);
			staticShader.setVec3("viewPos", camera3.Position.x, camera3.Position.y, camera3.Position.z);
			shopperPieDer.Draw(staticShader);


			model2 = glm::rotate(tmp35, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::translate(model2, glm::vec3(0.22f, 0.26f, -2.8f));
			model2 = glm::rotate(model2, glm::radians(-brazoDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(0.25f));
			staticShader.setMat4("model", model2);
			staticShader.setVec3("viewPos", camera3.Position.x, camera3.Position.y, camera3.Position.z);
			shopperBraIzq.Draw(staticShader);


			model2 = glm::rotate(tmp35, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::translate(model2, glm::vec3(+0.10f, -0.33f, -2.5f));
			model2 = glm::rotate(model2, glm::radians(-pieDerShopper), glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::scale(model2, glm::vec3(0.25f));
			staticShader.setMat4("model", model2);
			staticShader.setVec3("viewPos", camera3.Position.x, camera3.Position.y, camera3.Position.z);
			shopperPieIzq.Draw(staticShader);
		}
		
		

		

		// PRIMERA ANIMACION PEZ

		// Body 

		//model = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-905.0f, 50.0f, 600.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-905.0f, 100.0f, 600.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-905.0f, 150.0f, 600.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-905.0f, 200.0f, 600.0f));
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



		// Body 

		//model = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1200.0f, 50.0f, 600.0f));
		//model = glm::translate(model, glm::vec3(-200.0f, 14.0f, 250.0f));
		//model = glm::translate(model, glm::vec3(posXpez , 0, 0));
		model = glm::translate(model, pez011);
		//tmp = model = glm::rotate(model, glm::radians(rotPez), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotPez1), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1200.0f, 100.0f, 600.0f));
		model = glm::translate(model, pez011);
		tmp = model = glm::rotate(model, glm::radians(rotPez1), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1200.0f, 150.0f, 600.0f));
		model = glm::translate(model, pez011);
		tmp = model = glm::rotate(model, glm::radians(rotPez1), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1200.0f, 200.0f, 600.0f));
		model = glm::translate(model, pez011);
		tmp = model = glm::rotate(model, glm::radians(rotPez1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		bodyfish.Draw(staticShader);

		// Cola 04
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(movCola), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 13.0f));
		staticShader.setMat4("model", model);
		colafish.Draw(staticShader);



		// SEGUNDA ANIMACION PEZ

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-800.0f, 40.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-800.0f, 90.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-800.0f, 140.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-800.0f, 190.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-980.0f, 40.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-980.0f, 90.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-980.0f, 140.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-980.0f, 190.0f, 600.0f));
		model = glm::translate(model, pez02);
		model = glm::rotate(model, glm::radians(rotPezA), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1280.0f, 40.0f, 600.0f));
		model = glm::translate(model, pez021);
		model = glm::rotate(model, glm::radians(rotPezA1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1280.0f, 90.0f, 600.0f));
		model = glm::translate(model, pez021);
		model = glm::rotate(model, glm::radians(rotPezA1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1280.0f, 140.0f, 600.0f));
		model = glm::translate(model, pez021);
		model = glm::rotate(model, glm::radians(rotPezA1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1280.0f, 190.0f, 600.0f));
		model = glm::translate(model, pez021);
		model = glm::rotate(model, glm::radians(rotPezA1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		staticShader.setMat4("model", model);
		pez2.Draw(staticShader);


		// Animacion Pez Gris

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1420.0f, 35.0f, 600.0f));
		model = glm::translate(model, tfish1);
		model = glm::rotate(model, glm::radians(rotfish1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		pezGris.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1420.0f, 85.0f, 600.0f));
		model = glm::translate(model, tfish1);
		model = glm::rotate(model, glm::radians(rotfish1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		pezGris.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1420.0f, 135.0f, 600.0f));
		model = glm::translate(model, tfish1);
		model = glm::rotate(model, glm::radians(rotfish1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		pezGris.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1420.0f, 185.0f, 600.0f));
		model = glm::translate(model, tfish1);
		model = glm::rotate(model, glm::radians(rotfish1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		staticShader.setMat4("model", model);
		pezGris.Draw(staticShader);



		//ANIMACION PEZ 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-850.0f, 40.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-850.0f, 90.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-850.0f, 140.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-850.0f, 190.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);




		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1050.0f, 40.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1050.0f, 90.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1050.0f, 140.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1050.0f, 190.0f, 600.0f));
		model = glm::translate(model, tfish3);
		model = glm::rotate(model, glm::radians(rotfish3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1360.0f, 50.0f, 600.0f));
		model = glm::translate(model, tfish31);
		model = glm::rotate(model, glm::radians(rotfish31), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1360.0f, 90.0f, 600.0f));
		model = glm::translate(model, tfish31);
		model = glm::rotate(model, glm::radians(rotfish31), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1360.0f, 140.0f, 600.0f));
		model = glm::translate(model, tfish31);
		model = glm::rotate(model, glm::radians(rotfish31), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1360.0f, 190.0f, 600.0f));
		model = glm::translate(model, tfish31);
		model = glm::rotate(model, glm::radians(rotfish31), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		pez3.Draw(staticShader);

		
		// Personas Pecera peces



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-540.0f, 0.0f, 760.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona2.Draw(staticShader);





		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-540.0f, 0.0f, 360.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona3.Draw(staticShader);

		
		// HORIZONTAL

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1400.0f, 0.0f, 150.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona4.Draw(staticShader);



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-600.0f, 0.0f, 150.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona5.Draw(staticShader);







		// Personas Zona de Pinguinos


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-540.0f, 0.0f, -200.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona4.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-540.0f, 0.0f, -800.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona6.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-540.0f, 0.0f, -700.0f));
		model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.065f, 0.065f, 0.065f));
		staticShader.setMat4("model", model);
		persona9.Draw(staticShader);

		//Horizontal
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1400.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona2.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-700.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona8.Draw(staticShader);



		// Personas Pecera Tiburones

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(420.0f, 0.0f, 200.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona7.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(420.0f, 0.0f, -800.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona6.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(420.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.065f, 0.065f, 0.065f));
		staticShader.setMat4("model", model);
		persona9.Draw(staticShader);

		//Horizontal
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1400.0f, 0.0f, 300.0f));
		model = glm::rotate(model, glm::radians(210.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona2.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(700.0f, 0.0f, 300.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		persona8.Draw(staticShader);

		

		//ANIMACION TIBURON

		// Tiburon 1

		// Boca Tiburon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 50.0f, -160.0f));
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


		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
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


		// Tiburon 2

		// Boca Tiburon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 150.0f, -160.0f));
		model = glm::translate(model, tshark01);
		tmp = model = glm::rotate(model, glm::radians(rotshark01), glm::vec3(0.0f, 1.0f, 0.0f));
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


		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
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


		// Tiburon 3

		// Boca Tiburon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1050.0f, 250.0f, -390.0f));
		model = glm::translate(model, tshark02);
		tmp = model = glm::rotate(model, glm::radians(rotshark02), glm::vec3(0.0f, 1.0f, 0.0f));
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


		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
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

		// Tiburon 4

		// Boca Tiburon
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1050.0f, 350.0f, -380.0f));
		model = glm::translate(model, tshark03);
		tmp = model = glm::rotate(model, glm::radians(rotshark03), glm::vec3(0.0f, 1.0f, 0.0f));
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


		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
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





		//ANIMACION MEDUSAS

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(950.0f, 40.0f, -600.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 02
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(930.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 03
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(910.0f, 40.0f, -600.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 04
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(890.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 05
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(870.0f, 40.0f, -600.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 06
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(850.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 07
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(830.0f, 40.0f, -600.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 08
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(810.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 09
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(790.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 10
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(770.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 11
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(750.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 12
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(730.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 13
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(710.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 14
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(690.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 15
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(670.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 16
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(650.0f, 50.0f, -600.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		////////////////////

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1420.0f, 40.0f, -560.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 02
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1400.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 03
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1375.0f, 40.0f, -560.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 04
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1350.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 05
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1325.0f, 40.0f, -560.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 06
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1300.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 07
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1275.0f, 40.0f, -560.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 08
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1250.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 09
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1225.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 10
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1200.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 11
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1175.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 12
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1150.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 13
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1125.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 14
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1100.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa02);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 15
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1075.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa03);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);

		//ANIMACION MEDUSA 16
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1050.0f, 50.0f, -560.0f));
		model = glm::translate(model, tmedusa04);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		staticShader.setMat4("model", model);
		medusa.Draw(staticShader);


		//Pinguino

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1300.0f, 50.0f, -670.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1335.0f, 50.0f, -670.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1317.5f, 50.0f, -650.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(10.9f, 10.9f, 10.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(10.9f, 10.9f, 10.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(10.9f, 10.9f, 10.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		// 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1050.0f, 50.0f, -660.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 50.0f, -670.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);



		model = glm::translate(glm::mat4(1.0f), glm::vec3(-960.0f, 50.0f, -670.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-910.0f, 50.0f, -660.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);



		// 3

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1050.0f, 50.0f, -480.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 50.0f, -470.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);



		model = glm::translate(glm::mat4(1.0f), glm::vec3(-960.0f, 50.0f, -470.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-910.0f, 50.0f, -480.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);



		// 4

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-800.0f, 50.0f, -280.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		//CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		//TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		//patasPin.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-760.0f, 50.0f, -280.0f));
		model = glm::translate(model, glm::vec3(posXPin, 0, posZPin));
		tmp = model = glm::rotate(model, glm::radians(rotPin), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(movCabezaPin), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		CabezaPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		TorsoPin.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPatasPin), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.9f, 20.9f, 20.9f));
		staticShader.setMat4("model", model);
		patasPin.Draw(staticShader);




		
		//Banio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 0.0f, -760.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.075f));
		model = glm::scale(model, glm::vec3(0.7f, 1.0f, 0.6f));
		staticShader.setMat4("model", model);
		banio.Draw(staticShader);


		//Acuario
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, -110.0f, -1400.0f));
		model = glm::scale(model, glm::vec3(300.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		acuario.Draw(staticShader);

		//Acuario vidrio
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente 
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, -167.0f));
		model = glm::rotate(model, glm::radians(92.24f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		model = glm::scale(model, glm::vec3(0.74f, 0.955f, 1.27f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glDisable(GL_BLEND);


		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente Peces
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 0.0f, 490.0f));
		model = glm::rotate(model, glm::radians(-1.24f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::scale(model, glm::vec3(11.19f, 5.0f, 5.27f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glDisable(GL_BLEND);


		// Caja Transparente tiburones
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1465.0f, 0.0f, -445.0f));
		model = glm::rotate(model, glm::radians(-2.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::scale(model, glm::vec3(12.6f, 5.0f, 9.03f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glDisable(GL_BLEND);


		//Vegetacion
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-570.0f, -72.0f, -380.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(110.0f));
		staticShader.setMat4("model", model);
		vegetacion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-350.0f, -72.0f, -380.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(110.0f));
		staticShader.setMat4("model", model);
		vegetacion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -72.0f, -380.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(110.0f));
		staticShader.setMat4("model", model);
		vegetacion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -72.0f, -380.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(110.0f));
		staticShader.setMat4("model", model);
		vegetacion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, -72.0f, -380.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(110.0f));
		staticShader.setMat4("model", model);
		vegetacion.Draw(staticShader);




		// -------------------------------------------------------------------------------------------------------------------------
		// Primitivas
		// -------------------------------------------------------------------------------------------------------------------------

		//En cada primitiva las luces van después de las traslaciones para que tenga material la primitiva
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(4, 2, 1));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.5f, 0.5f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f); //más 0 es más brillante menos 0 menos brillante
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		CrearCuboRenderizar();  //dibuja el cubo

	

		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(-8, 2, 1));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.1745f, 0.01175f, 0.01175f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.61424f, 0.04136f, 0.04136f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.727811f, 0.626959f, 0.626959f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.0000032f); //más 0 es más brillante menos 0 menos brillante
		staticShader.setFloat("material_shininess", 0.6f);
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		CrearPiramideCuadrangularRenderizar();
		

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(6, 2, 1));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 1.0f, 0.f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.633f, 0.727811f, 0.633f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00000032f); //más 0 es más brillante menos 0 menos brillante
		staticShader.setFloat("material_shininess", 0.6f);
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		staticShader.setMat4("model", model);
		CrearPiramideRenderizar();

		
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(-1, 2, 1));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(212.0f/255.0f, 175.0f/255.0f, 55.0f/255.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.000000032f); //más 0 es más brillante menos 0 menos brillante
		staticShader.setFloat("material_shininess", 0.4f);
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(-3, 2, 1));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(192.0f/255.0f, 192.0f/255.0f, 192.0f/255.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.50754f, 0.50754f, 0.50754f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.508273f, 0.508273f, 0.508273f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.000000032f); //más 0 es más brillante menos 0 menos brillante
		staticShader.setFloat("material_shininess", 0.4f);
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.4f));
		staticShader.setMat4("model", model);
		CrearConoRenderizar();

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(70.0f, 20.0f, 42.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(10, -3, 21));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		model = glm::scale(model, glm::vec3(1.0f));
		myShader2.setMat4("model", model);
		CrearTorusRenderizar();
	

		// -------------------------------------------------------------------------------------------------------------------------
		// TETERA
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 100.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.3098f, 0.5843f, 0.5411f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(15.0f, 1.5f, 15.0f));
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.1f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.3098f, 0.5843f, 0.5411f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();
		
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));

		staticShader.setVec3("pointLight[0].position", glm::vec3(6.0f, 2.0f, 3.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.3098f, 0.5843f, 0.5411f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.97f, 1.0f, 0.97f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.97f, 1.0f, 0.97f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.95f, 1.0f, 0.95f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.93f, 1.0f, 0.93f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.90f, 1.0f, 0.90f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.50f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.50f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();


		//inicia asa

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(7.0f, -15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-2.5f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.8f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.28f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.3f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(8.0f, -13.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.5f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.8f, 1.5f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.5f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.5f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.0f, 2.5f, 1.0));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();





		// -------------------------------------------------------------------------------------------------------------------------
		// BARCO
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-350.0f, 0.0f, -600.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 6.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 0.894f, 0.710f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.633f, 0.727811f, 0.633f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.1f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 0.894f, 0.710f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.633f, 0.727811f, 0.633f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();



		// -------------------------------------------------------------------------------------------------------------------------
		// TOROIDE a partir de cilindros
		// -------------------------------------------------------------------------------------------------------------------------


		model = glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f);
		tmp = model = glm::translate(model, glm::vec3(45.0f, 30.0f, 20.0f));
		//model = glm::rotate(tmp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(tmp, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(10, -3, 21));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);
		myShader2.setMat4("model", model);
		//SphereRenderizar();
		CrearCilindroRenderizar();

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::rotate(tmp, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);

		myShader2.setMat4("model", model);
		//SphereRenderizar();
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);

		//myShader2.setMat4("model", model);
		//SphereRenderizar();
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));


		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));


		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();



		model = glm::rotate(tmp, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(70.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);

		//myShader2.setMat4("model", model);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(80.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();


		model = glm::rotate(tmp, glm::radians(100.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(110.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(130.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(140.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(150.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(160.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(170.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(190.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(200.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(210.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(220.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(230.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(240.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(250.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(260.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(280.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(290.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(300.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(310.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(320.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(330.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();


		model = glm::rotate(tmp, glm::radians(340.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(350.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::rotate(tmp, glm::radians(360.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		//myShader2.setMat4("model", model);

		staticShader.setMat4("model", model);

		//myOtherShader->setMat4("model", model);
		//meshSphere->Draw(*myOtherShader);
		myShader2.setMat4("model", model);
		CrearCilindroRenderizar();



		// -------------------------------------------------------------------------------------------------------------------------
		// TETERA CON TOROIDE
		// -------------------------------------------------------------------------------------------------------------------------

		staticShader.use();

		tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 2.0f, 40.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		CrearTeteraRenderizar();


		model = glm::translate(model, glm::vec3(5.0f, 6.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearAsaTeteraRenderizar();

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -8.0f));
		model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		staticShader.setMat4("model", model);
		CrearBoquillaTeteraRenderizar();



		// -------------------------------------------------------------------------------------------------------------------------
		// ICE MAN
		// -------------------------------------------------------------------------------------------------------------------------

		//CUERPO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-750.0f, 50.0f, -470.0f));
		//model = glm::translate(model, glm::vec3(270.0f, 15.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(15.0f, 1.5f, 15.0f)); //escala
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.1f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); //color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));

		staticShader.setVec3("pointLight[0].position", glm::vec3(6.0f, 2.0f, 3.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); //color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.97f, 1.0f, 0.97f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.97f, 1.0f, 0.97f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.95f, 1.0f, 0.95f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.93f, 1.0f, 0.93f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.90f, 1.0f, 0.90f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.50f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();



		//CABEZA 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-750.0f, 110.0f, -470.0f));
		//model = glm::translate(model, glm::vec3(270.0f, 40.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f)); //escala
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.1f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); //color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));

		staticShader.setVec3("pointLight[0].position", glm::vec3(6.0f, 2.0f, 3.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); //color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.05f, 1.0f, 1.05f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.97f, 1.0f, 0.97f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.97f, 1.0f, 0.97f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.95f, 1.0f, 0.95f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.93f, 1.0f, 0.93f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.90f, 1.0f, 0.90f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.50f));
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		//SOMBRERO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-750.0f, 150.0f, -470.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(14.0f, 0.1f, 14.0f)); //escala
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		//COPA SOMBRERO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-750.0f, 162.0f, -470.0f));
		//model = glm::translate(model, glm::vec3(270.0f, 60.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 10.0f, 5.0f)); //escala
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();



		
		// ------------------------------------------------------------------------------------------------------------------------ -
		// PINGUINO AVATAR
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-900.0f, 110.0f, -570.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-912.0f, 118.0f, -570.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.0f, 1.0f, 3.0f)); //escala
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();// brazo derecho

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-887.0f, 118.0f, -574.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//model = glm::translate(model, glm::vec3(-12.0f, 10.0f, 1000.0f));// es el único traslación pa mover
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.0f, 1.0f, 3.0f)); //escala
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();// brazo izquierdo
		

		//pico
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(-8, 2, 1));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 0.5f, 0.0f));  // Ambiente naranja
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 0.5f, 0.0f));  // Difuso naranja
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 0.5f, 0.0f));  // Especular naranja
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.0000032f); //más 0 es más brillante menos 0 menos brillante
		staticShader.setFloat("material_shininess", 0.6f);
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(0.0f, 18.0f, 991.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-900.0f, 128.0f, -579.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +461.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, +12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
	//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 2.0f));
		staticShader.setMat4("model", model);
		CrearPiramideCuadrangularRenderizar();

		//Esfera blanca cuerpo barriga
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(0.0f, 10.0f, 995.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-900.0f, 120.0f, -575.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, +5.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//sefera del cuerpo barriga

		//Esfera Negra cuerpo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(0.0f, 9.0f, 1000.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-900.0f, 119.0f, -570.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, -5.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//sefera del cuerpo

		//Esfera negra cabeza
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(0.0f, 15.0f, 1000.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-900.0f, 125.0f, -570.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, -5.0f));
		model = glm::scale(model, glm::vec3(7.5f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja cabeza

		//Esfera cabeza ojo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.5f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(-2.0f, 20.0f, 993.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-898.0f, 130.0f, -579.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, +10.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja ojo derecho

		//Esfera cabeza pupila ojo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.5f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(-2.0f, 20.0f, 992.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-898.0f, 130.0f, -577.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));

		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, +10.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja pupila ojo derecho

		//Esfera cabeza ojo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(2.0f, 20.0f, 993.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-903.0f, 130.0f, -578.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, +10.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja cabeza ojo izquierdo

		//Esfera cabeza pupila ojo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(2.0f, 20.0f, 991.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-903.0f, 130.0f, -577.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, +11.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja cabeza pupila ojo izquierdo


		//Esfera pata naranja
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(3.0f, 1.0f, 1000.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-897.0f, 111.0f, -570.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -7.0f, -5.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja pata derecha

		//Esfera pata naranja
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	//	model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 1000.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(-903.0f, 111.0f, -570.0f));
		model = glm::translate(model, glm::vec3(+700.0f, -90.0f, +460.0f));
		//Traslación individual
		model = glm::translate(model, glm::vec3(0.0f, -7.0f, -5.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();//dibuja pata izquierda
//acaba pinguino






		//Esfera Negra
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(100.0f, 10.0f, 100.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();

		//Esfera blanca
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(100.0f, 40.0f, 100.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();


		//Plano naranja
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)


		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		}
		else if (activeCamera2) {
			projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera2.GetViewMatrix();
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3)
		{
			projection = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3.GetViewMatrix();
			staticShader.setVec3("viewPos", camera3.Position);
		}


		//projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		myOtherShader->setMat4("projection", projection);

		//view = glm::lookAt(camera.Position,camera.Front, camera.Up);//*****************<<<<<<
		myOtherShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(80.0f, 5.0f, 60.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", model);

		myOtherShader->setMat4("model", model);
		meshPlane->Draw(*myOtherShader);


		//Circulo rosa
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		rosa.Draw(staticShader);


		// Aplicamos transformaciones de proyección y cámara (si las hubiera)


		if(activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		}
		else if (activeCamera2) {
			projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera2.GetViewMatrix();
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3)
		{
			projection = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3.GetViewMatrix();
			staticShader.setVec3("viewPos", camera3.Position);
		}
		//projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		myOtherShader->setMat4("projection", projection);

		//view = glm::lookAt(camera.Position, camera.View, camera.Up);
		myOtherShader->setMat4("view", view);

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(100.0f, 5.0f, 60.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", model);

		myOtherShader->setMat4("model", model);
		meshCircle->Draw(*myOtherShader);



		//Triangulo rojo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		rojo.Draw(staticShader);
		//rosa.Draw(staticShader);


		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		}
		else if (activeCamera2) {
			projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera2.GetViewMatrix();
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3)
		{
			projection = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3.GetViewMatrix();
			staticShader.setVec3("viewPos", camera3.Position);
		}

		//projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		myOtherShader->setMat4("projection", projection);

		//view = glm::lookAt(camera.Position, camera.View, camera.Up);
		myOtherShader->setMat4("view", view);

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(110.0f, 3.0f, 130.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", model);

		myOtherShader->setMat4("model", model);
		meshTriangle->Draw(*myOtherShader);
		staticShader.use();


		
		// Zunesha
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -350.0f));
		model = glm::translate(model, glm::vec3(0.0f, -500.0f, -1500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(120.0f, 15.0f, 120.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		staticShader.setMat4("model", model);
		//zunesha.Draw(staticShader);

		glm::mat4 modelT = glm::mat4(1.0f);
		
		// view/projection transformations

		glm::mat4 projectionT;
		glm::mat4 viewT;

		if (activeCamera) {
			projectionT = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			viewT = camera.GetViewMatrix();
			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		}
		else if (activeCamera2) {
			projectionT = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			viewT = camera2.GetViewMatrix();
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3)
		{
			projectionT = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			viewT = camera3.GetViewMatrix();
			staticShader.setVec3("viewPos", camera3.Position);
		}
		//glm::mat4 projectionT = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		//glm::mat4 viewT = camera.GetViewMatrix();

		// -------------------------------------------------------------------------------------------------------------------------
	// CUBO TEXTURIZADO
	// -------------------------------------------------------------------------------------------------------------------------
		texturaShader.use();
		texturaShader.setMat4("projection", projectionT);
		texturaShader.setMat4("view", viewT);

		modelT = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, -100.0f));

		modelT = glm::scale(modelT, glm::vec3(2.0f, 50.0f, 100.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[8]);
		glBindTexture(GL_TEXTURE_2D, t_brick_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);
		//myDataRenderizar();

		modelT = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, -100.0f));
		modelT = glm::scale(modelT, glm::vec3(100.0f, 50.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[8]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);


		///////////inicia CASA SENCILLA


		/// primeramente PUERTA con ventana CORREDIZA se acciona con F3 y F4
		glm::mat4 tmp61 = glm::mat4(1.0f);
		tmp61 = modelT = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 30.0f, 1000.0f));
		modelT = glm::translate(modelT, glm::vec3(0.0f+door_offset, 70.0f, -400.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 200.0f, 300.0f));


		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);




		//VENTANA muro hacia piscina
		modelT = glm::translate(tmp61, glm::vec3(0.0f+door_offset, 90.0f, -400.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(6.0f, 60.0f, 100.0f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_ventana_jpg); 
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		modelT = glm::translate(tmp61, glm::vec3(0.0f, 70.0f, -200.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 200.0f, 300.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_brick_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);



		//VENTANA muro hacia el mar
		modelT = glm::translate(tmp61, glm::vec3(0.0f, 90.0f, -200.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(6.0f, 60.0f, 100.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_ventana_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		modelT = glm::translate(tmp61, glm::vec3(145.0f, 70.0f, -300.0f));
		modelT = glm::rotate(modelT, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 200.0f, 195.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_brick_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		modelT = glm::translate(tmp61, glm::vec3(-145.0f, 70.0f, -300.0f));
		modelT = glm::rotate(modelT, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 200.0f, 195.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_brick_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		//PUERTA muro hacia centro
		modelT = glm::translate(tmp61, glm::vec3(-145.0f, 45.0f, -250.0f));
		modelT = glm::rotate(modelT, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(6.0f, 130.0f, 40.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_puerta_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);



		modelT = glm::translate(tmp61, glm::vec3(0.0f, 70.0f, -300.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelT = glm::translate(modelT, glm::vec3(95.0f, 0.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 200.0f, 300.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_techo_jpg);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Techo
		glBindVertexArray(0);



		///////////inicia MESA SENCILLA

		glm::mat4 tmp62 = glm::mat4(1.0f);
		tmp62 = modelT = glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 0.0f, 1052.0f));
		modelT = glm::translate(modelT, glm::vec3(0.0f, 40.0f, -400.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelT = glm::scale(modelT, glm::vec3(2.0f, 70.0f, 110.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		//PATAS de la mesa
		modelT = glm::translate(tmp62, glm::vec3(0.0f - 30.0f, 20.0f, -400.0f - 40.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 4.0f, 40.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		modelT = glm::translate(tmp62, glm::vec3(0.0f + 30.0f, 20.0f, -400.0f - 40.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 4.0f, 40.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		modelT = glm::translate(tmp62, glm::vec3(0.0f - 30.0f, 20.0f, -400.0f + 40.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 4.0f, 40.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);

		modelT = glm::translate(tmp62, glm::vec3(0.0f + 30.0f, 20.0f, -400.0f + 40.0f));
		modelT = glm::rotate(modelT, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(4.0f, 4.0f, 40.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", modelT);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[21]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);



		// -------------------------------------------------------------------------------------------------------------------------
		// FBX
		// -------------------------------------------------------------------------------------------------------------------------

		glm::mat4 modelFBX = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projectionFBX;
		glm::mat4 viewFBX;



		// -------------------------------------------------------------------------------------------------------------------------
		// OLAS DE PECERA DE ENMEDIO
		// -------------------------------------------------------------------------------------------------------------------------
		// Activamos el shader de Phong
		wavesShader.use();
		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




		if (activeCamera) {
			// Aplicamos transformaciones de proyección y cámara (si las hubiera)
			projectionFBX = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			viewFBX = camera.GetViewMatrix();
			wavesShader.setMat4("projection", projectionFBX);
			wavesShader.setMat4("view", viewFBX);
			staticShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		}
		else if (activeCamera2) {
			projectionFBX = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			viewFBX = camera2.GetViewMatrix();
			wavesShader.setMat4("projection", projectionFBX);
			wavesShader.setMat4("view", viewFBX);
			staticShader.setVec3("viewPos", camera2.Position);
		}
		else if (activeCamera3) {
			projectionFBX = glm::perspective(glm::radians(camera3.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			viewFBX = camera3.GetViewMatrix();
			wavesShader.setMat4("projection", projectionFBX);
			wavesShader.setMat4("view", viewFBX);
			staticShader.setVec3("viewPos", camera3.Position);
		}

	




		// Aplicamos transformaciones del modelo	
		modelFBX = glm::mat4(1.0f);
		modelFBX = glm::translate(modelFBX, glm::vec3(-20.0f, 75.0f, -220.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelFBX = glm::scale(modelFBX, glm::vec3(2.5f));
		modelFBX = glm::scale(modelFBX, glm::vec3(5.5f, 1.0f, 2.0f));
		modelFBX = glm::rotate(modelFBX, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wavesShader.setMat4("model", modelFBX);
		wavesShader.setFloat("time", wavesTime);
		wavesShader.setFloat("radius", 4.0f);
		wavesShader.setFloat("height", 3.0f);
		olaFBX.Draw(wavesShader);
		


		// Aplicamos transformaciones del modelo	
		modelFBX = glm::mat4(1.0f);
		modelFBX = glm::translate(modelFBX, glm::vec3(-1080.0f, 439.0f, 555.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelFBX = glm::scale(modelFBX, glm::vec3(2.5f));
		modelFBX = glm::scale(modelFBX, glm::vec3(4.56f, 1.0f, 3.40f));
		modelFBX = glm::scale(modelFBX, glm::vec3(4.0f));
		modelFBX = glm::rotate(modelFBX, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wavesShader.setMat4("model", modelFBX);
		wavesShader.setFloat("time", wavesTime);
		wavesShader.setFloat("radius", 3.0f);
		wavesShader.setFloat("height", 3.0f);
		olaFBX.Draw(wavesShader);



		// Aplicamos transformaciones del modelo	
		modelFBX = glm::mat4(1.0f);
		modelFBX = glm::translate(modelFBX, glm::vec3(1000.0f, 439.0f, -300.0f));
		modelFBX = glm::rotate(modelFBX, glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelFBX = glm::scale(modelFBX, glm::vec3(52.0f, 1.0f, 55.0f));
		modelFBX = glm::rotate(modelFBX, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wavesShader.setMat4("model", modelFBX);
		wavesShader.setFloat("time", wavesTime);
		wavesShader.setFloat("radius", 3.0f);
		wavesShader.setFloat("height", 3.0f);
		olaFBX.Draw(wavesShader);
		wavesTime += 0.01;
		glDisable(GL_BLEND);
		
	



		//LUNA
		lunaShader.use();
		lunaShader.setMat4("projection", projectionFBX);
		lunaShader.setMat4("view", viewFBX);

		modelFBX = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1800.0f, 0.0f));
		modelFBX = glm::rotate(modelFBX, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelFBX = glm::scale(modelFBX, glm::vec3(700.0f));
		fbxShader.setMat4("model", modelFBX);
		moonFBX.Draw(fbxShader);

		lunaShader.setMat4("model", modelFBX);
		lunaShader.setFloat("time", proceduralTime2);
		lunaShader.setFloat("radius", 50.0f);
		lunaShader.setFloat("height", 10.0f);
		proceduralTime2 += 0.02;



		

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		if (activaSkyboxDia) {
			skyboxShader.use();
			skybox.Draw(skyboxShader, view, projection, camera);
		}
		else
		{
			skyboxShader.use();
			skybox1.Draw(skyboxShader, view, projection, camera);
		}
		

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
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		if (activeCamera)
			camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		else if (activeCamera2)
			camera2.ProcessKeyboard(FORWARD, (float)deltaTime);
		else if (activeCamera3)
			camera3.ProcessKeyboard(FORWARD, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		if (activeCamera)
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		else if (activeCamera2)
			camera2.ProcessKeyboard(BACKWARD, (float)deltaTime);
		else if (activeCamera3)
			camera3.ProcessKeyboard(BACKWARD, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		if (activeCamera)
			camera.ProcessKeyboard(LEFT, (float)deltaTime);
		else if (activeCamera2)
			camera2.ProcessKeyboard(LEFT, (float)deltaTime);
		else if (activeCamera3)
			camera3.ProcessKeyboard(LEFT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		if (activeCamera)
			camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		else if (activeCamera2)
			camera2.ProcessKeyboard(RIGHT, (float)deltaTime);
		else if (activeCamera3)
			camera3.ProcessKeyboard(RIGHT, (float)deltaTime);
	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		if (activeCamera3) {
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(camera3.Position.x, camera3.Position.y, camera3.Position.z));
			model = glm::rotate(model, glm::radians(180.0f + rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			forwardView = glm::vec3(viewVector);
			forwardView = glm::normalize(forwardView);

			position = position + scaleV * forwardView;
			camera3.Front = forwardView;
			camera3.ProcessKeyboard(FORWARD, deltaTime);
			camera3.Position = position;
			camera3.Position.z -= 5.0f; //camera.Position.y += 1.7f
			camera3.Position.y = 5.0f;
			camera3.Position -= forwardView;
		}


	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (activeCamera3) {
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(camera3.Position.x, camera3.Position.y, camera3.Position.z));
			model = glm::rotate(model, glm::radians(180.0f + rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			forwardView = glm::vec3(viewVector);
			forwardView = glm::normalize(forwardView);

			position = position - scaleV * forwardView;
			camera3.Front = forwardView;
			camera3.ProcessKeyboard(BACKWARD, deltaTime);
			camera3.Position = position;
			camera3.Position.z += 5.0f;
			camera3.Position.y = 5.0f;
			camera3.Position -= forwardView;
		}

	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

		if (activeCamera3) {
			rotateCharacter += 3.0f;

			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(camera3.Position.x, camera3.Position.y, camera3.Position.z));
			model = glm::rotate(model, glm::radians(180.0f + rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			forwardView = glm::vec3(viewVector);
			forwardView = glm::normalize(forwardView);

			position = position + scaleV * forwardView;
			camera3.Front = forwardView;
			camera3.ProcessKeyboard(LEFT, (float)deltaTime);
			camera3.Position = position;
			camera3.Position.y += 1.7f;
			camera3.Position -= forwardView;


		}

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (activeCamera3) {
			rotateCharacter -= 3.0f;

			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(camera3.Position.x, camera3.Position.y, camera3.Position.z));
			model = glm::rotate(model, glm::radians(180.0f + rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			forwardView = glm::vec3(viewVector);
			forwardView = glm::normalize(forwardView);

			position = position - scaleV * forwardView;
			camera3.Front = forwardView;
			camera3.ProcessKeyboard(RIGHT, (float)deltaTime);
			camera3.Position = position;
			camera3.Position.y += 1.7f;
			camera3.Position -= forwardView;
		}

	}
	

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		camera2.Front = glm::vec3(-0.47f, -0.41f, -0.78f);
		activeCamera = false;
		activeCamera2 = true; //isometrico
		activeCamera3 = false;
		
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		activeCamera = true; //normal
		activeCamera2 = false;
		activeCamera3 = false;
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		activeCamera = false;//camara avatar
		activeCamera2 = false;
		activeCamera3 = true;

	}

		
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
		door_offset += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
		door_offset -= 3.0f;
	

	
	//PROYECTO

//Teclas mueve cola pez

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		movCola--;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		movCola++;


	// Teclas movimiento Tiburon


	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		movAletasShark -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		movAletasShark += 0.2f;

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		movBocaShark -= 0.15f;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		movBocaShark += 0.15f;


	//Torniquetes animacion
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		giroTorniquete_x++;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		giroTorniquete_x--;


	// Teclas Pinguino

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		movCabezaPin += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		movCabezaPin -= 0.2f;





	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		music();

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
		lastY = ypos; //ypos
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY -ypos; // reversed since y-coordinates go from bottom to top lastY-ypos

	lastX = xpos;
	lastY =ypos;  //ypos
	if (activeCamera)
		camera.ProcessMouseMovement(xoffset, yoffset); //xoffset,yoffset
	else if (activeCamera2)
		camera2.ProcessMouseMovement(xoffset, yoffset); //xoffset,yoffset
	else if (activeCamera3)
		camera3.ProcessMouseMovement(xoffset, yoffset); //xoffset,yoffset
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (activeCamera)
		camera.ProcessMouseScroll(yoffset);
	else if (activeCamera2)
		camera2.ProcessMouseMovement(xoffset, yoffset); //xoffset,yoffset
	else if (activeCamera3)
		camera3.ProcessMouseMovement(xoffset, yoffset); //xoffset,yoffset
}
