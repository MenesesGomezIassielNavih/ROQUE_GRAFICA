/*---------------------------------------------------------*/
/* ----------------  Proyecto Final              ----------*/
/*-----------------       -------------------------*/
/*---------------------------------------------------------*/

#include <Windows.h>
#include <mmsystem.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
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
GLuint VAO[10], VBO[10], IBO[10];
GLuint indexCount[10];
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


///////////////////TEXTURIZADO
unsigned int		t_smile,
					t_toalla,
					t_unam,
					t_white,
					t_brick_png,
					t_wood_png,
					t_verdura_png;

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
	t_smile = generateTextures("resources/Texturas1/awesomeface.png", 1);
	t_toalla = generateTextures("resources/Texturas1/toalla.tga", 0);
	t_unam = generateTextures("resources/Texturas1/escudo_unam.jpg", 0);
	t_white = generateTextures("resources/Texturas1/white.jpg", 0);
	t_brick_png = generateTextures("resources/Texturas1/bricks.jpg", 0);
	t_wood_png = generateTextures("resources/Texturas1/wood01.png", 0);
	t_verdura_png = generateTextures("resources/Texturas1/verdura.png", 1);
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
	glBindTexture(GL_TEXTURE_2D, t_brick_png);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

///////////////////   FIN    TEXTURIZADO


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


// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
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

glm::vec3 pez01(0.0f, -1.0f, -1.0f);
glm::vec3 pez02(0.0f, -1.0f, -1.0f);
glm::vec3 tfish(0.0f, -1.0f, -1.0f);
glm::vec3 tfish3(0.0f, -1.0f, -1.0f);

glm::vec3 pez011(0.0f, -1.0f, -1.0f);
glm::vec3 pez021(0.0f, -1.0f, -1.0f);
glm::vec3 tfish1(0.0f, -1.0f, -1.0f);
glm::vec3 tfish31(0.0f, -1.0f, -1.0f);



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
float

posXsharkInc = 0.0f,
rotsharkInc = 0.0f,
movColaSharkInc = 0.0f,
movAletasSharkInc = 0.0f,
movBocaSharkInc = 0.0f,

posXmedusaInc = 0.0f,

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


	/*KeyFrame[FrameIndex].posXshark = posXshark;
	KeyFrame[FrameIndex].rotshark = rotshark;
	KeyFrame[FrameIndex].movColaShark = movColaShark;
	KeyFrame[FrameIndex].movAletasShark = movAletasShark;
	KeyFrame[FrameIndex].movBocaShark = movBocaShark;

	KeyFrame[FrameIndex].posXmedusa = posXmedusa;

	KeyFrame[FrameIndex].posXniña = posXniña;
	KeyFrame[FrameIndex].posZniña = posZniña;
	KeyFrame[FrameIndex].movBrazoDerNiña = movBrazoDerNiña;
	KeyFrame[FrameIndex].movBrazoIzqNiña = movBrazoIzqNiña;
	KeyFrame[FrameIndex].rotPiernaDerNiña = rotPiernaDerNiña;
	KeyFrame[FrameIndex].rotPiernaIzqNiña = rotPiernaIzqNiña;
	KeyFrame[FrameIndex].rotCabezaNiña = rotCabezaNiña;

	KeyFrame[FrameIndex].posXPin = posXPin;
	KeyFrame[FrameIndex].posZPin = posZPin;
	KeyFrame[FrameIndex].rotPin = rotPin;
	KeyFrame[FrameIndex].rotPatasPin = rotPatasPin;
	KeyFrame[FrameIndex].movCabezaPin = movCabezaPin;*/





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

	/*
	posXshark = KeyFrame[0].posXshark;
	rotshark = KeyFrame[0].rotshark;
	movColaShark = KeyFrame[0].movColaShark;
	movAletasShark = KeyFrame[0].movAletasShark;
	movBocaShark = KeyFrame[0].movBocaShark;
	
	posXmedusa = KeyFrame[0].posXmedusa;

	posXniña = KeyFrame[0].posXniña;
	posZniña = KeyFrame[0].posZniña;
	movBrazoDerNiña = KeyFrame[0].movBrazoDerNiña;
	movBrazoIzqNiña = KeyFrame[0].movBrazoIzqNiña;
	rotPiernaDerNiña = KeyFrame[0].rotPiernaDerNiña;
	rotPiernaIzqNiña = KeyFrame[0].rotPiernaIzqNiña;
	rotCabezaNiña = KeyFrame[0].rotCabezaNiña;

	posXPin = KeyFrame[0].posXPin;
	posZPin = KeyFrame[0].posZPin;
	rotPin = KeyFrame[0].rotPin;
	rotPatasPin = KeyFrame[0].rotPatasPin;
	movCabezaPin = KeyFrame[0].movCabezaPin;*/


	
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

	/*
	posXsharkInc = (KeyFrame[playIndex + 1].posXshark - KeyFrame[playIndex].posXshark) / i_max_steps;
	rotsharkInc = (KeyFrame[playIndex + 1].rotshark - KeyFrame[playIndex].rotshark) / i_max_steps;
	movColaSharkInc = (KeyFrame[playIndex + 1].movColaShark - KeyFrame[playIndex].movColaShark) / i_max_steps;
	movAletasSharkInc = (KeyFrame[playIndex + 1].movAletasShark - KeyFrame[playIndex].movAletasShark) / i_max_steps;
	movBocaSharkInc = (KeyFrame[playIndex + 1].movBocaShark - KeyFrame[playIndex].movBocaShark) / i_max_steps;
	
	posXmedusaInc = (KeyFrame[playIndex + 1].posXmedusa - KeyFrame[playIndex].posXmedusa) / i_max_steps;

	posXniñaInc = (KeyFrame[playIndex + 1].posXniña - KeyFrame[playIndex].posXniña) / i_max_steps;
	posZniñaInc = (KeyFrame[playIndex + 1].posZniña - KeyFrame[playIndex].posZniña) / i_max_steps;
	movBrazoDerNiñaInc = (KeyFrame[playIndex + 1].movBrazoDerNiña - KeyFrame[playIndex].movBrazoDerNiña) / i_max_steps;
	movBrazoIzqNiñaInc = (KeyFrame[playIndex + 1].movBrazoIzqNiña - KeyFrame[playIndex].movBrazoIzqNiña) / i_max_steps;
	rotPiernaDerNiñaInc = (KeyFrame[playIndex + 1].rotPiernaDerNiña - KeyFrame[playIndex].rotPiernaDerNiña) / i_max_steps;
	rotPiernaIzqNiñaInc = (KeyFrame[playIndex + 1].rotPiernaIzqNiña - KeyFrame[playIndex].rotPiernaIzqNiña) / i_max_steps;
	rotCabezaNiñaInc = (KeyFrame[playIndex + 1].rotCabezaNiña - KeyFrame[playIndex].rotCabezaNiña) / i_max_steps;


	posXPinInc = (KeyFrame[playIndex + 1].posXPin - KeyFrame[playIndex].posXPin) / i_max_steps;
	posZPinInc = (KeyFrame[playIndex + 1].posZPin - KeyFrame[playIndex].posZPin) / i_max_steps;
	rotPinInc = (KeyFrame[playIndex + 1].rotPin - KeyFrame[playIndex].rotPin) / i_max_steps;
	rotPatasPinInc = (KeyFrame[playIndex + 1].rotPatasPin - KeyFrame[playIndex].rotPatasPin) / i_max_steps;
	movCabezaPinInc = (KeyFrame[playIndex + 1].movCabezaPin - KeyFrame[playIndex].movCabezaPin) / i_max_steps;*/




}

void animate(void)
{
	////////////////////////////////    SOL
	lightDirection.x = 100 * cos(myVariable);
	lightDirection.y = 100 * sin(myVariable);
	myVariable += 0.01f;

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

	/*
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
	posXmedusa += 0.002f;*/


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

			/*
			posXshark += posXsharkInc;
			rotshark += rotsharkInc;
			movColaShark += movColaSharkInc;
			movAletasShark += movAletasSharkInc;
			movBocaShark += movBocaSharkInc;

			posXmedusa += posXmedusaInc;

			posXniña += posXniñaInc;
			posZniña += posZniñaInc;
			movBrazoDerNiña += movBrazoDerNiñaInc;
			movBrazoIzqNiña += movBrazoIzqNiñaInc;
			rotPiernaDerNiña += rotPiernaDerNiñaInc;
			rotPiernaIzqNiña += rotPiernaIzqNiñaInc;
			rotCabezaNiña += rotCabezaNiñaInc;

			posXPin += posXPinInc;
			posZPin += posZPinInc;
			rotPin += rotPinInc;
			rotPatasPin += rotPatasPinInc;
			movCabezaPin += movCabezaPinInc;*/



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


	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
		/*"resources/skybox/posx.jpg",
		"resources/skybox/negx.jpg",
		"resources/skybox/posy.jpg",
		"resources/skybox/negy.jpg",
		"resources/skybox/posz.jpg",
		"resources/skybox/negz.jpg"
		*/
		/*"resources/skybox/blueAba.jpg",
		"resources/skybox/blueAba.jpg",
		"resources/skybox/blueAba.jpg",
		"resources/skybox/blueAba.jpg",
		"resources/skybox/blueAba.jpg",
		"resources/skybox/blueAba.jpg"
		*/
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);


	//MODELOS MIGUEL PROYECTO

	Model pecera1("resources/objects/models/tanques/pecera1/PeceraYObjetos.obj");
	Model pecera2("resources/objects/models/tanques/pecera2/pecera2yElementos.obj");
	Model pecera3("resources/objects/models/tanques/pecera3/pecera3yTroncos.obj");

	Model tanque("resources/objects/Edificio/tanque.obj");
	Model tanque2("resources/objects/Edificio/tanque2.obj");


	Model colafish("resources/objects/Peses/colafish.obj");
	Model bodyfish("resources/objects/Peses/bodyfish.obj");
	Model pez2("resources/objects/Peses/pez2.obj");
	Model pez3("resources/objects/Peses/Pez3/13009_Coral_Beauty_Angelfish_v1_l3.obj");
	Model pezGris("resources/objects/ModeloPez/fish.obj");

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
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

	Model zuneshaFBX("resources/FBX/Zunesha/Sunicha.fbx");
	Model ladrillo_Texture("resources/Texturas1/bricks.jpg");
	Model madera_Texture("resources/Texturas1/wood01.png");


	Model amarillo("resources/objects/Esfera/amarillo/amarillo.obj");
	Model azul("resources/objects/Esfera/azul/azul.obj");
	Model blanco("resources/objects/Esfera/blanco/blanco.obj");
	Model gris("resources/objects/Esfera/gris/gris.obj");
	Model naranja("resources/objects/Esfera/naranja/naranja.obj");
	Model negro("resources/objects/Esfera/negro/negro.obj");
	Model rojo("resources/objects/Esfera/rojo/rojo.obj");
	Model rosa("resources/objects/Esfera/rosa/rosa.obj");


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
		
		
		/*
		KeyFrame[i].posXshark = 0;
		KeyFrame[i].rotshark = 0;
		KeyFrame[i].movColaShark = 0;
		KeyFrame[i].movAletasShark = 0;
		KeyFrame[i].movBocaShark = 0;

		KeyFrame[i].posXmedusa = 0;

		KeyFrame[i].posXniña = 0;
		KeyFrame[i].posZniña = 0;
		KeyFrame[i].movBrazoDerNiña = 0;
		KeyFrame[i].movBrazoIzqNiña = 0;
		KeyFrame[i].rotPiernaDerNiña = 0;
		KeyFrame[i].rotPiernaIzqNiña = 0;

		KeyFrame[i].posXPin = 0;
		KeyFrame[i].posZPin = 0;
		KeyFrame[i].rotPin = 0;
		KeyFrame[i].rotPatasPin = 0;*/

	}

	//Carga de las PRIMITIVAS
	CrearCubo();
	CrearPiramide();
	CrearPiramideCuadrangular();
	CrearCilindro(512, 1.0f);
	CrearCono(20, 1.0f);
	CrearSphere(1.0, 40, 40);
	LoadTextures();
	myData();
	
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
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		//GLint modelLoc = glGetUniformLocation(staticShader.setFloat("material_shininess", 32.0f), "model");

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion CHAVA
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, -20.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion NINJA
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, -20.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		//--Personaje Caminando

		model = glm::translate(glm::mat4(1.0f), glm::vec3(70.3f, 0.0f, 40.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.3f));	// it's a bit too big for our scene, so scale it down
		// = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		perCaminando.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

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

		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -350.0f));
		model = glm::translate(model, glm::vec3(-2300.0f, 0.0f, -470.0f));
		////model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(120.0f, 15.0f, 120.0f));
		model = glm::scale(model, glm::vec3(135.0f, 15.0f, 140.0f));
		staticShader.setMat4("model", model);
		pecera3.Draw(staticShader);

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



		//clima frio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(190.0f, 0.0f, 275.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		igloo.Draw(staticShader);


		//Banio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, -260.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015f));
		staticShader.setMat4("model", model);
		banio.Draw(staticShader);


		//Acuario
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, -220.0f, -1400.0f));
		model = glm::scale(model, glm::vec3(300.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		acuario.Draw(staticShader);

		//Acuario vidrio
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente 
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 10.0f, -167.0f));
		model = glm::rotate(model, glm::radians(92.24f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		model = glm::scale(model, glm::vec3(0.74f, 0.955f, 1.27f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);


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
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
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
		

		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		staticShader.setVec3("pointLight[0].position", glm::vec3(-3, 2, 1));
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
		// ICE MAN
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(270.0f, 15.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(15.0f, 1.5f, 15.0f)); //escala
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
		model = glm::translate(model, glm::vec3(270.0f, 40.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f)); //escala
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
		model = glm::translate(model, glm::vec3(270.0f, 55.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(14.0f, 0.1f, 14.0f)); //escala
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();

		//COPA SOMBRERO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(270.0f, 60.0f, 320.0f));// es el único traslación pa mover
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); // color
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 10.0f, 5.0f)); //escala
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		staticShader.setMat4("model", model);
		CrearCilindroRenderizar();



		// ------------------------------------------------------------------------------------------------------------------------ -
		// PINGUINO
		// -------------------------------------------------------------------------------------------------------------------------
		texturaShader.use();


		//Esfera Negra
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		staticShader.setMat4("model", model);
		//En esta parte escoges el color que vas a seleccionar para la esfera
		//amarillo.Draw(staticShader);
		//azul.Draw(staticShader);
		//blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(10.0f, 10.0f, 0.0f));
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
		//amarillo.Draw(staticShader);
		//azul.Draw(staticShader);
		blanco.Draw(staticShader);
		//gris.Draw(staticShader);
		//naranja.Draw(staticShader);
		//negro.Draw(staticShader);
		//rojo.Draw(staticShader);
		//rosa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(10.0f, 40.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		CrearSphereRenderizar();





		//pinguino con primitivas
		// muñeco de nieve con primitivas
		//staticShader.use();
		//staticShader.setMat4("projection", projection);
		//staticShader.setMat4("view", view);
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));//al origen
		//estas forman la esfera
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		//la posición de la luz debe estar x constante, Y+2 y z+1
		//staticShader.setVec3("pointLight[0].position", glm::vec3(0, 4, 1));
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.61424f, 0.04136f, 0.04136f));
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(0.727811f, 0.626959f, 0.626959f));
		//staticShader.setFloat("pointLight[0].constant", 0.08f);
		//staticShader.setFloat("pointLight[0].linear", 0.009f);
		//staticShader.setFloat("pointLight[0].quadratic", 0.0000032f); //más 0 es más brillante menos 0 menos brillante
		//staticShader.setFloat("material_shininess", 0.6f);
		//model = glm::scale(model, glm::vec3(40.0f));
		//model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.1f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//staticShader.setMat4("model", model);
		//CrearSphereRenderizar();
		//staticShader.use();
		


		// -------------------------------------------------------------------------------------------------------------------------
		// CUBO TEXTURIZADO
		// -------------------------------------------------------------------------------------------------------------------------
		texturaShader.use();
		texturaShader.setMat4("projection", projection);
		texturaShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, -100.0f));

		model = glm::scale(model, glm::vec3(2.0f, 50.0f, 100.0f));

		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", model);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[8]);
		glBindTexture(GL_TEXTURE_2D, t_wood_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);
		//myDataRenderizar();

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(100.0f, 50.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		texturaShader.setMat4("model", model);
		texturaShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[8]);
		glBindTexture(GL_TEXTURE_2D, t_brick_png);
		glDrawArrays(GL_TRIANGLES, 0, 36);        ///Muro
		glBindVertexArray(0);
		staticShader.use();




		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// 


		// -------------------------------------------------------------------------------------------------------------------------
		// FBX
		// -------------------------------------------------------------------------------------------------------------------------
		
		//Zunesha Elefante
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -500.0f, -1500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		fbxShader.setMat4("model", model);
		zuneshaFBX.Draw(fbxShader);


		//LUNA
		lunaShader.use();
		lunaShader.setMat4("projection", projection);
		lunaShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1800.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(700.0f));
		fbxShader.setMat4("model", model);
		moonFBX.Draw(fbxShader);

		lunaShader.setMat4("model", model);

		lunaShader.setFloat("time", proceduralTime2);
		lunaShader.setFloat("radius", 50.0f);
		lunaShader.setFloat("height", 10.0f);

		moonFBX.Draw(lunaShader);
		proceduralTime2 += 0.01;
		staticShader.use();

		//OLA con animación procedural
		// Activamos el shader de Phong
		wavesShader.use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
		wavesShader.setMat4("projection", projection);
		wavesShader.setMat4("view", view);

		// Aplicamos transformaciones del modelo
				
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, 150.0f, -220.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		model = glm::scale(model, glm::vec3(5.5f, 1.0f, 2.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		wavesShader.setMat4("model", model);

		wavesShader.setFloat("time", wavesTime);
		wavesShader.setFloat("radius", 5.0f);
		wavesShader.setFloat("height", 5.0f);

		olaFBX.Draw(wavesShader);
		wavesTime += 0.01;

		staticShader.use();
		

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

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
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
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
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	
	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;


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
