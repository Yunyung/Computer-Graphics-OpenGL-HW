#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stb_image.h>
#include <cmath>
#include <vector>
#define M_PI 3.14159265358979323846

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//------------- TO DO ------------- 
// Declaration: ex. mouse events variables, camera positions ...
unsigned int drawedObjectID = 1;
char keyboard_changeCameraPosition_state = 'N'; // N = "No change", (W,A,S,D) = Change camera view
double keyboard_cameraPosition_x = 0, keyboard_cameraPosition_y = 0, keyboard_cameraPosition_z = 0;
// mouse event
bool mouseLeft_Clicked = false;
double mouse_start_x = 0, mouse_start_y = 0, mouse_now_x = 0, mouse_now_y = 0, mouse_cummulative_x = 0, mouse_cummulative_y = 0;

void mySphere(double r, int slice, int stack, unsigned int texture) {
    //------------- TO DO ------------- 
    // Draw your sphere

    // Calculate Position, normal, textCoord
    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;
    float x, y, z, xz;                         // vertex position
    float nx, ny, nz, lengthInv = 1.0f / r;    // vertex normal
    float s, t;                                // vertex textCoord

    float sliceStep = 2 * M_PI / slice;
    float stackStep = M_PI / stack;
    float sliceAngle, stackAngle;

    for (int i = 0;i <= stack;++i) {
        stackAngle = M_PI / 2 - i * stack;        // starting from pi/2 to -pi/2
        xz = r * cos(stackAngle);             // r * cos(u)
        y = r * sin(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= slice; ++j) {
            sliceAngle = j * slice;           // starting from 0 to 2pi
            // vertex position (x, y, z)
            z = xz * cosf(sliceAngle);             // r * cos(u) * cos(v)
            x = xz * sinf(sliceAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(z);
            vertices.push_back(y);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(nz);
            normals.push_back(ny);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / slice;
            t = (float)i / stack;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }
    
    // Enable & Bind Texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw objects
    glBegin(GL_QUAD_STRIP);

    for (int i = 0; i <= 360; i++) {
        int index = i * 3;
        cout << "(" << vertices[index] << "," << vertices[index + 1] << "," << vertices[index + 2] << ")";

        glNormal3f(normals[index], normals[index + 1], normals[index + 2]);
        glTexCoord2f(texCoords[index], texCoords[index + 1]);
        glVertex3f(vertices[index], vertices[index + 1], vertices[index + 2]);
    }

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnd();
}

void myCube2(unsigned int texture) {
    // Enable & Bind Texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw objects
    glBegin(GL_QUADS);


    //--fornt--
    // Order -> Bottom left, Top left, Top right, Bottom right
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);


    //--Left--
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    //--Right--
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    //--Top--
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    //--Bottom--
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnd();
}
void myCube(unsigned int texture) {
    //------------- TO DO ------------- 
    // Draw your cube

    // Enable & Bind Texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw objects
    glBegin(GL_QUADS);


    //--fornt--
    // Order -> Bottom left, Top left, Top right, Bottom right
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    
    //--Left--
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    //--Right--
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    //--Top--
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    //--Bottom--
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnd();
}

int main()
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG_HW1_TA", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    unsigned int texture1, texture2, texture3;
    //------------- TO DO ------------- 
    // Generate textures

    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);    
    glGenTextures(1, &texture3);

    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    int w, h, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("../resources/container.jpg", &w, &h, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else{
        cout << "Failed to load texture" << endl;
    }
    // stbi_image_free(data);

 
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("../resources/earth.jpg", &w, &h, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);






    //------------- TO DO ------------- 
    // Enable lighting
    GLfloat sun_light_position[] = { 10.0f, 10.0f, 10.0f, 0.0f};
    GLfloat sun_light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);


    

    while (!glfwWindowShouldClose(window))
    {

        //------------- TO DO ------------- 
        //Declare your variables




        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        //------------- TO DO ------------- 

        
        //Projection Matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        gluPerspective(45.0f, width / (GLfloat)height, 0.1, 1000);
        //Viewport Matrix
        glViewport(0, 0, width, height);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0 + keyboard_cameraPosition_x + mouse_cummulative_x + (mouse_now_x - mouse_start_x) * 0.01, 0 + keyboard_cameraPosition_y + mouse_cummulative_y + (mouse_now_y - mouse_start_y) * -0.01, 5 + keyboard_cameraPosition_z,
                  0 + keyboard_cameraPosition_x + mouse_cummulative_x + (mouse_now_x - mouse_start_x) * 0.01, 0 + keyboard_cameraPosition_y + mouse_cummulative_y + (mouse_now_y - mouse_start_y) * -0.01, -1,
                  0, 1, 0); // these are the defaults
       

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);

        //------------- TO DO ------------- 
        // Enable GL capabilities
        glEnable(GL_DEPTH_TEST);



        // clear
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //------------- TO DO ------------- 
        // Rotate, Draw and Switch objects
        glRotatef((float)glfwGetTime() * 50.0f, 1.0f, 1.0f, 0.0f);
        
        switch (drawedObjectID) {
        case 1:
            myCube(texture1);
            break;
        case 2:
            mySphere(1, 360, 180, texture2);
            break;
        case 3:
            mySphere(1, 360, 180, texture3);
            break;
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();

        // keyboard event change camera view
        if (keyboard_changeCameraPosition_state != 'N') {
            switch (keyboard_changeCameraPosition_state) {
            case 'W':
                keyboard_cameraPosition_y += 0.05;
                break;
            case 'S':
                keyboard_cameraPosition_y -= 0.05;
                break;
            case 'A':
                keyboard_cameraPosition_x -= 0.05;
                break;
            case 'D':
                keyboard_cameraPosition_x += 0.05;
                break;
            }

        }
        // mouse drag
        if (mouseLeft_Clicked) {
            glfwGetCursorPos(window, &mouse_now_x, &mouse_now_y);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    //------------- TO DO ------------- 
    // Define your keyboard event

    //--switch switching three objects--
    // Cube
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        drawedObjectID = 1;
    }
    // Shperes with earth texture
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        drawedObjectID = 2;
    }
    // Shperes with moon texture
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        drawedObjectID = 3;
    }

    //--keyboard event change camera view--
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        keyboard_cameraPosition_y += 0.05;
        keyboard_changeCameraPosition_state = 'W';
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        keyboard_cameraPosition_y -= 0.05;
        keyboard_changeCameraPosition_state = 'S';
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        keyboard_cameraPosition_x -= 0.05;
        keyboard_changeCameraPosition_state = 'A';
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        keyboard_cameraPosition_x += 0.05;
        keyboard_changeCameraPosition_state = 'D';
    }

    if (keyboard_changeCameraPosition_state != 'N' && action == GLFW_RELEASE) {
        keyboard_changeCameraPosition_state = 'N';
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    //------------- TO DO ------------- 
    // Define your mouse event
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        keyboard_cameraPosition_x = 0;
        keyboard_cameraPosition_y = 0;
        keyboard_cameraPosition_z = 0;
        mouse_start_x = mouse_start_y = mouse_now_x = mouse_now_y = mouse_cummulative_x = mouse_cummulative_y = 0;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mouseLeft_Clicked = true;
        //getting cursor position
        glfwGetCursorPos(window, &mouse_start_x, &mouse_start_y);
        mouse_now_x = mouse_start_x;
        mouse_now_y = mouse_start_y;
    
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mouseLeft_Clicked = false;
        // record object's current position
        mouse_cummulative_x += (mouse_now_x - mouse_start_x) * 0.01;
        mouse_cummulative_y += (mouse_now_y - mouse_start_y) * -0.01;
        mouse_start_x = mouse_start_y = mouse_now_x = mouse_now_y = 0;
    }

    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //------------- TO DO ------------- 
    // (optional) Define your scroll event
    keyboard_cameraPosition_z += -yoffset * 0.2;
    cout << keyboard_cameraPosition_z;
}