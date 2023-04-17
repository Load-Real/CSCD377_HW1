#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

GLuint  vao;
GLuint  vbo[2]; //IDs for vertex positions and colors
GLuint v, f;
GLboolean show_line = false;

GLfloat vertices[18][2] = { { 0.0, 0.0},  // Triangle 1
                            { 0.9, 0.0},
                            { 0.45, 0.75},

						    { 0.0, 0.0},  // Triangle 2
						    { 0.45, 0.75},
						    { -0.45, 0.75},

						    { 0.0, 0.0},  // Triangle 3
						    { -0.45, 0.75},
						    { -0.9, 0.0},

						    {0.0, 0.0},   // Triangle 4
						    {-0.9, 0.0},
						    {-0.45, -0.75},

						    {0.0, 0.0},   // Triangle 5
						    {-0.45, -0.75},
						    {0.45, -0.75},

							{0.0, 0.0},   // Triangle 6
							{0.9, 0},
							{0.45, -0.75},
                         };

//All the colors to fill in each triangle of the hexagon
//Each lines corresponds to the following vertex above
GLfloat colors[18][4] = {
//  { R,   G,   B,   A}
	{0.0, 0.0, 1.0, 1.0},  //color = blue (triangle 1)
	{0.0, 0.0, 1.0, 1.0},
	{0.0, 0.0, 1.0, 1.0},

	{1.0, 1.0, 0.0, 1.0},  //color = yellow (triangle 2)
	{1.0, 1.0, 0.0, 1.0},
	{1.0, 1.0, 0.0, 1.0},

	{0.0, 1.0, 0.0, 1.0},  //color = green {triangle 3)
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},

	{0.0, 1.0, 1.0, 1.0},  //color = cyan (triangle 4)
	{0.0, 1.0, 1.0, 1.0},
	{0.0, 1.0, 1.0, 1.0},

	{1.0, 0.0, 1.0, 1.0}, //color = magenta (triangle 5)
	{1.0, 0.0, 1.0, 1.0},
	{1.0, 0.0, 1.0, 1.0},

	{1.0, 0.0, 0.0, 1.0}, //color = red (triangle 6)
	{1.0, 0.0, 0.0, 1.0},
	{1.0, 0.0, 0.0, 1.0},
};

char* ReadFile(const char* filename);
GLuint initShaders(char* v_shader, char* f_shader);
void init();
void display(void);

/************************************************/

char* ReadFile(const char* filename) {

	FILE* infile;
#ifdef WIN32
	fopen_s(&infile, filename, "rb");
#else
	infile = fopen(filename, "rb");
#endif


	if (!infile) {
		printf("Unable to open file %s\n", filename);
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	char* source = (char*)malloc(len + 1);
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	return (source);

}

/*************************************************************/

GLuint initShaders(const char* v_shader, const char* f_shader) {

	GLuint p = glCreateProgram();

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile(v_shader);
	const char * fs = ReadFile(f_shader);

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);

	free((char*)vs);
	free((char*)fs);

	glCompileShader(v);

	GLint compiled;

	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

		char* log = (char*)malloc(len + 1);

		glGetShaderInfoLog(v, len, &len, log);

		printf("Vertex Shader compilation failed: %s\n", log);

		free(log);

	}

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {

		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len + 1);
		glGetShaderInfoLog(f, len, &len, log);
		printf("Vertex Shader compilation failed: %s\n", log);
		free(log);
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);
	GLint linked;

	glGetProgramiv(p, GL_LINK_STATUS, &linked);

	if (!linked) {

		GLsizei len;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)malloc(len + 1);
		glGetProgramInfoLog(p, len, &len, log);
		printf("Shader linking failed: %s\n", log);
		free(log);
	}

	glUseProgram(p);

	return p;

}

/*******************************************************/

void init() {

	GLuint program = initShaders("hexagonColored.vs", "hexagonColored.fs");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//glGenBuffers updated to 2 in order to hold both COLORS and VERTICES
	glGenBuffers(2, vbo); //try removing & later

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Second segment made for COLORS
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

/**********************************************************/
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 's':
		show_line = !show_line;
		break;
	}
	glutPostRedisplay();
}

/**********************************************************/

void display(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(5.0);
	if (show_line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 18);

	glFlush();

}

/*******************************************************/

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Hexagon Incomplete");

	if (glewInit()) {
		printf("%s\n", "Unable to initialize GLEW ...");
	}

	init();
	/* we can now get data for the specific OpenGL instance we created */
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	
	printf("GLSL Version : %s\n", glslVersion);
	printf("%s\n", glGetString(GL_VERSION));
	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	return 0;

}

/*******************************************************/